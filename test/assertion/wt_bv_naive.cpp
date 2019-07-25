// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file test_wt_huff_bv_naive.cpp
    \brief test access/rank/select for wt<huff_wt<bv_naive>, bv_naive>
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/

#include <cstdio>
#include <cstring>
#include <stdint.h>
#include <cassert>
#include <random>
#include <iostream>

#include "include/wt.hpp"

typedef uint64_t INT;

char str[1000] = "Hi everyone, welcome to the succinct wavelet tree \
library! I am eopXD.\nThis library seeks for compression of data will \
maintaining well accessibility, and received help from Professor Tsan-sheng\
Hsu.\nI am a test string and will be encoded into the wavelet tree.\nIt can \
be accessed by rank/select/access using the underlying bitvector \
implementation.\nThe wavelet tree is also Huffman-shaped because it compresses \
the input string data into zero-th order entropy.\nFurther compression can \
be done to the bitstring, which is left as a possible future work.\nThank you \
for your attention.\n";

INT naive_rank ( int blk_hash, INT pos, int bpa ) {
	INT res = 0;
	int tmp_hash = 0, len = 0;
	for ( INT i=0; i<pos; ++i ) {
		++len;
		tmp_hash = (tmp_hash<<8)+str[i];
		if ( len == bpa ) {
			if ( tmp_hash == blk_hash ) {
				++res;
			}
			tmp_hash = len = 0;
		}
	}
	return (res);
}
// 0th occurence is the first occurence
INT naive_select ( int blk_hash, INT n, INT occ, int bpa ) {
	if ( occ < 0 ) {
		return (-1);
	}
	INT cnt = 0;
	int tmp_hash = 0, len = 0;
	for ( INT i=0; i<n; ++i ) {
		++len;
		tmp_hash = (tmp_hash<<8)+str[i];
		if ( len == bpa ) {
			if ( tmp_hash == blk_hash ) {
				++cnt;
				if ( (cnt-1) == occ ) {
					return (i);
				}
			}
			tmp_hash = len = 0;
		}
	}
	return (-1);
}
using namespace eopxd;
int main ()
{
 	INT n = strlen(str);;
 	int bpa = 1;

 	wt<wt_huff<bv_naive>, bv_naive> *wt_ptr = 
 		new wt<wt_huff<bv_naive>, bv_naive>(str, n, bpa);
 	wt<wt_huff<bv_naive>, bv_naive> wt_var(str, n, bpa);

 	wt_ptr->support_rank();
 	wt_ptr->support_select();
 	wt_var.support_rank();
 	wt_var.support_select();

	for ( INT i=0; i<n; ++i ) {
		char *a0 = wt_ptr->access(i);
		char *a1 = wt_var.access(i);
		assert(a0[0] == str[i]);
		assert(a1[0] == str[i]);
	}
	std::cout << "pass access assertion\n";

	std::string vowel = "aeiou";
	INT v_sz = vowel.size();
	for ( INT pos=0; pos<n; ++pos ) {
		for ( INT j=0; j<v_sz; ++j ) {
			int blk_hash = vowel[j];
			INT r0 = wt_ptr->rank(blk_hash, pos);
			INT r1 = wt_var.rank(blk_hash, pos);
			assert(r0 == naive_rank(blk_hash, pos, bpa));
			assert(r1 == naive_rank(blk_hash, pos, bpa));			
			for ( INT k=0; k<10; ++k ) { // select op on random occ.
				INT occ = (pos==0) ? 0 : rand()%pos;
				INT s0 = wt_ptr->select(blk_hash, occ);
				INT s1 = wt_var.select(blk_hash, occ);
				assert(s0 == naive_select(blk_hash, n, occ, bpa));
				assert(s1 == naive_select(blk_hash, n, occ, bpa));
			}
		}
	}
	std::cout << "pass rank/select assertion\n";
	exit(0);
}
