// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file bv_lookup.cpp
    \brief bv_lookup.cpp tests functionality of the RRR bit vector
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/

#include <iostream>
#include <stdint.h>
#include <cassert>
#include <random>

#include "include/bv.hpp"

using namespace eopxd;
typedef uint64_t UINT64;
typedef uint8_t UINT8;
typedef bv_naive BV;
int main ()
{
	int n = 10000, m = 500;
	BV *bv_ptr = new BV(n);
	bool *ans = new bool [n];
	UINT64 *naive_rank = new UINT64 [n+1];
	for ( int i=0; i<n; ++i ) {
		bv_ptr->assign(i, 0);
		ans[i] = 0;
	}
	for ( int i=0; i<m; ++i ) {
		int x = rand()%n;
		bv_ptr->assign(x, 1);
		ans[x] = 1;
	}
	//for ( int i=0; i<n; ++ i ) printf("%d", ans[i]); puts("");
	naive_rank[0] = 0;
	for ( int i=0; i<n; ++i ) {
		naive_rank[i+1] = naive_rank[i]+ans[i];
		assert(bv_ptr->access(i) == ans[i]);
	}

	bv_ptr->support_rank();
	//bv_ptr->display_rank();
	
	//for ( int i=0; i<15; ++i ) {
	//	printf("%2d: %d, %d\n", i, vec->access(i), ans[i]);
	//}
	for ( int i=0; i<n; ++i ) {
		assert(bv_ptr->access(i) == ans[i]);
		assert(bv_ptr->rank(i, 0) == (i-naive_rank[i]));
		assert(bv_ptr->rank(i, 1) == naive_rank[i]);
	}
	std::cout << "mem_used: " << bv_ptr->mem_used << "\n";
	std::cout << "complete assertion\n";
	exit(0);
}
