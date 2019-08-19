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
	typedef uint64_t INT;
	typedef uint8_t UINT8;
	typedef misalign<1, UINT8, 8> BITVEC;

	INT len;
	//bool *bitvec;
	BITVEC *compact_bitvec; 

	INT mem_used;

	void account_mem () {
		mem_used += sizeof(INT)*2;
		mem_used += sizeof(BITVEC*); 
		if ( compact_bitvec ) {
			mem_used += compact_bitvec->mem_used;
		}
	}

	// default constructor (shall not be called)
	bv_naive () {
		mem_used = 0;

	//	bitvec = nullptr;
		compact_bitvec = nullptr;

		account_mem();
	}

	// main constructor
	bv_naive ( INT _len ) {
		mem_used = 0;

		len = _len;
		//bitvec = new bool [len];
		compact_bitvec = new BITVEC(len);

		//mem_used += sizeof(bool)*len;
		
		//std::cout << "len: " << len << "\n";
		//std::cout << "bool_bitvec: " << sizeof(bool)*len << "\n";
		//std::cout << "compact_bitvec: " << compact_bitvec->mem_used << "\n";

		account_mem();
	}
	
	// destructor
	~bv_naive () {
		//if ( bitvec ) {
		//	delete[] bitvec;
		//}
		if ( compact_bitvec ) {
			delete compact_bitvec;
		}
		//bitvec = nullptr;
		compact_bitvec = nullptr;
	}
	void assign ( INT p, bool v ) {
		compact_bitvec->assign(p, v, 1);
		//bitvec[p] = v;
		//assert(bitvec[p] == compact_bitvec->access(p));
	}
	bool access ( INT p ) { 
		//assert(compact_bitvec->access(p) == bitvec[p]);
		//return (bitvec[p]); 
		return (compact_bitvec->access(p));
	}

	INT rank ( INT p, bool c ) {
		INT o = 0;
		for ( INT i=0; i<p; ++i ) {
			if ( compact_bitvec->access(i) == c ) {
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
		for ( INT i=0; i<len; ++i ) {
			std::cout << compact_bitvec->access(i);
		}
		std::cout << "\n";
	}
}; // end struct bit_naive

} // end namespace
#endif
