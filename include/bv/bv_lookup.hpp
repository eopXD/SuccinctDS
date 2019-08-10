// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file bitvector_lookup.hpp
	\brief bitvector_lookup.hpp uses pre-built lookup table and compress
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

void genlookup ( uint64_t *mem_used )  { 
	
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

	*(mem_used) += sizeof(uint64_t)*(blk_size+5)*2;
	*(mem_used) += sizeof(uint16_t)*((1<<blk_size)+5)*2;
	*(mem_used) += sizeof(uint8_t)*((1<<blk_size)+5);

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

	typedef uint64_t INTLEN;
	typedef uint16_t INTPERM;
	typedef uint8_t INTPOP;
	
	INTLEN len;
	bool *bitvec;
	bool rank_support;
	INTLEN blkcnt, superblkcnt;
	INTPOP *pop;
	INTPERM *perm;
	INTLEN *superpop;
	
	misalign<4, INTPOP, 8> *compact_pop;
	misalign<13, INTPERM, 16> *compact_perm;

	INTLEN mem_used;

	void account_mem () { // 65 Byte
		mem_used += sizeof(bool*); // 8 Byte
		mem_used += sizeof(INTPOP*); // 8 Byte
		mem_used += sizeof(INTPERM*); // 8 Byte
		mem_used += sizeof(INTLEN*); // 8 Byte
		mem_used += sizeof(bool); // 1 Byte (sizeof(bool) == sizeof(char))
		mem_used += sizeof(INTLEN)*4; // 8 Byte * 4
	}

	// default constructor (SHALL NOT BE USED)
	bv_lookup () {
		mem_used = 0;

		rank_support = false;
		len = blkcnt = superblkcnt = 0;
		bitvec = nullptr, pop = nullptr, perm = nullptr, superpop = nullptr;
		
		account_mem();
	}

	// main constructor
	bv_lookup ( INTLEN _len ) {
		mem_used = 0;

		rank_support = false;
		len = _len;
		bitvec = new bool [len];
		blkcnt = superblkcnt = 0;
		pop = nullptr, perm = nullptr, superpop = nullptr;
		
		mem_used += sizeof(bool)*len;
		account_mem();
	}

	// destructor
	~bv_lookup () {
		if ( bitvec ) {
			delete[] bitvec;
		}
		if ( pop ) {
			delete[] pop;
		}
		if ( perm ) {
			delete[] perm;
		}
		if ( superpop ) {
			delete[] superpop;
		}
	}

	// call this to delete original bitstring
	void kill_bitvec () {
		if ( bitvec ) {
			delete[] bitvec;
			mem_used -= sizeof(bool)*len;
			//std::cout << "bitvec deleted, size: " << sizeof(bool)*len << "\n";
		}
		bitvec = nullptr;
	}

	// make bitvec to representations (2-level blocking)
	void support_rank () { // rank support also changes memory used
		if ( lookpop == nullptr ) {
			//std::cout << "[bv_lookup]: construct lookup table\n";
			genlookup(&mem_used);
		}
		pop = new INTPOP [(len+blk_size-1)/blk_size];
		perm = new INTPERM [(len+blk_size-1)/blk_size];
		superpop = new INTLEN [(len+popblk_size-1)/popblk_size];

		compact_pop = new misalign<4, INTPOP, 8>((len+blk_size-1)/blk_size);
		compact_perm = new misalign<13, INTPERM, 16>((len+blk_size-1)/blk_size);

		blkcnt = superblkcnt = 0;
		superpop[superblkcnt++] = 0;

		INTPERM bitstring = 0;
		INTLEN nowpop = 0;
		for ( INTLEN i=1; i<=len; ++i ) {
			bitstring = bitstring*2 + bitvec[i-1];
			if ( i%blk_size == 0 ) {
				INTPOP _pop = get_pop(bitstring);
				INTPERM _perm = get_perm(bitstring);
				pop[blkcnt] = _pop;
				perm[blkcnt] = _perm;

				compact_pop->assign(blkcnt, _pop, 0);
				compact_perm->assign(blkcnt, _perm, 0);

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
			INTPOP _pop = get_pop(bitstring);
			INTPERM _perm = get_perm(bitstring);
			pop[blkcnt] = _pop;
			perm[blkcnt] = _perm;

			compact_pop->assign(blkcnt, _pop, 0);
			compact_perm->assign(blkcnt, _perm, 0);

			nowpop += _pop;
			bitstring = 0;
			++blkcnt;
			if ( blkcnt%superblk_size == 0 ) {
				superpop[superblkcnt] = nowpop;
				++superblkcnt;		
			}
		}


		rank_support = true;
		kill_bitvec(); // the real bitvec is no longer needed (to save space)
		
		mem_used += sizeof(INTPOP)*((len+blk_size-1)/blk_size);
		mem_used += sizeof(INTPERM)*((len+blk_size-1)/blk_size);
		mem_used += sizeof(INTLEN)*((len+popblk_size-1)/popblk_size);
		
		std::cout << "pop: " << sizeof(INTPOP)*((len+blk_size-1)/blk_size) << "\n";
 		std::cout << "compact_pop: " << compact_pop->mem_used << "\n";
		std::cout << "perm: " << sizeof(INTPERM)*((len+blk_size-1)/blk_size) << "\n";
 		std::cout << "compact_perm: " << compact_perm->mem_used << "\n";
		//std::cout<< "support_rank\n";
		//std::cout << "pop: " << sizeof(INTPOP)*((len+blk_size-1)/blk_size) << "\n";
		//std::cout << "perm: " << sizeof(INTPERM)*((len+blk_size-1)/blk_size) << "\n";
		//std::cout << "superpop: " << sizeof(INTLEN)*((len+popblk_size-1)/popblk_size) << "\n";
	}

	bool access ( INTLEN p ) {
		if ( bitvec != nullptr ) {
			return (bitvec[p]);
		}
		else if ( rank_support ) {
			INTLEN blk_num = p/blk_size;
			INTPERM bitstring = get_bitstring(pop[blk_num], perm[blk_num]);
			INTPERM _test = 
				get_bitstring(compact_pop->access(blk_num), compact_perm->access(blk_num));
			assert(bitstring == _test);
			return ((bitstring>>(blk_size-(p%(INTLEN)blk_size)-1))&1);
		}
		else {
			std::cout << "[bv_lookup]: something is wrong 😱\n";
			exit(1);
		}
	}
	// rank[0~p)
	INTLEN rank ( INTLEN p, bool c ) {
		if ( rank_support ) {
			INTLEN blk_num = p/blk_size;
			INTLEN superblk_num = blk_num/superblk_size;
			INTLEN res = superpop[superblk_num];
			INTLEN _res = superpop[superblk_num];
			for ( INTLEN i=superblk_num*superblk_size; i<blk_num; ++i ) {
				res += pop[i];
				_res += compact_pop->access(i);
			}
			assert(res == _res);
			INTPERM bitstring = get_bitstring(pop[blk_num], perm[blk_num]);
			INTPERM _test = 
				get_bitstring(compact_pop->access(blk_num), compact_perm->access(blk_num));
			assert(bitstring == _test);
			res += __builtin_popcount(bitstring>>(blk_size-(p%blk_size)));
			_res += __builtin_popcount(_test>>(blk_size-(p%blk_size)));
			assert(res == _res);
			return (c==1) ? (res) : (p-res);
		}
		else {
			std::cout << "no rank support\n";
			exit(1);
		}
	}

	void display_rank () {
		if ( pop == nullptr ) {
			std::cout << "[bv_lookup]: please call support_rank()\n";
			return ;
		}
		std::cout << "[bv_lookup]:\n";
		std::cout << "\tlength: " << len << "\n";
		std::cout << "\tbasic blocks: " << blkcnt << "\n";
		std::cout << "\tsuper blocks: " << superblkcnt << "\n\t";
		for ( INTLEN i=0; i<blkcnt; ++i ) {
			std::cout << "(" << unsigned(pop[i]) << "," << perm[i] << "), ";
		}
		std::cout << "\n==================================================\n";
	}
	void display_bitmap () {
		std::cout << "[bv_lookup]:\n";
		std::cout << "\tlength: " << len << "\n";
		std::cout << "\tbitmap: ";
		for ( INTLEN i=0; i<len; ++i ) {
			std::cout << bitvec[i];
		}
		std::cout << "\n==================================================\n";	
	}

}; // end struct bv_lookup

} // end namespace
#endif
