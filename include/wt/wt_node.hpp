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

	int bpa;
	wt_node *mama;
	wt_node *child[2]; // [0] = left, [1] = right (to avoid conditional branch)
	BV *bitmap;
	INT bitmapcnt; // for filling bit 1 by 1 ( check huff_wt->fill_bit() )
	INT len;
	int blk_hash;
	unsigned char *blk;

	INT mem_used;

	void account_mem () { // 72 B 
		mem_used += sizeof(wt_node*)*3; // 24 B
		mem_used += sizeof(BV*); // 8 B
		mem_used += sizeof(unsigned char*); // 8 B
		mem_used += sizeof(INT)*3; // 24B
		mem_used += sizeof(int)*2; // 8 B
	}

	void decode ( int _blk_hash ) { // "bpa+1" Byte for alphabet of leaf node
		blk = new unsigned char [bpa+1];
		blk[bpa] = '\0';
		for ( int i=bpa-1; i>=0; i-- ) {
			blk[i] = _blk_hash%256;
			_blk_hash >>= 8;
		}
	
		mem_used += sizeof(unsigned char)*(bpa+1);
	}
	// default constructor (usually not called)
	wt_node () {
		mem_used = 0;

		mama = child[0] = child[1] = nullptr;
		bitmap = nullptr;
		blk = nullptr;
		len = bitmapcnt = 0;
		bitmap = new BV(len);

		mem_used += bitmap->mem_used;
		account_mem();
	}

	// leaf constructor (leaf nodes don't need bitmap)
	wt_node ( int _blk_hash, int _bpa, INT _len ) {
		mem_used = 0;

		mama = child[0] = child[1] = nullptr;
		len = _len, bpa = _bpa;
		blk_hash = _blk_hash;
		decode(_blk_hash);

		account_mem();
	}

	// internal node constructor
	wt_node ( wt_node *a, wt_node *b ) { 
		mem_used = 0;

		mama = nullptr;
		blk = nullptr;
		child[0] = a, child[1] = b;
		len = a->len+b->len, bitmapcnt = 0;
		bitmap = new BV(len);

		mem_used += bitmap->mem_used;
		account_mem();
	}

	void support_rank () {
		mem_used -= bitmap->mem_used;
		bitmap->support_rank();
		mem_used += bitmap->mem_used;
	}

	void display () {
		std::cout << "[wt_node]\n";
		if ( blk != nullptr ) {
			std::cout << "leaf node: " << blk << "\n";
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
