// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file stream.cpp
    \brief test read data in stream mode
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/

#include <cstdio>
#include <cassert>
#include <iostream>
#include <string>

#include "include/wt.hpp"

using namespace eopxd;

int main ()
{

	std::string filename = "test.file";
	std::streamsize size = get_size(filename);
	std::cout << size << "\n";
	int bpa = 1;
	wt<wt_huff<bv_naive>, bv_naive> *wt_ptr = 
		new wt<wt_huff<bv_naive>, bv_naive>(filename, size, bpa);
	std::cout << wt_ptr->rank_support << " " << wt_ptr->select_support << "\n";	
	exit(0);
}