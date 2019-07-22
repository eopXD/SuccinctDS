// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file wavelet.hpp
    \brief wavelet.hpp contains wavelet tree rank/select operations.
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/


//*******************************	
//** TODO: SUPPORT STREAM MODE **
//*******************************
/*
stream mode: spend one time to read through it to investigate
empirical frequency of data. construct tree shape, then fill the
bitstrings in stream mode.

this feature is needed if the data cannot fit into memory.
*/

#ifndef EOPXD_WAVELET_HPP
#define EOPXD_WAVELET_HPP

// C99
#include <stdint.h>
// C++
#include <iostream>
#include <fstream>
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
	typedef std::string STR;

	typedef wt_type WT_T;
	typedef bitvector_type BV;
	typedef wt_node<BV> NODE;
	
	WT_T *tree;
	NODE *root;

	bool rank_support, select_support;

	wt () {
		std::cout << "[wt] give me data lah🤦\n";
		exit(1);
	}

	wt ( STR data, INT bpa ) { // bpa = bytes per alphabet
		std::cout << "[wt] batch mode\n";
		tree = new WT_T(data, bpa);
		root = tree->root;
		rank_support = select_support = false;
	}
	wt ( STR filename, std::streamsize size, INT bpa ) {
		std::cout << "[wt] stream mode\n";
		tree = new WT_T(filename, size, bpa);
		root = tree->root;
		rank_support = select_support = false;
	}
	bool isLeaf ( NODE *now ) { 
		return (now->left == nullptr and now->right == nullptr); 
	}

/* Access: access the alphabet on position p */
	STR _access ( NODE *now, INT p ) {
		if ( isLeaf(now) ) return (now->leaf);
		bool charbit = now->bitmap->access(p);
		INT next_p = now->bitmap->rank(p, charbit);
		if ( charbit == 0 ) {
			return (_access(now->left, next_p));
		}
		else {
			return (_access(now->right, next_p));
		}

	}
	STR access ( INT p ) {
		return (_access(root, p));
	}

/* Rank: count the occurence of alphabet from [0,p) */
	INT _rank ( NODE *now, STR code, INT p, INT lv ) {
		if ( isLeaf(now) ) return (p);
		bool charbit = bool(code[lv]-'0');
		int next_p = now->bitmap->rank(p, charbit);
		if ( charbit == 0 ) {
			return (_rank(now->left, code, next_p, lv+1));
		}
		else {
			return (_rank(now->right, code, next_p, lv+1));
		}
	}
	INT rank ( STR c, INT p ) {
		if ( !rank_support ) {
			std::cout << "call support_rank() for rank ops\n";
			return (-1);
		}
		if ( tree->huffcode.find(c) == tree->huffcode.end() ) {
			std::cout << "this alphabet does not belong to exist\n";
			return (-1);
		}
		return (_rank(root, tree->huffcode[c], p, 0));
	}

/* Select: find the n-th occurence of alphabet */
	NODE* getLeaf ( NODE *now, STR code, INT lv ) {
		if ( isLeaf(now) ) return (now);
		if ( code[lv] == '0' ) {
			return (getLeaf(now->left, code, lv+1));
		}
		else {
			return (getLeaf(now->right, code, lv+1)); 
		}
	}
	INT _selectrev ( NODE *now, bool charbit, INT o ) {
		int p = now->bitmap->select(o, charbit);
		if ( now->mama == nullptr ) { // reach root node
			return (p);
		}
		if ( p == -1 ) { // no such answer
			return (p);
		}

		if ( now->mama->left == now ) {
			return (_selectrev(now->mama, 0, p));
		}
		else {
			return (_selectrev(now->mama, 1, p));
		}
	}
	INT _select ( NODE *now, STR code, INT o ) {
		NODE *leaf = getLeaf(now, code, 0);
		if ( leaf->mama->left == leaf ) {
			return (_selectrev(leaf->mama, 0, o));
		}
		else {
			return (_selectrev(leaf->mama, 1, o));
		}
	}
	INT select ( STR c, INT o ) {
		if ( o < 0 ) {
			std::cout << "o >= 0 required\n";
			return (-1);
		}
		if ( !select_support ) {
			std::cout << "call support_select() for select ops.\n";
			return (-1);
		}
		if ( tree->huffcode.find(c) == tree->huffcode.end() ) {
			std::cout << "this alphabet does not belong to exist\n";
			return (-1);
		}
		return (_select(root, tree->huffcode[c], o));
	}
	void support_rank () {
		_support_rank(root);
		rank_support = true;
	}
	void _support_rank ( NODE *now ) {
		now->bitmap->support_rank();
		if ( !isLeaf(now->left) ) {
			_support_rank(now->left);
		}
		if ( !isLeaf(now->right) ) {
			_support_rank(now->right);
		}
	}
	void support_select () {
		_support_select(root);
		select_support = true;
	}
	void _support_select ( NODE *now ) {
		now->bitmap->support_select();
		if ( now->left != NULL ) {
			_support_select(now->left);
		}
		if ( now->right != NULL ) {
			_support_select(now->right);
		}
	}
}; // end struct wt

} // end namespace
#endif