// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file misalign.hpp
    \brief misalign.hpp is the library to used integers with misaligned length
    It is useful when we want to achieve better compression rate, however the 
    overhead will be the cost of searching the position of the data and 
    decompressing the packed integer into use-able aligned ones.
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/
// C99
#include <stdint.h>
#include <cassert>
#include <cstdlib>
#include <ctime>
// C++
#include <iostream>

void line () {
	std::cout << "===========================================\n";
}
void star_line () {
	std::cout << "*******************************************\n";
}

void decimal_to_binary ( int length, uint64_t x ) {
	int str[100] = {};
	int now = 0;
	while ( x > 0 ) {
		str[now++] = x%2;
		x /= 2;
	}
	for ( int i=length-1; i>=0; i-- ) {
		std::cout << str[i];
	} std::cout << "\n";
}

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
		for ( int l=0; l<=medium_bit_len; ++l ) {
			for ( int r=0; r<=medium_bit_len; ++r ) {
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
		for ( int i=0; i<data_len; ++i ) { // all zeros filled in!
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
int main ()
{
	srand(112358);
	uint64_t allocate_size = 10000;
	misalign<13, uint16_t, 16> compact13(allocate_size);
	uint16_t *ans;
	ans = new uint16_t [allocate_size];
/*	while ( 1 ) {
		int op;
		std::cout << "enter op (0: access, 1 assign): ";
		std::cin >> op;
		if ( op == 0 ) {
			std::cout << "enter position to access: ";
			int pos; 
			std::cin >> pos;
			uint16_t val = compact13.access(pos);
			std::cout << "access: " << (uint64_t)val << "\n";
		}
		else {
			std::cout << "enter position and value to assign: ";
			int pos, val;
			std::cin >> pos >> val;
			compact13.assign(pos, val, 1);
		}
	}*/
	for ( int i=0; i<10000; ++i ) {
		int pos = rand()%compact13.max_pos;
		int val = rand()%compact13.max_val;
		ans[pos] = val;
		compact13.assign(pos, val, 1);
		uint16_t res = compact13.access(pos);
		//std::cout << i << " " << pos << ": " << ans[pos] << " " << res << "\n";
		assert(ans[pos] == res);
//		break;
	}
	
	return (0);
}
