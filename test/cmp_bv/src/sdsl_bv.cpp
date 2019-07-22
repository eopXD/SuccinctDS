// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file sdsl_bv.cpp
    \brief sdsl_bv.cpp shows performance of sdsl::rrr_vector
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <ctime>
#include <cmath>
#include <iostream>

#include "sdsl/bit_vectors.hpp" // 2.1.1

typedef long long LL;
LL BV_SZ = 10000000000; // 1G = 8589934592
double spent_time ( clock_t stamp ) {
	clock_t cost = clock() - stamp;
	return ((double)cost/CLOCKS_PER_SEC);
}
int main ()
{
	clock_t start = clock();
	srand(112358);
	
	bool *ans = new bool [BV_SZ];
	int *rank1 = new int [BV_SZ+1];
	rank1[0] = 0;
	
	std::cout << "bit vector size: " << (double)BV_SZ/(1LL<<33) << " G\n";
	double sparsity = 0.01;
	std::cout << "sparsity: " << sparsity << "\n";
	clock_t stamp = clock();
	sdsl::bit_vector b = sdsl::bit_vector(BV_SZ, 0);
	std::cout << "create bit_vector(BV_SZ, 0): " << spent_time(stamp) << " seconds\n";
	stamp = clock();
	
	for ( LL i=0; i<BV_SZ; ++i ) {
		ans[i] = 0;
	}
	std::cout << "init all 0: " << spent_time(stamp) << " seconds\n";
	stamp = clock();

	LL ones = BV_SZ*sparsity;
	while ( ones ) {
		LL pos = rand()%BV_SZ;
		if ( b[pos] == 0 ) {
			ans[pos] = b[pos] = 1;
			ones--;
		}
	}
	std::cout << "sparsity cost: " << spent_time(stamp) << " seconds\n";
	stamp = clock();
	LL cnt1 = 0;
	for ( LL i=0; i<BV_SZ; ++i ) {
		cnt1 += ans[i];
		rank1[i+1] = cnt1;
	}
	std::cout << "sum: " << spent_time(stamp) << " seconds\n";
	stamp = clock();

	sdsl::rrr_vector<63> rrr(b);
	std::cout << "sdsl::rrr_vector<63>: " << spent_time(stamp) << " seconds\n";
	stamp = clock();
	//sdsl::rrr_vector<15> rrr(b);
	//std::cout << "sdsl::rrr_vector<15>: " << spent_time(stamp) << " seconds\n";
	//stamp = clock();

	sdsl::rrr_vector<63>::rank_1_type rrrank(&rrr);
	std::cout << "<63>::rank_1_type: " << spent_time(stamp) << " seconds\n";
	stamp = clock();

	//sdsl::rrr_vector<15>::rank_1_type rrrank(&rrr);
	//std::cout << "<15>::rank_1_type: " << spent_time(stamp) << " seconds\n";
	//stamp = clock();

	/*for ( int i=0; i<30; ++i ) {
		std::cout << ans[i];
	} puts("");

	for ( int i=1; i<30; ++i ) {
		std::cout << rank1[i] << " " << rank15(i) << " " << rank63(i) << "\n";
	}*/

	ones = BV_SZ*sparsity;
	LL twice = ones*2, fourth = ones*4, eighth = ones*8;

	for ( LL i=0; i<ones; ++i ) {
		LL pos = rand()%BV_SZ;
		int a = rrr[pos];
		assert(ans[pos] == a);
	}
	std::cout << "access() " << ones << " times, "  << spent_time(stamp) << " seconds\n";
	stamp = clock();

	for ( LL i=0; i<twice; ++i ) {
		LL pos = rand()%BV_SZ;
		int a = rrr[pos];
		assert(ans[pos] == a);
	}
	std::cout << "access() " << twice << " times, "  << spent_time(stamp) << " seconds\n";
	stamp = clock();

	for ( LL i=0; i<fourth; ++i ) {
		LL pos = rand()%BV_SZ;
		int a = rrr[pos];
		assert(ans[pos] == a);
	}
	std::cout << "access() " << fourth << " times, "  << spent_time(stamp) << " seconds\n";
	stamp = clock();

	for ( LL i=0; i<eighth; ++i ) {
		LL pos = rand()%BV_SZ;
		int a = rrr[pos];
		assert(ans[pos] == a);
	}
	std::cout << "access() " << eighth << " times, "  << spent_time(stamp) << " seconds\n";
	stamp = clock();

	for ( LL i=0; i<ones; ++i ) {
		LL pos = rand()%BV_SZ;
		LL r = rrrank(pos);
		assert(r == rank1[pos]);
	}
	std::cout << "rank(): " << ones << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();

	for ( LL i=0; i<twice; ++i ) {
		LL pos = rand()%BV_SZ;
		LL r = rrrank(pos);
		assert(r == rank1[pos]);
	}
	std::cout << "rank(): " << twice << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();

	for ( LL i=0; i<fourth; ++i ) {
		LL pos = rand()%BV_SZ;
		LL r = rrrank(pos);
		assert(r == rank1[pos]);
	}
	std::cout << "rank(): " << fourth << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();
	
	for ( LL i=0; i<eighth; ++i ) {
		LL pos = rand()%BV_SZ;
		LL r = rrrank(pos);
		assert(r == rank1[pos]);
	}
	std::cout << "rank(): " << eighth << " times, " << spent_time(stamp) << " seconds\n";
	stamp = clock();

	std::cout << "total time: " << spent_time(start) << " seconds\n";
	return (0);
}
