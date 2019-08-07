// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file wavelet.hpp
    \brief wavelet.hpp contains wavelet tree rank/select operations.
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/


//*******************************	
//** DONE: SUPPORT STREAM MODE **
//*******************************
/*
stream mode: spend one time to read through it to investigate
empirical frequency of data. construct tree shape, then fill the
bitstrings in stream mode.

this feature is needed if the data cannot fit into memory.
*/


//*******************************	
//** TODO: SIZE CALCULATION    **
//*******************************
//calculation of the data structure is neccessary

//*******************************	
//** TODO: PEAK MEMORTY USAGE  **
//*******************************
// peak of memory usage is also needed to 

#ifndef EOPXD_WAVELET_HPP
#define EOPXD_WAVELET_HPP

// C99
#include <stdint.h>
// C++
#include <iostream>
// eopXD
#include "util.hpp"
#include "bv.hpp"
#include "wt/wt_node.hpp"
#include "wt/wt_huff.hpp"

//! namespace by eopXD
namespace eopxd {

template<class wt_type, class bitvector_type>
struct wt {
	typedef uint64_t INT;

	typedef wt_type WT_T;
	typedef bitvector_type BV;
	typedef wt_node<BV> NODE;

	WT_T *tree; // tree structure (alphabet set, huffcode, memory used by tree)
	NODE *root; // root node of tree
	
	bool rank_support, select_support;

	INT mem_used; // memory allocated

	void account_mem () {
		mem_used += tree->mem_used;
		
		std::cout << "tree_structure: " << tree->mem_used << "\n";

		// 26 Bytes
		mem_used += sizeof(WT_T*); // 8 Byte
		mem_used += sizeof(NODE*); // 8 Byte 
		mem_used += sizeof(bool)*2; // 2 Byte
		mem_used += sizeof(INT); // 8 Byte
	}

	wt () {
		std::cout << "[wt] give me data lah ~~~\n";
		exit(1);
	}
	// batch mode constructor
	wt ( unsigned char *data, INT data_len, INT bpa ) { // bpa = bytes per alphabet
		std::cout << "[wt] batch mode\n";
		tree = new WT_T(data, data_len, bpa);
		root = tree->root;
		rank_support = select_support = false;
		mem_used = 0;
		account_mem();
	}
	// stream mode constructor (last argument is dummy variable)
	wt ( char filename[], int name_len, INT bpa, bool dummy ) {
		std::cout << "[wt] stream mode\n";
		tree = new WT_T(filename, name_len, bpa, dummy);
		root = tree->root;
		rank_support = select_support = false;
		mem_used = 0;
		account_mem();
	}

/* Check if the node is a leaf node (only leaf nodes)*/
	bool isLeaf ( NODE *now ) { 
		return (now->child[0] == nullptr and now->child[1] == nullptr); 
	}

/* Access: access the alphabet on position p */
	void _access ( NODE *now, INT p, unsigned char **res ) {
		if ( isLeaf(now) ) {
			*(res) = now->blk;
			return ;
		}
		bool charbit = now->bitmap->access(p);
		INT next_p = now->bitmap->rank(p, charbit);
		_access(now->child[charbit], next_p, res);
	}
	void access ( INT p, unsigned char **res ) {
	//	if ( !rank_support ) {
	//		std::cout << "wavelet tree access op also needs rank\n";
	//		std::cout << "call support_rank() for access ops\n";
	//		return (0);
	//	}
		_access(root, p, res);
	}
/* Rank: count the occurence of alphabet from [0,p) */
	void _rank ( NODE *now, int blk_hash, INT p, int lv, INT *res ) {
		if ( isLeaf(now) ) {
			*(res) = p;
			return ;
		}
		bool charbit = tree->huffcode[blk_hash][lv];
		INT next_p = now->bitmap->rank(p, charbit);
		_rank(now->child[charbit], blk_hash, next_p, lv+1, res);
	}
	void rank ( int blk_hash, INT p, INT *res ) {
	//	if ( !rank_support ) {
	//		std::cout << "call support_rank() for rank ops\n";
	//		return (0);
	//	}
	//	if ( tree->alphabet[blk_hash] == -1 ) {
	//		std::cout << "this alphabet does not belong to exist\n";
	//		return (0);
	//	}
		_rank(root, blk_hash, p, 0, res);
	}

/* Select: find the n-th occurence of alphabet */
	NODE* getLeaf ( NODE *now, int blk_hash, int lv ) {
		if ( lv == tree->huff_len[blk_hash] ) {
			return (now);
		}
		return (getLeaf(now->child[tree->huffcode[blk_hash][lv]], blk_hash, lv+1));
	}
	void _selectrev ( NODE *now, bool charbit, INT o, INT *res ) {
		INT p = now->bitmap->select(o, charbit);
		if ( now->mama == nullptr ) { // reach root node
			*(res) = p;
			return ;
		}
		if ( now->mama->child[0] == now ) {
			_selectrev(now->mama, 0, p, res);
		}
		else {
			_selectrev(now->mama, 1, p, res);
		}
	}
	void _select ( NODE *now, int blk_hash, INT o, INT *res ) {
		NODE *leaf = getLeaf(now, blk_hash, 0);
		if ( leaf->mama->child[0] == leaf ) {
			_selectrev(leaf->mama, 0, o, res);
		}
		else {
			_selectrev(leaf->mama, 1, o, res);
		}
	}
	void select ( int blk_hash, INT o, INT *res ) {
	//	if ( o < 0 ) {
	//		std::cout << "o >= 0 required\n";
	//		return (0);
	//	}
	//	if ( !select_support ) {
	//		std::cout << "call support_select() for select ops.\n";
	//		return (0);
	//	}
	//	if ( tree->alphabet[blk_hash] == -1 ) {
	//		std::cout << "this alphabet does not belong to exist\n";
	//		return (0);
	//	}
		_select(root, blk_hash, o, res);
	}
	void support_rank () {
		_support_rank(root);
		rank_support = true;
	}
	void _support_rank ( NODE *now ) {
		mem_used -= now->mem_used;
		now->support_rank();
		mem_used += now->mem_used;

		if ( !isLeaf(now->child[0]) ) {
			_support_rank(now->child[0]);
		}
		if ( !isLeaf(now->child[1]) ) {
			_support_rank(now->child[1]);
		}
	}
	void support_select () {
		_support_select(root);
		select_support = true;
	}
	void _support_select ( NODE *now ) {
		now->bitmap->support_select();
		if ( now->child[0] != NULL ) {
			_support_select(now->child[0]);
		}
		if ( now->child[1] != NULL ) {
			_support_select(now->child[1]);
		}
	}
}; // end struct wt

} // end namespace
#endif
