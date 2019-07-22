// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file wavelet_node.hpp
    \brief wavelet_node.hpp contains node structure of the wavelet-tree, node
    structure may vary because of underlying bitvector implementation.
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/

#ifndef EOPXD_WAVELET_NODE_HPP
#define EOPXD_WAVELET_NODE_HPP

// C99
#include <stdint.h>
// C++
#include <string>

//! namespace by eopXD
namespace eopxd {

template<class bitvector_type>
struct wt_node {
	typedef uint64_t INT;
	typedef std::string STR;
	typedef bitvector_type BV;

	wt_node *mama, *left, *right;
	BV *bitmap;
	INT bitmapcnt; // for filling bit 1 by 1 ( check huff_wt->fill_bit() )
	INT len;
	STR leaf;

	// default constructor
	wt_node () {
		mama = left = right = bitmap = nullptr;
		len = bitmapcnt = 0;
		bitmap = new BV(len);
	}

	// leaf constructor (leaf nodes don't need bitmap)
	wt_node ( STR _leaf, INT _len ) {
		mama = left = right = nullptr;
		leaf = _leaf, len = _len, bitmapcnt = 0;
	}

	// internal node constructor
	wt_node ( wt_node *a, wt_node *b ) { 
		mama = nullptr;
		left = a, right = b;
		len = a->len+b->len, bitmapcnt = 0;
		bitmap = new BV(len);
	}

	void display () {
		std::cout << "[wt_node]\n";
		if ( leaf.size() != 0 ) {
			std::cout << "\talphabet: " << leaf << "\n";
		}
		else {
			std::cout << "\tnot a leaf node" << "\n";
		}
		std::cout << "length: " << len;
		std::cout << "\n**************************************************\n";

	}
}; // end struct wt_node

} // end namespace
#endif