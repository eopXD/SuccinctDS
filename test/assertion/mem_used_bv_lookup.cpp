// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file mem_used.cpp
    \brief test if the memoery accounting is correct
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
using namespace eopxd;

typedef uint64_t UINT64;
typedef unsigned char UCHAR;

UCHAR test_string[11] = "aaaaaaaabb"; // a*8, b*2


int main ()
{
	srand(112358);
//	std::cout << "test string: " << sizeof(test_string) << "\n"; 
/* test bitstring mem_used*/
	int n = 100000000, m = 50000;
	bv_lookup *bv_ptr = new bv_lookup(n);
	bv_lookup bv_var(n);
	bool *ans = new bool [n];
	for ( int i=0; i<n; ++i ) {
		bv_ptr->assign(i, 0);
		bv_var.assign(i, 0);
		ans[i] = 0;
	}
	for ( int i=0; i<m; ++i ) {
		int x = rand()%n;
		bv_ptr->assign(x, 1);
		bv_var.assign(x, 1);
		ans[x] = 1;
	}

	UINT64 before_support;
	before_support = bv_ptr->mem_used;

	std::cout << "bv_ptr: " << bv_ptr->mem_used << "\n\n";
	bv_ptr->support_rank();
	std::cout << "bv_ptr: " << bv_ptr->mem_used << "\n";
	std::cout << "compression rate: " << (double)bv_ptr->mem_used/before_support << "\n";
	std::cout << "=============================================\n";
	
	before_support = bv_var.mem_used;
	std::cout << "bv_var: " << bv_var.mem_used << "\n\n";
	bv_var.support_rank();
	std::cout << "bv_var: " << bv_var.mem_used << "\n";
	std::cout << "compression rate: " << (double)bv_var.mem_used/before_support << "\n";

	return (0);	
}
