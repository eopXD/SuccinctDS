// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file bv_lookup.hpp
	\brief bv_lookup.hpp uses pre-built lookup table and compress
	bitstring with expression that can be indexed to the table. Blocking
	to the bitstring also makes querying on rank/select constant time.

	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/
#ifndef EOPXD_BITVECTOR_LOOKUP_HPP
#define EOPXD_BITVECTOR_LOOKUP_HPP

// C99
#include <stdint.h>
#include <cassert>
#include <cstdlib>

// C++
#include <iostream>

//! namespace by eopXD
namespace eopxd {

/*
***************************
**     Lookup Table      **
***************************

Block bitstring into fixed length blocks, for the blocks we need the following
lookup table, n-bit block takes memory of:
- (2^n) * (log n) bit: [bitstring] = popcount
- (2^n) * (log n) bit: [bitstring] = permutation
- (2^n) * n : [popcount][permutation] = bitstring

It is appropriate to set BasicBlock size to 15.
b = 15, we would need 2^{15} * (4+4+15) bit ~ 1MB

Using this length:

- save popcount values in uint8_t
- save bitstring in uint16_t
- save permutation in uint16_t ( log2(15!/7!/8!)~12.X )

With fixed length variable these tables cost 2^{15} * (8+16+16) = 1.25 MB
*/

const uint64_t blk_size = 15; 			// 15 bit per basic block
const uint64_t superblk_size = 128; 	// 128 block per superblock

uint8_t *lookpop = nullptr;				// [bitstring] = popcount
uint16_t *lookperm = nullptr;			// [bitstring] = permutation
uint16_t *revlookup = nullptr;			// [popcount][permutation] = bitstring

uint64_t *step = nullptr, *permsum = nullptr;

uint64_t next_perm ( uint64_t v ) { // the next pemutation (popcount-wise)
	uint64_t t = (v|(v-1))+1;
	uint64_t w = t|((((t&-t)/(v&-v))>>1)-1);
	return (w);
}
uint64_t elem_0 ( uint64_t c ) { // first element of a given popcount
	return ((1<<c)-1);
}

void genlookup ()  { 
	
	step = new uint64_t [blk_size+5];
	permsum = new uint64_t [blk_size+5];
	lookpop = new uint8_t [(1<<blk_size)+5];
	lookperm = new uint16_t [(1<<blk_size)+5];
	revlookup = new uint16_t [(1<<blk_size)+5];

	step[0]=1;
	permsum[0] = 0, permsum[1] = 1;
	
	for ( uint64_t i=1; i<=blk_size+1; ++i ) step[i] = step[i-1]*i;
	for ( uint64_t i=1; i<=blk_size; ++i ) 
		permsum[i+1] = permsum[i]+step[blk_size]/step[i]/step[blk_size-i];
	

	lookpop[0] = lookperm[0] = revlookup[0] = 0;
	
	uint64_t now = 1, blk_mask = elem_0(blk_size);
	for ( uint8_t popcnt=1; popcnt<=blk_size; ++popcnt ) {
		uint16_t v, initial, permcnt = 0;
		v = initial = elem_0(popcnt);
		while ( v >= initial ) {
			revlookup[now++] = v;
			lookpop[v] = popcnt;
			lookperm[v] = permcnt++;
			v = next_perm(v) & blk_mask;
		}
	}

	//std::cout << "step & persum: " << sizeof(uint64_t)*(blk_size+5)*2 << "\n";
	//std::cout << "lookperm & revlookup: " << sizeof(uint16_t)*((1<<blk_size)+5)*2 << "\n";
	//std::cout << "lookpop: " << sizeof(uint8_t)*((1<<blk_size)+5) << "\n";
	
}

uint8_t get_pop ( uint16_t bitstring ) { 
	return (lookpop[bitstring]); 
}
uint16_t get_perm ( uint16_t bitstring ) { 
	return (lookperm[bitstring]); 
}
uint16_t get_bitstring ( uint8_t popcnt, uint16_t perm ) { 
	return (revlookup[permsum[popcnt]+perm]); 
}

const uint64_t popblk_size = blk_size*superblk_size;

struct bv_lookup {

	typedef uint8_t UINT8;
	typedef uint16_t UINT16;
	typedef uint64_t UINT64;
	
	typedef misalign<1, UINT8, 8> COMPACT_BOOL;
	typedef misalign<4, UINT8, 8> COMPACT_POP;
	typedef misalign<13, UINT16, 16> COMPACT_PERM;

	UINT64 len;
	
	COMPACT_BOOL *bv;
	COMPACT_POP *pop;
	COMPACT_PERM *perm;
	UINT64 *superpop;

	UINT64 blkcnt, superblkcnt;
	
	UINT64 mem_used;
	bool rank_support;
	
	void account_mem () { // 65 Byte
		mem_used += sizeof(COMPACT_BOOL*); // 8 Byte
		mem_used += sizeof(COMPACT_POP*); // 8 Byte
		mem_used += sizeof(COMPACT_PERM*); // 8 Byte
		mem_used += sizeof(UINT64*); // 8 Byte
		mem_used += sizeof(bool); // 1 Byte (sizeof(bool) == sizeof(char))
		mem_used += sizeof(UINT64)*4; // 8 Byte * 4
	}

	// default constructor (SHALL NOT BE USED)
	bv_lookup () {
		std::cout << "usage: bv_lookup(allocated_length)\n";
		exit(1);
	}

