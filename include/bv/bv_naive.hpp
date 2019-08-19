// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file bitvector_naive.hpp
	\brief bitvector_naive.hpp contains naive implementation of bitvector.
	BinaryRank: 	O(N)
	BinarySelect: 	O(N)
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/

// try misalign.hpp on naive bitvector

#ifndef EOPXD_BITVECTOR_NAIVE_HPP
#define EOPXD_BITVECTOR_NAIVE_HPP

// C99
#include <stdint.h>
// C++
#include <iostream>
#include <cassert>

//! namespace by eopXD
namespace eopxd {

struct bv_naive {
	typedef uint64_t UINT64;
	typedef uint8_t UINT8;
	typedef misalign<1, UINT8, 8> BITVEC;

	UINT64 len;
	BITVEC *compact_bitvec; 

	UINT64 mem_used;

	void account_mem () {
		mem_used += sizeof(UINT64)*2;
		mem_used += sizeof(BITVEC*); 
		if ( compact_bitvec ) {
			mem_used += compact_bitvec->mem_used;
		}
	}

	// default constructor (shall not be called)
	bv_naive () {
		std::cout << "usage: bv_naive(allocated_length)\n";
		exit(1);
	}

	// main constructor
	bv_naive ( UINT64 _len ) {
		mem_used = 0;
		len = _len;
		compact_bitvec = new BITVEC(len);
		account_mem();
	}
	
	// destructor
	~bv_naive () {
		if ( compact_bitvec ) {
			delete compact_bitvec;
		}
		compact_bitvec = nullptr;
	}
	void assign ( UINT64 p, bool v ) {
		compact_bitvec->assign(p, v, 1);
	}
	bool access ( UINT64 p ) { 
		return (compact_bitvec->access(p));
	}

	UINT64 rank ( UINT64 p, bool c ) {
		UINT64 o = 0;
		for ( UINT64 i=0; i<p; ++i ) {
			if ( compact_bitvec->access(i) == c ) {
				++o;
			}
		}
		return (o);
	}
	UINT64 select ( UINT64 o, bool c ) { // 0th occurence is the first occurence
		if ( o < 0 ) {
			return (0);
		}
		UINT64 cnt = 0;
		for ( UINT64 p=0; p<len; ++p ) {
			cnt += (compact_bitvec->access(p) == c);
			if ( (cnt-1) == o ) {
				return (p);
			}
		}
		return (0);
	}

	void support_rank () {}
	void support_select () {}

	void display_bitmap () {
		std::cout << "Length: " << len << "\n";
		std::cout << "Bitmap: ";
		for ( UINT64 i=0; i<len; ++i ) {
			std::cout << compact_bitvec->access(i);
		}
		std::cout << "\n";
	}
}; // end struct bit_naive

} // end namespace
#endif
