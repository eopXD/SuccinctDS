// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file bv_lookup.cpp
    \brief bv_lookup.cpp tests functionality of the RRR bit vector
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/

#include <cstdio>
#include <cassert>
#include <random>
#include <iostream>

#include "include/bv.hpp"

using namespace eopxd;
int naive_rank ( bool *ans, int pos, int c ) {
    int res = 0;
    for ( int i=0; i<pos; ++i ) {
        if ( ans[i]==c ) {
            ++res;
        }		
    }
    return (res);
}

int main ()
{
	int n = 100000, m = 50000;
	bv_lookup *bv_ptr = new bv_lookup(n);
	bv_lookup bv_var(n);
	bool *ans = new bool [n];
	for ( int i=0; i<n; ++i ) {
		bv_ptr->bitvec[i] = bv_var.bitvec[i] = ans[i] = 0;
	}
	for ( int i=0; i<m; ++i ) {
		int x = rand()%n;
		bv_ptr->bitvec[x] = bv_var.bitvec[x] = ans[x] = 1;
	}
	//for ( int i=0; i<n; ++ i ) printf("%d", ans[i]); puts("");		
	for ( int i=0; i<n; ++i ) {
		assert(bv_ptr->bitvec[i] == ans[i]);
		assert(bv_var.bitvec[i] == ans[i]);
	}

	bv_ptr->support_rank();
	bv_var.support_rank();
	//bv_ptr->display_rank();
	
	//for ( int i=0; i<15; ++i ) {
	//	printf("%2d: %d, %d\n", i, vec->access(i), ans[i]);
	//}
	for ( int i=0; i<n; ++i ) {
		assert(bv_ptr->access(i) == ans[i]);
		assert(bv_ptr->rank(i, 0) == naive_rank(ans, i, 0));
		assert(bv_ptr->rank(i, 1) == naive_rank(ans, i, 1));

		assert(bv_var.access(i) == ans[i]);
		assert(bv_var.rank(i, 0) == naive_rank(ans, i, 0));
		assert(bv_var.rank(i, 1) == naive_rank(ans, i, 1));
	}
	exit(0);
}
