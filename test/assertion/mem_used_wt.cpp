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

typedef uint64_t INT;
typedef unsigned char UCHAR;

UCHAR test_string[10] = "aaaabbaac";
int main ()
{
	INT n = strlen((char*)test_string);
	int bpa = 1;
	wt<wt_huff<bv_lookup>, bv_lookup> *wt_ptr = 
		new wt<wt_huff<bv_lookup>, bv_lookup>(test_string, n, bpa);  		
	std::cout << "memory_used: " << wt_ptr->mem_used << "\n";
	wt_ptr->support_rank();
	std::cout << "memory_used: " << wt_ptr->mem_used << "\n";


	return (0);
}
