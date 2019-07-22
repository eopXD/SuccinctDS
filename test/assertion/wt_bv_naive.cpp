// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file test_wt_huff_bv_naive.cpp
    \brief test access/rank/select for wt<huff_wt<bv_naive>, bv_naive>
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/

#include <cstdio>
#include <cassert>
#include <random>
#include <iostream>
#include <string>

#include "include/wt.hpp"

int naive_rank ( std::string s, int pos, std::string c, int bpa ) {
	int res = 0;
	for ( int i=0; i+bpa<=pos; i+=bpa ) {
		std::string x = s.substr(i, bpa);
		if ( x == c ) {
			++res;
		}
	}
	return (res);
}
// 0th occurence is the first occurence
int naive_select ( std::string s, int n, int occ, std::string c, int bpa ) {
	if ( occ < 0 ) {
		return (-1);
	}
	int cnt = 0;
	for ( int i=0; i<n; i+=bpa ) {
		std::string x = s.substr(i, bpa);
		cnt += (x == c);
		if ( (cnt-1) == occ ) {
			return (i);
		}
	}
	return (-1);
}
using namespace eopxd;
int main ()
{
	std::string str = "Hi everyone, welcome to the succinct wavelet tree \
library! I am eopXD.\nThis library seeks for compression of data will \
maintaining well accessibility, and received help from Professor Tsan-sheng\
Hsu.\nI am a test string and will be encoded into the wavelet tree.\nIt can\
 be accessed by rank/select/access using the underlying bitvector \
implementation.\nThe wavelet tree is also Huffman-shaped because it compresses\
 the input string data into zero-th order entropy.\nFurther compression can \
be done to the bitstring, which is left as a possible future work.\nThank you\
 for your attention.\n";

 	int n = str.size();
 	int bpa = 1;

 	wt<wt_huff<bv_naive>, bv_naive> *wt_ptr = 
 		new wt<wt_huff<bv_naive>, bv_naive>(str, bpa);
 	wt<wt_huff<bv_naive>, bv_naive> wt_var(str, bpa);

 	wt_ptr->support_rank();
 	wt_ptr->support_select();
 	wt_var.support_rank();
 	wt_var.support_select();

	for ( int i=0; i<n; ++i ) {
		std::string res = wt_ptr->access(i), res1 = wt_var.access(i);
		assert(res[0] == str[i]);
		assert(res1[0] == str[i]);
	}

	std::string vowel = "aeiou";
	for ( int pos=0; pos<n; ++pos ) {
		for ( int j=0; j<vowel.size(); ++j ) {
			std::string c = vowel.substr(j,1);
			int r = wt_ptr->rank(c, pos), r1 = wt_var.rank(c, pos);
			for ( int k=0; k<10; ++k ) {
				int occ = (pos==0) ? 0 : rand()%pos;
				int s = wt_ptr->select(c, occ), s1 = wt_var.select(c, occ);
				assert(s == naive_select(str, n, occ, c, bpa));
				assert(s1 == naive_select(str, n, occ, c, bpa));
			}
			assert(r == naive_rank(str, pos, c, bpa));
			assert(r1 == naive_rank(str, pos, c, bpa));			
		}
	}
	exit(0);
}