// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file suc_wt.cpp
    \brief observe on performance of wt<wt_huff<bv_lookup>, bv_lookup>
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <stdint.h>
#include <cassert>
#include <iostream>
#include <string>

#include "include/wt.hpp"
using namespace eopxd;

typedef uint64_t INT;
const INT filesize = 2000000000;
char a, b; // calculate rank on 2 random characters
//std::string input; // reading the whole string to assert access correct
INT *rank[2];
int cnt[2], now;
int main ()
{
	clock_t start = clock(), stamp = clock();

	srand(112358);
	a = rand()%256, b = rand()%256;

	char filename[25] = "test_data.big";
	int bpa = 1;
	wt<wt_huff<bv_lookup>, bv_lookup> *wt_ptr = 
		new wt<wt_huff<bv_lookup>, bv_lookup>(filename, bpa, 1); 
	std::cout << wt_ptr->rank_support << "\n";
	
	std::cout << "initialize: " << spent_time(stamp) << " seconds\n";
	stamp = clock();
	
	rank[0] = new INT [filesize+1];
	rank[1] = new INT [filesize+1];
	rank[0][0] = 0, rank[1][0] = 0;
	cnt[0] = cnt[1] = 0;
	now = 1;
		
	int fd = open(filename, O_RDONLY);
	if ( fd < 0 ) {
		std::cout << "open file fail\n";
		return (0);
	}
	INT buf_size = bpa*65536, sz;
	char *buf = new char [buf_size];
	while ( (sz=read(fd, buf, buf_size)) > 0 ) {
		for ( INT i=0; i<sz; ++i ) {
			if ( buf[i] == a ) cnt[0]++;
			if ( buf[i] == b ) cnt[1]++;
			rank[0][now] = cnt[0];
			rank[1][now++] = cnt[1];
		}	
	}
	close(fd);
	std::cout << "now: " << now << "\n";
	
	std::cout << "naive rank: " << spent_time(stamp) << " seconds\n";
	stamp = clock();

	wt_ptr->support_rank();
	std::cout << "support rank: " << spent_time(stamp) << " seconds\n";
	stamp = clock();


	std::string s, t;
	s += a, t+= b;
	INT times = 100000000;
	INT twice = times*2, fourth = times*4, eighth = times*8;
	for ( INT i=0; i<times; ++i ) {
		INT pos = rand()%filesize;
		wt_ptr->access(pos);
	}
	std::cout << "access " << times << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();

	for ( INT i=0; i<twice; ++i ) {
		INT pos = rand()%filesize;
		wt_ptr->access(pos);
	}
	std::cout << "access " << twice << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();

	for ( INT i=0; i<fourth; ++i ) {
		INT pos = rand()%filesize;
		wt_ptr->access(pos);
	}
	std::cout << "access " << fourth << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();

	for ( INT i=0; i<eighth; ++i ) {
		INT pos = rand()%filesize;
		wt_ptr->access(pos);
	}
	std::cout << "access " << eighth << " times, " << spent_time(stamp) << " seconds\n";
	
	stamp = clock();
	for ( INT i=0; i<times; ++i ) {
		INT pos = rand()%filesize;
		INT r0 = wt_ptr->rank(s, pos);
		INT r1 = wt_ptr->rank(t, pos);

		assert(r0 == rank[0][pos]);
		assert(r1 == rank[1][pos]);
	}
	std::cout << "rank " << times << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();

	for ( INT i=0; i<twice; ++i ) {
		INT pos = rand()%filesize;
		INT r0 = wt_ptr->rank(s, pos);
		INT r1 = wt_ptr->rank(t, pos);

		assert(r0 == rank[0][pos]);
		assert(r1 == rank[1][pos]);
	}
	std::cout << "rank " << twice << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();
	for ( INT i=0; i<fourth; ++i ) {
		INT pos = rand()%filesize;
		INT r0 = wt_ptr->rank(s, pos);
		INT r1 = wt_ptr->rank(t, pos);

		assert(r0 == rank[0][pos]);
		assert(r1 == rank[1][pos]);
	}
	std::cout << "rank " << fourth << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();
	for ( INT i=0; i<eighth; ++i ) {
		INT pos = rand()%filesize;
		INT r0 = wt_ptr->rank(s, pos);
		INT r1 = wt_ptr->rank(t, pos);

		assert(r0 == rank[0][pos]);
		assert(r1 == rank[1][pos]);
	}
	std::cout << "rank " << eighth << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();

	std::cout << "total: " << spent_time(start) << " seconds\n";
	return (0);
}