	// main constructor
	bv_lookup ( UINT64 _len ) {
		mem_used = 0;
		rank_support = false;

		len = _len;
		bv = new COMPACT_BOOL(len);
		mem_used += bv->mem_used;

		blkcnt = superblkcnt = 0;
		pop = nullptr, perm = nullptr, superpop = nullptr;
		
		account_mem();
	}

	// destructor
	~bv_lookup () {
		if ( bv != nullptr ) {
			delete bv;
		}
		if ( pop != nullptr ) {
			delete pop;
		}
		if ( perm != nullptr ) {
			delete perm;
		}
		if ( superpop != nullptr ) {
			delete[] superpop;
		}
	}

	// call this to delete original bitstring
	void kill_bv () {
		if ( bv ) {
			mem_used -= bv->mem_used;
			delete bv;
		}
		bv = nullptr;
	}

	// make bitvec to representations (2-level blocking)
	void support_rank () { // rank support also changes memory used
		if ( lookpop == nullptr ) {
			//std::cout << "[bv_lookup]: construct lookup table\n";
			genlookup();
		}

		pop = new COMPACT_POP((len+blk_size-1)/blk_size);
		perm = new COMPACT_PERM((len+blk_size-1)/blk_size);
		superpop = new UINT64 [(len+popblk_size-1)/popblk_size];

		blkcnt = superblkcnt = 0;
		superpop[superblkcnt++] = 0;

		UINT16 bitstring = 0;
		UINT64 nowpop = 0;
		for ( UINT64 i=1; i<=len; ++i ) {
			bitstring = (bitstring<<1) + bv->access(i-1);
			if ( i%blk_size == 0 ) {
				UINT8 _pop = get_pop(bitstring);
				UINT16 _perm = get_perm(bitstring);

				pop->assign(blkcnt, _pop, 0);
				perm->assign(blkcnt, _perm, 0);

				nowpop += _pop;
				bitstring = 0;
				++blkcnt;
				if ( blkcnt%superblk_size == 0 ) {
					superpop[superblkcnt] = nowpop;
					++superblkcnt;
				}
			}
		}

		if ( bitstring != 0 ) {
			bitstring = bitstring<<(blk_size-(len%blk_size));
			UINT8 _pop = get_pop(bitstring);
			UINT16 _perm = get_perm(bitstring);

			pop->assign(blkcnt, _pop, 0);
			perm->assign(blkcnt, _perm, 0);

			nowpop += _pop;
			bitstring = 0;
			++blkcnt;
			if ( blkcnt%superblk_size == 0 ) {
				superpop[superblkcnt] = nowpop;
				++superblkcnt;		
			}
		}


		rank_support = true;
		kill_bv(); // the real bitvec is no longer needed (to save space)
		
		mem_used += pop->mem_used;
		mem_used += perm->mem_used;
		mem_used += sizeof(UINT64)*((len+popblk_size-1)/popblk_size);
		
		//std::cout<< "support_rank\n";
		//std::cout << "pop: " << sizeof(UINT8)*((len+blk_size-1)/blk_size) << "\n";
		//std::cout << "perm: " << sizeof(UINT16)*((len+blk_size-1)/blk_size) << "\n";
		//std::cout << "superpop: " << sizeof(UINT64)*((len+popblk_size-1)/popblk_size) << "\n";
	}
	void assign ( UINT64 p, UINT8 val ) {
		if ( val > 1 ) {
			std::cout << "[bv_lookup]: assign(), receive bad value\n";
			exit(1);
		}
		bv->assign(p, val, 1); // pos, value, dirty_bit
	}
	bool access ( UINT64 p ) {
		if ( bv != nullptr ) {
			return (bv->access(p));
		}
		else if ( rank_support ) {
			UINT64 blk_num = p/blk_size;
			UINT16 bitstring = 
				get_bitstring(pop->access(blk_num), perm->access(blk_num));
			return ((bitstring>>(blk_size-(p%(UINT64)blk_size)-1))&1);
		}
		else {
			std::cout << "[bv_lookup]: access(), something is wrong 😱\n";
			exit(1);
		}
	}
	// rank[0~p)
	UINT64 rank ( UINT64 p, bool c ) {
		if ( rank_support ) {
			UINT64 blk_num = p/blk_size;
			UINT64 superblk_num = blk_num/superblk_size;
			UINT64 res = superpop[superblk_num];
			for ( UINT64 i=superblk_num*superblk_size; i<blk_num; ++i ) {
				res += pop->access(i);
			}
			UINT16 bitstring = 
				get_bitstring(pop->access(blk_num), perm->access(blk_num));
			res += __builtin_popcount(bitstring>>(blk_size-(p%(UINT64)blk_size)));
			return (c==1) ? (res) : (p-res);
		}
		else {
			std::cout << "no rank support\n";
			exit(1);
		}
	}

	void display_rank () {
		std::cout << "[bv_lookup]:\n";
		std::cout << "\tlength: " << len << "\n";
		std::cout << "\tbasic blocks: " << blkcnt << "\n";
		std::cout << "\tsuper blocks: " << superblkcnt << "\n\t";
		std::cout << "\n==================================================\n";
	}
	void display_bitmap () {
		std::cout << "[bv_lookup]:\n";
		std::cout << "\tlength: " << len << "\n";
		std::cout << "\tbitmap: ";
		for ( UINT64 i=0; i<len; ++i ) {
			std::cout << bv->access(i);
		}
		std::cout << "\n==================================================\n";	
	}

}; // end struct bv_lookup

} // end namespace
#endif
