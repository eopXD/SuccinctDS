// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file bitvector_naive.hpp
	\brief bitvector_naive.hpp contains naive implementation of bitvector.
	BinaryRank: 	O(N)
	BinarySelect: 	O(N)
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/

#ifndef EOPXD_BITVECTOR_NAIVE_HPP
#define EOPXD_BITVECTOR_NAIVE_HPP

// C99
#include <stdint.h>
// C++
#include <iostream>

//! namespace by eopXD
namespace eopxd {

struct bv_naive {
	typedef uint64_t INT;

	INT len;
	bool *bitvec;

	// default constructor
	bv_naive () {
		bitvec = nullptr;
	}

	// main constructor
	bv_naive ( INT _len ) {
		len = _len;
		bitvec = new bool [len];
	}
	
	// destructor
	~bv_naive () {
		if ( bitvec ) delete[] bitvec;
		bitvec = nullptr;
	}

	bool access ( INT p ) { return (bitvec[p]); }

	INT rank ( INT p, bool c ) {
		INT o = 0;
		for ( INT i=0; i<p; ++i ) {
			if ( bitvec[i] == c ) {
				++o;
			}
		}
		return (o);
	}
	INT select ( INT o, bool c ) { // 0th occurence is the first occurence
		if ( o < 0 ) {
			return (0);
		}
		INT cnt = 0;
		for ( INT p=0; p<len; ++p ) {
			cnt += (bitvec[p] == c);
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
		for ( INT i=0; i<len; ++i ) {
			std::cout << bitvec[i];
		}
		std::cout << "\n";
	}
}; // end struct bit_naive

} // end namespace
#endif
