// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file stream.cpp
    \brief test read data in stream mode
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/

#include <stdint.h>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <iostream>

#include "include/wt.hpp"

using namespace eopxd;
typedef uint64_t INT;
int main ()
{

	char filename[100] = "stream.cpp";
	int name_len = strlen(filename);
	int bpa = 1, dummy = 1;
	wt<wt_huff<bv_lookup>, bv_lookup> *wt_ptr = 
		new wt<wt_huff<bv_lookup>, bv_lookup>(filename, name_len, bpa, dummy);
	
	wt_ptr->support_rank();
	
	char *buf = new char [100000];
	int fd = open(filename, O_RDONLY);
	if ( fd < 0 ) {
		std::cout << "fail to open file\n";
		exit(1);
	}
	INT sz;
	sz = read(fd, buf, 100000);
	
	for ( INT i=0; i<sz; ++i ) {
		char *a0 = wt_ptr->access(i);
		assert(a0[0] == buf[i]);
	}
	exit(0);
}
