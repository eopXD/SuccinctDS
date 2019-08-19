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

UCHAR test_string[500] = "YOOOO every one, this is eopXD! glad u be checking \
through these assertion codes to know that correctness is assured in the best \
standard";
int main ()
{
	UINT64 n = strlen((char*)test_string);
	int bpa = 1;
	wt<wt_huff<bv_lookup>, bv_lookup> *wt_ptr = 
		new wt<wt_huff<bv_lookup>, bv_lookup>(test_string, n, bpa);  		
	
	UINT64 before_support;

	std::cout << "tree: " << wt_ptr->tree->mem_used << "\n";
	std::cout << "root: " << wt_ptr->tree->root->mem_used << "\n";
	std::cout << "root->bv: " << wt_ptr->tree->root->bitmap->mem_used << "\n";
	std::cout << "root->bv->compact_bv: " << wt_ptr->tree->root->bitmap->bv->mem_used << "\n";

	std::cout << "\nbefore support_rank()\n";
	std::cout << "memory_used: " << wt_ptr->mem_used << "\n";
	before_support = wt_ptr->mem_used;

	wt_ptr->support_rank();
	
	std::cout << "\nafter support_rank()\n";
	std::cout << "memory_used: " << wt_ptr->mem_used << "\n";

	std::cout << "\nratio: " << (double)wt_ptr->mem_used/before_support << "\n";

	return (0);
}
