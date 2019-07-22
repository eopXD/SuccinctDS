// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file wavelet_huffman.hpp
    \brief wavelet_huffman.hpp contains methods to construct Huffman-code-
    shaped Wavelet Tree. The tree includes good properties such as it performs 
    best in query when the query probability is of the empirical probability 
    of the string.
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/

#ifndef EOPXD_WAVELET_HUFFMAN_HPP
#define EOPXD_WAVELET_HUFFMAN_HPP

// C99
#include <stdint.h>
// C++
#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
// eopXD
#include "wt_node.hpp"

//! namespace by eopXD
namespace eopxd {

template<class bitvector_type>
struct wt_huff {

	typedef uint64_t INT;
	typedef bitvector_type BV;
	typedef std::string STR;

	typedef std::map<STR, INT> MAPSI;
	typedef std::map<STR, STR> MAPSS;
	typedef std::pair<INT, STR> PIS;
	typedef std::vector<PIS> VIS;
	
	typedef wt_node<BV> NODE;
	typedef std::vector< NODE* > VNODE; // vector of node pointers
	
	static bool cmp ( const NODE *a, const NODE *b ) { return (a->len > b->len); }

	INT bpa; // byte per alphabet

	MAPSI alphabet; // [alphabet] = index
	MAPSS huffcode; // [alphabet] = huffcode

	VIS freq; // [i] = pair(frequency, alphabet)
	VNODE vec; // sort it up, merge it up!!!
	NODE *root; // root of tree

	// also allocates space for bitmap (rank/select not built)
	
	// stream mode
	void get_freq ( STR filename, std::streamsize size ) {
		std::ifstream *file = start_stream(filename);
		INT buf_size = bpa * 1024;
		char *buf = new char [buf_size];
		STR blk;
		while ( file->read(buf, buf_size) ) {
			INT extracted = file->gcount();	
			for ( int i=0; i<extracted; ++i ) {
				blk += buf[i];
				if ( blk.size() == bpa ) {
					if ( alphabet.count(blk) == 0 ) {
						std::cout << "get block: " << blk << "\n";
						INT idx = alphabet.size();
						alphabet[blk] = idx;
						freq.push_back(make_pair(0, blk));
					}
					++(freq[alphabet[blk]].first);
					blk.clear();
				}
			}
		}	
	}
	// batch mode
	void get_freq ( STR &data ) {
		STR blk;
		for ( char c: data ) {
			blk += c;
			if ( blk.size() == bpa ) {
				if ( alphabet.count(blk) == 0 ) {
					INT idx = alphabet.size();
					alphabet[blk] = idx;
					freq.push_back(make_pair(0, blk));
				}
				++(freq[alphabet[blk]].first);
				blk.clear();
			}
		}
	}
	void construct_tree () {
		std::cout << "[huff_wt] empirical frequency\n";
		for ( INT i=0; i<freq.size(); ++i ) {
			std::cout << "\t" << freq[i].second << ":" << freq[i].first << "\n";
			NODE *a = new NODE(freq[i].second, freq[i].first);
			vec.push_back(a);
		}
		std::cout << "[huff_wt] node merge...\n";
		while ( vec.size() > 1 ) {
			std::sort(vec.begin(), vec.end(), cmp);
			//for ( typename VNODE::iterator it=vec.begin();
			//	it!=vec.end(); ++it ) 
			//	it->display_bitmap();
			NODE *a = vec[vec.size()-2], *b = vec[vec.size()-1];
			vec.pop_back(); vec.pop_back();
			NODE *c = new NODE(a, b);
			a->mama = b->mama = c;
			vec.push_back(c);
		}
		root = vec[0];
		//root->display_bitmap();
	}

	void dfs ( NODE *now, STR str ) {
		if ( now->left == nullptr and now->right == nullptr ) { // is leaf
			huffcode[now->leaf] = str;
			return ;
		}
		dfs(now->left, str+"0");
		dfs(now->right, str+"1");
	}

	void gen_code () {
		dfs(root, STR());
		std::cout << "[wt_huff] Huffman code generated\n";
		for ( typename MAPSS::iterator it=huffcode.begin(); 
			it!=huffcode.end(); ++it ) {
			std::cout << "\t" << it->first << ":" << it->second << "\n";
		}
		std::cout << "\n**************************************************\n";
	}

	void fill_bit ( NODE *now, STR str, INT lv ) {
		if ( lv == str.size() ) {
			return ;
		}
		now->bitmap->bitvec[now->bitmapcnt++] = str[lv]-'0';
		if ( str[lv] == '0' ) {
			fill_bit(now->left, str, lv+1);
		}
		else {
			fill_bit(now->right, str, lv+1);
		}
	}
	void fill_data ( STR filename, std::streamsize size ) {
		std::ifstream *file = start_stream(filename);
		INT buf_size = bpa * 1024;
		char *buf = new char [buf_size];
		STR blk;
		while ( file->read(buf, buf_size) ) {
			INT extracted = file->gcount();	
			for ( int i=0; i<extracted; ++i ) {
				blk += buf[i];
				if ( blk.size() == bpa ) {
					STR code = huffcode[blk];
					fill_bit(root, code, 0);
					blk.clear();
				}
			}
		}		
	}
	void fill_data ( STR &data ) {
		STR blk;
		for ( char &c: data ) {
			blk += c;
			if ( blk.size() == bpa ) {
				STR code = huffcode[blk];
				fill_bit(root, code, 0);
				blk.clear();
			}
		}
	}
	// defualt constructor (SHALL NOT BE CALLED)
	wt_huff () {
		std::cout << "[wt_huff] default constructor 😈\n";
		exit(1);
	}
	// batch mode constructor
	wt_huff ( STR &data, INT _bpa ) {
		std::cout << "[wt_huff] batch mode constructor activated\n";
		bpa = _bpa;
		get_freq(data);
		construct_tree();
		gen_code();
		fill_data(data);
	}
	// stream mode constructor
	wt_huff ( STR filename, std::streamsize size, INT _bpa ) {
		std::cout << "[wt_huff] stream mode constructor acctivated\n";
		bpa = _bpa;
		get_freq(filename, size);
		construct_tree();
		gen_code();
		fill_data(filename, size);
	}

}; // end struct wt_huff

} // end namespace
#endif