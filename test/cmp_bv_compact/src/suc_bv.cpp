// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file suc_bv.cpp
    \brief suc_bv.cpp shows performance of eopxd::bv_lookup
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <ctime>
#include <cmath>
#include <iostream>
#include <vector>

#include "include/bv.hpp"

using namespace eopxd;

typedef long long LL;
const LL BV_SZ = 10000000000; // ~ 1.1G
double spent_time ( clock_t stamp ) {
	clock_t cost = clock() - stamp;
	return ((double)cost/CLOCKS_PER_SEC);
}
int main ()
{
	clock_t start = clock();
	clock_t stamp = clock();
	
	srand(112358); 
	bool *ans = new bool [BV_SZ];
	int *rank1 = new int [BV_SZ+1];
	rank1[0] = 0;
	bv_lookup bv(BV_SZ);
	std::cout << "create eopxd::bv_lookup: " << spent_time(stamp) << " seconds\n";
	std::cout << "bit vector size: " << (double)BV_SZ/(1LL<<33) << " G\n";
	stamp = clock();

	double sparsity = 0.01;
	std::cout << "sparsity: " << sparsity << "\n";
	for ( LL i=0; i<BV_SZ; ++i ) {
		ans[i] = bv.bitvec[i] = 0;
	}
	std::cout << "init all 0: " << spent_time(stamp) << " seconds\n";
	stamp = clock();
	LL ones = BV_SZ*sparsity;
	while ( ones ) {
		LL pos = rand()%BV_SZ;
		if ( bv.bitvec[pos] == 0 ) {
			ans[pos] = bv.bitvec[pos] = 1;
			ones--;
		}
	}
	std::cout << "sparsity: " << spent_time(stamp) << " seconds\n";
	stamp = clock();
	
	LL cnt1 = 0;
	for ( LL i=0; i<BV_SZ; ++i ) {
		cnt1 += ans[i];
		rank1[i+1] = cnt1;
	}
	std::cout << "sum: " << spent_time(stamp) << " seconds\n";
	stamp = clock();

	bv.support_rank();
	std::cout << "support_rank(): " << spent_time(stamp) << " seconds\n";
	stamp = clock();
	
	ones = BV_SZ*sparsity;
	for ( LL i=0; i<ones; ++i ) {
		LL pos = rand()%BV_SZ;
		bool b = bv.access(pos);
		assert(ans[pos] == b);
	}
	std::cout << "access(): " << ones << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();
	
	LL twice = ones*2, fourth = ones*4, eighth = ones*8;
	
	for ( LL i=0; i<twice; ++i ) {
		LL pos = rand()%BV_SZ;
		bool b = bv.access(pos);
		assert(ans[pos] == b);
	}
	std::cout << "access(): " << twice << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();
	
	for ( LL i=0; i<fourth; ++i ) {
		LL pos = rand()%BV_SZ;
		bool b = bv.access(pos);
		assert(ans[pos] == b);
	}
	std::cout << "access(): " << fourth << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();
	
	for ( LL i=0; i<eighth; ++i ) {
		LL pos = rand()%BV_SZ;
		bool b = bv.access(pos);
		assert(ans[pos] == b);
	}
	std::cout << "access(): " << eighth << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();
	
	/*for ( int i=0; i<30; ++i ) {
		std::cout << ans[i]; 
	} puts("");
	for ( int i=0; i<31; ++i ) {
		std::cout << rank1[i] << " " << bv.rank(i, 1) << "\n";
	}*/
	
	for ( LL i=0; i<ones; ++i ) {
		LL pos = rand()%BV_SZ;
		LL r = bv.rank(pos, 1);
		assert(rank1[pos] == r);
	}
	std::cout << "rank(): " << ones << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();

	for ( LL i=0; i<twice; ++i ) {
		LL pos = rand()%BV_SZ;
		LL r = bv.rank(pos, 1);
		assert(rank1[pos] == r);
	}
	std::cout << "rank(): " << twice << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();

	for ( LL i=0; i<fourth; ++i ) {
		LL pos = rand()%BV_SZ;
		LL r = bv.rank(pos, 1);
		assert(rank1[pos] == r);
	}
	std::cout << "rank(): " << fourth << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();

	for ( LL i=0; i<eighth; ++i ) {
		LL pos = rand()%BV_SZ;
		LL r = bv.rank(pos, 1);
		assert(rank1[pos] == r);
	}
	std::cout << "rank(): " << eighth << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();
	
	std::cout << "total time: " << spent_time(start) << " seconds\n";
	return (0);
}
