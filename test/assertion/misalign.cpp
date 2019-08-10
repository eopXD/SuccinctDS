// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file misalign.cpp
    \brief assertions of access/assign (and TODO: mem_used)
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/
// C99
#include <stdint.h>
#include <cassert>
// C++
#include <iostream>
// eopxd
#include "include/misalign.hpp"
using namespace eopxd;

int main ()
{
	srand(112358);
	uint64_t allocate_size = 10000;
	misalign<13, uint16_t, 16> compact13(allocate_size);
	uint16_t *ans;
	ans = new uint16_t [allocate_size];
/*	while ( 1 ) {
		int op;
		std::cout << "enter op (0: access, 1 assign): ";
		std::cin >> op;
		if ( op == 0 ) {
			std::cout << "enter position to access: ";
			int pos; 
			std::cin >> pos;
			uint16_t val = compact13.access(pos);
			std::cout << "access: " << (uint64_t)val << "\n";
		}
		else {
			std::cout << "enter position and value to assign: ";
			int pos, val;
			std::cin >> pos >> val;
			compact13.assign(pos, val, 1);
		}
	}*/
	for ( int i=0; i<20000; ++i ) {
		int pos = rand()%compact13.max_pos;
		int val = rand()%compact13.max_val;
		ans[pos] = val;
		compact13.assign(pos, val, 1);
		uint16_t res = compact13.access(pos);
		//std::cout << i << " " << pos << ": " << ans[pos] << " " << res << "\n";
		assert(ans[pos] == res);
//		break;
	}
	for ( int i=0; i<20000; ++i ) {
		int pos = rand()%compact13.max_pos;
		uint16_t res = compact13.access(pos);
		assert(ans[pos] == res);	
	}
	std::cout << "pass assertion on assign / access\n";
	return (0);
}
