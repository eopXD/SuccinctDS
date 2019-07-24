// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file sdsl_wt.cpp
    \brief observe on performance of sdsl::wt
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/
// C99
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
// C++
#include <iostream>
// eopxd
#include "include/util.hpp"
// sdsl
#include "sdsl/wt_helper.hpp" // 2.1.1
#include "sdsl/wavelet_trees.hpp" // 2.1.1

using timer = std::chrono::high_resolution_clock;
using namespace std;
using namespace sdsl;

typedef uint64_t INT;

const INT filesize = 2000000000;
//const INT filesize = 100000;
char a, b; // calculate rank on 2 random characters
//std::string input; // reading the whole string to assert access correct
INT *rank1[2];
int cnt[2], now;

int main ()
{
	clock_t start = clock(), stamp = clock();
	srand(112358);
	a = rand()%256, b = rand()%256;

	char filename[25] = "test_data.big";
	int bpa = 1;

	rank1[0] = new INT [filesize+1];
	rank1[1] = new INT [filesize+1];
	rank1[0][0] = 0, rank1[1][0] = 0;
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
			rank1[0][now] = cnt[0];
			rank1[1][now++] = cnt[1];
		}	
	}
	close(fd);
	std::cout << "naive rank: " << eopxd::spent_time(stamp) << " seconds\n";
	stamp = clock();

	wt_huff<rrr_vector<63>> wt;	
	auto start1 = timer::now();
	memory_monitor::start();
	construct(wt, filename, 1); // obj, file, bytes per alphabet
	memory_monitor::stop();
	auto stop1 = timer::now();
	cout << "construct - 15\n";
	cout << "time: " <<chrono::duration_cast<chrono::milliseconds>
		(stop1-start1).count()/(double)1000 << "\n";
	cout << "space: " << memory_monitor::peak() << "\n";
	cout << "size: " << size_in_bytes(wt) << "\n";

	INT times = 100000000;
	INT twice = times*2, fourth = times*4, eighth = times*8;

	for ( INT i=0; i<times; ++i ) {
		INT pos = rand()%filesize;
		char c = wt[pos];
	}	
	cout << "access " << times << " times, " << eopxd::spent_time(stamp) << " seconds\n";
	stamp = clock();

	for ( INT i=0; i<twice; ++i ) {
		INT pos = rand()%filesize;
		char c = wt[pos];
	}	
	cout << "access " << twice << " times, " << eopxd::spent_time(stamp) << " seconds\n";
	stamp = clock();

	for ( INT i=0; i<fourth; ++i ) {
		INT pos = rand()%filesize;
		char c = wt[pos];
	}	
	cout << "access " << fourth << " times, " << eopxd::spent_time(stamp) << " seconds\n";
	stamp = clock();

	for ( INT i=0; i<eighth; ++i ) {
		INT pos = rand()%filesize;
		char c = wt[pos];
	}	
	cout << "access " << eighth << " times, " << eopxd::spent_time(stamp) << " seconds\n";
	stamp = clock();

	for ( INT i=0; i<times; ++i ) {
		INT pos = rand()%filesize;
		INT r0 = wt.rank(pos, a);
		INT r1 = wt.rank(pos, b);
		assert(r0 == rank1[0][pos]);
		assert(r1 == rank1[1][pos]);
	}
	cout << "rank " << times << " times, " << eopxd::spent_time(stamp) << " seconds\n";
	stamp = clock();

	for ( INT i=0; i<twice; ++i ) {
		INT pos = rand()%filesize;
		INT r0 = wt.rank(pos, a);
		INT r1 = wt.rank(pos, b);
		assert(r0 == rank1[0][pos]);
		assert(r1 == rank1[1][pos]);
	}
	cout << "rank " << twice << " times, " << eopxd::spent_time(stamp) << " seconds\n";
	stamp = clock();

	for ( INT i=0; i<fourth; ++i ) {
		INT pos = rand()%filesize;
		INT r0 = wt.rank(pos, a);
		INT r1 = wt.rank(pos, b);
		assert(r0 == rank1[0][pos]);
		assert(r1 == rank1[1][pos]);
	}
	cout << "rank " << fourth << " times, " << eopxd::spent_time(stamp) << " seconds\n";
	stamp = clock();

	for ( INT i=0; i<eighth; ++i ) {
		INT pos = rand()%filesize;
		INT r0 = wt.rank(pos, a);
		INT r1 = wt.rank(pos, b);
		assert(r0 == rank1[0][pos]);
		assert(r1 == rank1[1][pos]);
	}
	cout << "rank " << eighth << " times, " << eopxd::spent_time(stamp) << " seconds\n";
	stamp = clock();
	
/*
	wt_huff<rrr_vector<63>> wt63;
	auto start1 = timer::now();
	memory_monitor::start();
	construct(wt63, filename, 1);
	memory_monitor::stop();
	auto stop1 = timer::now();
	cout << "construct - 63\n";
	cout << "time: " <<chrono::duration_cast<chrono::milliseconds>
		(stop1-start1).count()/(double)1000 << "\n";
	cout << "space: " << memory_monitor::peak() << "\n";
	cout << "size: " << size_in_bytes(wt63) << "\n";
*/
	cout << "total " << eopxd::spent_time(start) << " seconds\n";
	return (0);
}
