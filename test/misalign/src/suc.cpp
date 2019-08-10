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
#include "include/util.hpp"
using namespace eopxd;

typedef uint8_t VAR;

int main ()
{
	clock_t start = clock(), stamp = clock();
	srand(112358);
	uint64_t allocate_size = 1000000000;
	misalign<4, VAR, 8> compact13(allocate_size);
	
	std::cout << "initialize: " << spent_time(stamp) << " seconds\n";
	stamp = clock();

	VAR *ans;
	ans = new VAR [allocate_size];
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
	std::cout << "mem_used: " << compact13.mem_used << "\n";
	star_line();
 	uint64_t times = 100000000;
	uint64_t twice = 2*times, fourth = times*4;

	for ( uint64_t i=0; i<times; ++i ) {
		int pos = rand()%compact13.max_pos;
		uint64_t val = rand()%compact13.max_val;
		ans[pos] = val;
		compact13.assign(pos, val, 1);
	}
	std::cout << "assign " << times << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();
	for ( uint64_t i=0; i<times; ++i ) {
		int pos = rand()%compact13.max_pos;
		VAR res = compact13.access(pos);
		assert(ans[pos] == res);	
	}
	std::cout << "access " << times << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();
	
	for ( uint64_t i=0; i<twice; ++i ) {
		int pos = rand()%compact13.max_pos;
		uint64_t val = rand()%compact13.max_val;
		ans[pos] = val;
		compact13.assign(pos, val, 1);
	}
	std::cout << "assign " << twice << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();
	
	for ( uint64_t i=0; i<twice; ++i ) {
		int pos = rand()%compact13.max_pos;
		VAR res = compact13.access(pos);
		assert(ans[pos] == res);	
	}
	std::cout << "access " << twice << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();
	for ( uint64_t i=0; i<fourth; ++i ) {
		int pos = rand()%compact13.max_pos;
		uint64_t val = rand()%compact13.max_val;
		ans[pos] = val;
		compact13.assign(pos, val, 1);
	}
	std::cout << "assign " << fourth << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();
	for ( uint64_t i=0; i<fourth; ++i ) {
		int pos = rand()%compact13.max_pos;
		VAR res = compact13.access(pos);
		assert(ans[pos] == res);	
	}
	std::cout << "access " << fourth << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();
	
	std::cout << "total: " << spent_time(start) << " seconds\n";	

	return (0);
}
