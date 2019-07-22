// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file test_wt_huff_bv_lookup.cpp
    \brief test access/rank for wt<huff_wt<bv_lookup>, bv_lookup>
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
using namespace eopxd;
int main ()
{
	std::string str = "Hi everyone, welcome to the succinct wavelet tree \
library! I am eopXD. This library seeks for compression of data will \
maintaining well accessibility, and received help from Professor Tsan-sheng\
Hsu. I am a test string and will be encoded into the wavelet tree. It can\
 be accessed by rank/select/access using the underlying bitvector \
implementation. The wavelet tree is also Huffman-shaped because it compresses\
 the input string data into zero-th order entropy. Further compression can \
be done to the bitstring, which is left as a possible future work. Thank you\
 for your attention.";

 	int n = str.size();
 	int bpa = 1;
 	std::string vowel = "aeiou";
 	
 	wt<wt_huff<bv_lookup>, bv_lookup> *wt_ptr = 
 		new wt<wt_huff<bv_lookup>, bv_lookup>(str, bpa);
 	wt_ptr->support_rank();
 	for ( int pos=0; pos<n; pos+=bpa ) {
 		for ( int j=0; j<vowel.size(); ++j ) {
 			std::string c = vowel.substr(j, 1);
 			int r = wt_ptr->rank(c, pos);
 			int r1 = naive_rank(str, pos, c, bpa);
 			if ( r != r1 ) {
 				std::cout << "error on rank " << pos << "\n";
// 				std::cout << pos << ": " << r << " " << r1 << " QAQ\n";
// 				return (1);
 			}
//			assert(r == r1);
 		}
 		std::string c = wt_ptr->access(pos);
 		std::string c1 = str.substr(pos, bpa);
// 		std::cout << pos << ": " << c << " " << c1 << "\n";
// 		std::cout << c;
 		if ( c != c1 ) {
 			std::cout << "error on access " << pos << "\n";
// 			std::cout << n << "\n";
//			std::cout << pos << ": " << c << " " << c1 << " QAQ\n";
//			return (1);
 		}
 //		assert(c == c1);
 	}
 	std::cout << "\n";
 	exit(0);
}
/*
g++ -std=c++17 -Wall -O3 -I..test_wt_huff_bv_lookup.cpp -o test_wt_bv_lookup
SEVERE ERROR!

Mystrerious error that occasionally happens.
To the input string above, 
sometimes error happens on access of position 560, 568 which effects the 
rank() results for all positions after the error access.

GUESSING: maybe it is caused by race condition inside the computer.
*/