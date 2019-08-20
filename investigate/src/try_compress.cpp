// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file try_compress.cpp
    \brief try compression on real data (take 4x5). Investigate on frequency
	to caclulate theoretical H0, then cimpare to the real results.
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/

// C99
#include <stdint.h>
// C++
#include <cstdio>
#include <cstring>
#include <cassert>
#include <iostream>
// eopxd
#include "include/wt.hpp"

using namespace eopxd;
typedef uint64_t UINT64;
typedef wt<wt_huff<bv_lookup>, bv_lookup> WAVELET;
int main ()
{
	char filename[100];
	int namelen;
	int bpa = 1, dummy = 1;
	UINT64 pre;
	WAVELET *wt_ptr;
	while ( 1 ) {
		std::cout << "input filename (absolute path): ";
		std::cin >> filename;
		namelen = strlen(filename);
		UINT64 file_size = get_filesize(filename);
		std::cout << "\n\nfile: " << filename << "\n";
		std::cout << "filesize: " << file_size << " " << (double)file_size/(1LL<<30) << "G\n";
	
		wt_ptr = new WAVELET(filename, namelen, bpa, dummy);
		
		std::cout << "\n\nwavelet tree:\n";
		std::cout << "mem_used: :" << wt_ptr->mem_used << " " << 
			(double)wt_ptr->mem_used/(1LL<<30) << "G\n";
		std::cout << "ratio: " << (double)wt_ptr->mem_used/file_size << "\n"; 
		pre = wt_ptr->mem_used;
		wt_ptr->support_rank();
		
		std::cout << "\nafter support_rank():\n";
		std::cout << "mem_used: :" << wt_ptr->mem_used << " " << 
			(double)wt_ptr->mem_used/(1LL<<30) << "G\n";
		std::cout << "ratio: " << (double)wt_ptr->mem_used/file_size << "\n"; 

		std::cout << "\nafter rank: " << (double)wt_ptr->mem_used/pre << "\n"; 
		delete wt_ptr;
	}
	return (0);

}
