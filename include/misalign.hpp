// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file misalign.hpp
    \brief misalign.hpp is the library to used integers with misaligned length
    It is useful when we want to achieve better compression rate, however the 
    overhead will be the cost of searching the position of the data and 
    decompressing the packed integer into use-able aligned ones.
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/
#ifndef EOPXD_MISALIGN_HPP
#define EOPXD_MISALIGN_HPP

// C99
#include <stdint.h>
// C++
#include <iostream>

namespace eopxd {

template<uint64_t needed_bit_len, class medium, uint64_t medium_bit_len> 
struct misalign { // needed_bit_len < medium_bit_len
	typedef unsigned char UCHAR;
	typedef uint64_t UINT64;
	typedef medium VAR;
	
	VAR *data;
	UINT64 data_len;

	UINT64 max_pos;
	VAR max_val;

	UINT64 mem_used; // memory used

// utility numbers (should be private members and not for any public usage)
// calculation on utility overhead needed 
	VAR diff; // difference between medium length and compact length
	VAR full; // 64-bit full oness
	VAR ones[64+1][64+1];
	/*a bitstring of length 7:
		ones[1][3] = 1000111
		ones[2][3] = 1100111
		ones[left_ones][right_ones] = bitstring
	*/

	void init_ones () {
		for ( UINT64 l=0; l<=medium_bit_len; ++l ) {
			for ( UINT64 r=0; r<=medium_bit_len; ++r ) {
				ones[l][r] = full;
				if ( l+r < medium_bit_len ) {
					ones[l][r] &= full>>l;
					ones[l][r] &= full<<r;
					ones[l][r] = ~ones[l][r];
				}
			}
		}
	}

	misalign () { // this shall not be called
		std::cout << "you should not call me with no parameter lah\n";
		std::cout << "ex: call misalign<7, uint8_t, 8> test(allocate_size);\n";
		exit(1);
	}
	misalign ( UINT64 allocate_size ) {
		if ( needed_bit_len > medium_bit_len ) {
			std::cout << "needed_bit_len needs to be smaller than medium_bit_len\n";
			exit(1);
		}
		data_len = (needed_bit_len*allocate_size+medium_bit_len-1)/medium_bit_len;
		data = new VAR [data_len];
		for ( UINT64 i=0; i<data_len; ++i ) { // all zeros filled in!
			data[i] = 0;
		}
		diff = medium_bit_len - needed_bit_len;
		full = (1ULL<<(medium_bit_len))-1;
		init_ones();

		max_pos = allocate_size-1;
		max_val = (1ULL<<needed_bit_len)-1;
	}
	// position start from zero
	void assign ( UINT64 pos, VAR val, bool dirty ) { // if dirty, cleans the space first
		if ( pos > max_pos ) {
			std::cout << "illegal access\n";
			exit(1);
		}
		if ( val > max_val ) {
			std::cout << "value to big\n";
			exit(1);
		}
		UINT64 l = pos*needed_bit_len, r = l+needed_bit_len-1;
		UINT64 blk_l = l/medium_bit_len, blk_r = r/medium_bit_len;
		UINT64 a = l%medium_bit_len, b = medium_bit_len-(r%medium_bit_len)-1;
		if ( blk_l == blk_r ) { // in same block
			if ( dirty ) { 
				data[blk_l] &= ones[a][b];
			}
			val <<= b;
			data[blk_l] |= val;
		}
		else { // goes across 2 blocks
			if ( dirty ) {
				data[blk_l] &= ones[a][0];
				data[blk_r] &= ones[0][b];
			}
			data[blk_l] |= val>>(a-diff);
			data[blk_r] |= val<<(diff+medium_bit_len-a);
		}
	}
	// position start from zero
	VAR access ( UINT64 pos ) {
		if ( pos > max_pos ) {
			std::cout << "illegal access\n";
			exit(1);
		}
		UINT64 l = pos*needed_bit_len, r = l+needed_bit_len-1;
		UINT64 blk_l = l/medium_bit_len, blk_r = r/medium_bit_len;
		UINT64 a = l%medium_bit_len, b = medium_bit_len-(r%medium_bit_len)-1;
		VAR res;
		if ( blk_l == blk_r ) { // in same block
			res = data[blk_l];
			res <<= a;
			res >>= (a+b);
			return (res);
		}
		else { // goes across 2 blocks
			res = data[blk_l];
			res <<= a;
			res >>=diff;
			res |= data[blk_r]>>b;
			return (res);
		}
	}
};

} // end namespace
#endif
