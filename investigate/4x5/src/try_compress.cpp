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

int main ()
{
	char filename[100];
	int namelen;
	int bpa = 1, dummy = 1;
	wt<wt_huff<bv_lookup>, bv_lookup> *wt_ptr;
	// decode0
	for ( int i=0; i<30; ++i ) {
		sprintf(filename, "/tmp2/b04705001/4x5_result_decoded1/part%03d.dat", i);
		puts(filename);
		namelen = strlen(filename);
		wt_ptr = new wt<wt_huff<bv_lookup>, bv_lookup>(filename, namelen, bpa, dummy);

		std::cout << "mem_used: " << wt_ptr->mem_used << "\n";
		delete wt_ptr;
		star_line();
		//break;
	}
	// decode1
	/*for ( int i=0; i<30; ++i ) {
		sprintf(filename, "/tmp2/b04705001/4x5_result_decoded1/part%03d.dat", i);
		



		break;
	}*/
	return (0);

}
