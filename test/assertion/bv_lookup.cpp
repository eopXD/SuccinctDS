// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file bv_lookup.cpp
    \brief bv_lookup.cpp tests functionality of the RRR bit vector
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/

#include <cstdio>
#include <stdint.h>
#include <cassert>
#include <random>
#include <iostream>

#include "include/bv.hpp"

using namespace eopxd;
typedef uint64_t INT;
typedef bv_lookup BV;
int main ()
{
	int n = 100000000, m = 500;
	
	BV *bv_ptr = new BV(n);
	bool *ans = new bool [n];
	INT *naive_rank = new INT [n+1];
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

	std::cout << "before support_rank()\n";
	std::cout << "bv_ptr memory: " << bv_ptr->mem_used << "\n";
	bv_ptr->support_rank();
	std::cout << "after support_rank()\n";
	std::cout << "bv_ptr memory: " << bv_ptr->mem_used << "\n";
	
	for ( int i=0; i<n; ++i ) {
		assert(bv_ptr->access(i) == ans[i]);
		assert(bv_ptr->rank(i, 0) == (i-naive_rank[i]));
		assert(bv_ptr->rank(i, 1) == naive_rank[i]);
	}
	std::cout << "pass assertion (access/rank)\n";
	exit(0);
}
