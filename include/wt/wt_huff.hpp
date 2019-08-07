// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file wavelet_huffman.hpp
    \brief wavelet_huffman.hpp contains methods to construct Huffman-code-
    shaped Wavelet Tree. The tree includes good properties such as it performs 
    best in query when the query probability is of the empirical probability 
    of the string.
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/

//*******************************	
//** TODO: SIZE CALCULATION    **
//*******************************
//calculation of the data structure is neccessary

//*******************************	
//** TODO: PEAK MEMORTY USAGE  **
//*******************************
// peak of memory usage is also needed to 

#ifndef EOPXD_WAVELET_HUFFMAN_HPP
#define EOPXD_WAVELET_HUFFMAN_HPP

// C99
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <stdint.h>
#include <ctime>
// C++
#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <algorithm>
// eopXD
#include "wt_node.hpp"

//! namespace by eopXD
namespace eopxd {

template<class bitvector_type>
struct wt_huff {

	typedef uint64_t INT;
	typedef bitvector_type BV;

	typedef std::pair<INT, int> PII;
	typedef std::vector<PII> VII;
	
	typedef wt_node<BV> NODE;
	typedef std::vector< NODE* > VNODE; // vector of node pointers
	
	static bool cmp ( const NODE *a, const NODE *b ) { return (a->len > b->len); }

// this array size only supports up to 4 bytes per alphabet, but can be easily adjusted
	const int max_bpa = 2; // 2^(2*8) = 65536
	
	int bpa; // byte per alphabet

	int idx;
	int alphabet[65537]; 	// [hash] = idx in freq[]
	bool* huffcode[65537]; 	// [hash] = huffman code
	int huff_len[65537]; 	// length of huffcode[]

	VII freq; // [i] = pair(frequency, alphabet), for sorting
	VNODE vec; // sort it up, merge it up!!!
	NODE *root; // root of tree
	
	INT mem_used; // memory allocated

	void add_freq ( int blk_hash ) {
		if ( alphabet[blk_hash] == -1 ) { // new alphabet
			alphabet[blk_hash] = idx++;
			freq.push_back(std::make_pair(0, blk_hash));
		}
		++(freq[alphabet[blk_hash]].first);
	}
	// stream mode
	void get_freq ( char filename[], int name_len, bool dummy  ) {
		int fd = open(filename, O_RDONLY);
		if ( fd < 0 ) {
			std::cout << "get_freq: fail to open\n";
			exit(1);
		}	
		INT buf_size = bpa*65536, sz;
		unsigned char *buf = new unsigned char [buf_size];
		int blk_hash = 0, len = 0; // blk for hashing
		while ( (sz=read(fd, buf, buf_size)) > 0 ) {
			for ( INT i=0; i<sz; ++i ) {
				++len;
				blk_hash = (blk_hash<<8) + buf[i]; // hashing
				if ( len == bpa ) {
					add_freq(blk_hash);
					blk_hash = len = 0;
				}
			}
		}	
		close(fd);
		delete [] buf;
	}
	// batch mode
	void get_freq ( unsigned char *data, INT data_len ) {
		int blk = 0, len = 0;
		for ( INT i=0; i<data_len; ++i ) {
			++len;
			blk = (blk<<8) + data[i];
			if ( len == bpa ) {
				add_freq(blk);
				blk = len = 0;
			}
		}
	}
	void construct_tree () {

		mem_used += sizeof(VII)+(sizeof(PII)*freq.size());
		//std::cout << "vector<PII> freq: " << sizeof(VII)+(sizeof(PII)*freq.size()) << "\n";
		std::cout << "[huff_wt] empirical frequency\n";
		for ( INT i=0; i<freq.size(); ++i ) {
			std::cout << "\t" << freq[i].second << ":" << freq[i].first << "\n";
			NODE *a = new NODE(freq[i].second, bpa, freq[i].first);
			//std::cout << "leaf node: " << a->mem_used << "\n";	
			mem_used += a->mem_used; // memory of node, leaf node no  bitvec
			
			vec.push_back(a);
		}
		mem_used += sizeof(VNODE)+(sizeof(NODE*)*vec.size());
		//std::cout << "vector<NODE*> vec: " << sizeof(VNODE)+(sizeof(NODE*)*vec.size()) << "\n";

		std::cout << "[huff_wt] node merge...\n";
		while ( vec.size() > 1 ) {
			std::sort(vec.begin(), vec.end(), cmp);
			//for ( typename VNODE::iterator it=vec.begin();
			//	it!=vec.end(); ++it ) {
			//	it->display_bitmap(); }
			NODE *a = vec[vec.size()-2], *b = vec[vec.size()-1];
			vec.pop_back(); vec.pop_back();
			NODE *c = new NODE(a, b);
			
			//std::cout << "internal node: " << c->mem_used << "\n";
			mem_used += c->mem_used; // memory of node, including bitvec

			a->mama = b->mama = c;
			vec.push_back(c);
		}
		root = vec[0]; // root is the last remaining node
		//root->display_bitmap();
	}

	void dfs ( NODE *now, bool *code, int len ) {
		if ( now->child[0] == nullptr and now->child[1] == nullptr ) { // is leaf
			huffcode[now->blk_hash] = new bool [len];
			
			mem_used += sizeof(bool)*len;
			//std::cout << "huffcode: " << sizeof(bool)*len << "\n";

			huff_len[now->blk_hash] = len;
			for ( int i=0; i<len; ++i ) {
				huffcode[now->blk_hash][i] = code[i];
			}
			return ;
		}
		code[len] = 0;
		dfs(now->child[0], code, len+1);
		code[len] = 1;
		dfs(now->child[1], code, len+1);
	}
	void decode ( int blk_hash ) {
		unsigned char blk[bpa+1];
		blk[bpa] = '\0';
		for ( int i=bpa-1; i>=0; i-- ) {
			blk[i] = blk_hash%256;
			blk_hash >>= 8;
		}
		std::cout << "\t" << blk << ": ";
	} 
	void gen_code () {
		bool *code = new bool [100];
		dfs(root, code, 0);
		std::cout << "[wt_huff] Huffman code generated\n";
		for ( int i=0; i<65537; ++i ) {
			if ( alphabet[i] != -1 ) {
				//decode(i);
				std::cout << "\t" << i << ": ";
				for ( int j=0; j<huff_len[i]; ++j ) {
					std::cout << huffcode[i][j];
				}
				std::cout << "\n";
			}
		}  
		std::cout << "\n**************************************************\n";
		delete [] code;
	}

	void fill_bit ( NODE *now, int blk_hash, int lv ) {
		if ( now->child[0] == nullptr and now->child[1] == nullptr ) {
			return ;
		}
		now->bitmap->bitvec[now->bitmapcnt++] = huffcode[blk_hash][lv];
		fill_bit(now->child[huffcode[blk_hash][lv]], blk_hash, lv+1);
	}
	void fill_data ( char filename[], int name_len, bool dummy ) {
		int fd = open(filename, O_RDONLY);
		if ( fd < 0 ) {
			std::cout << "fill_data: fail to open\n";
			return ;
		}
		INT buf_size = bpa*65536, sz;
		unsigned char *buf = new unsigned char [buf_size];
		int blk_hash = 0, len = 0;
		while ( (sz=read(fd, buf, buf_size)) > 0 ) {
			for ( INT i=0; i<sz; ++i ) {
				++len;
				blk_hash = (blk_hash<<8)+buf[i];
				if ( len == bpa ) {
					fill_bit(root, blk_hash, 0);
					blk_hash = len = 0;
				}
			}
		}	
		close(fd);
		delete [] buf;
	}
	void fill_data ( unsigned char *data, INT data_len ) {
		int blk_hash = 0, len = 0;
		for ( INT i=0; i<data_len; ++i ) {
			++len;
			blk_hash = (blk_hash<<8)+data[i];
			if ( len == bpa ) {
				fill_bit(root, blk_hash, 0);
				blk_hash = len = 0;
			}
		}
	}
	void _init ( int _bpa ) {
		idx = 0;
		for ( int i=0; i<65537; ++i ) {
			alphabet[i] = -1;
			huffcode[i] = NULL;
			huff_len[i] = -1;
		}
		bpa = _bpa;
	}
	void account_mem () {
		// huffcode[] will be added in gen_code
		mem_used += sizeof(alphabet);
		//std::cout << "alphabet: " << sizeof(alphabet) << "\n";
		mem_used += sizeof(huffcode);
		//std::cout << "huffcode: " << sizeof(huffcode) << "\n";
		mem_used += sizeof(huff_len); 
		//std::cout << "huff_len: " << sizeof(huff_len) << "\n";
		mem_used += sizeof(int)*2; // idx, bpa (8 Byte)
		//std::cout << "idx, bpa: " << sizeof(int)*2 << "\n";
	}
	// defualt constructor (SHALL NOT BE CALLED)
	wt_huff () {
		std::cout << "[wt_huff] default constructor ~~~n";
		exit(1);
	}
	// batch mode constructor
	wt_huff ( unsigned char *data, INT data_len, int _bpa ) {
		mem_used = 0;
		_init(_bpa);
		std::cout << "[wt_huff] batch mode constructor activated\n";
		get_freq(data, data_len);
		construct_tree();
		gen_code();
		fill_data(data, data_len);
		account_mem();
	}
	// stream mode constructor (bool is dummy variable)
	wt_huff ( char *filename, int name_len, int _bpa, bool dummy ) {
		mem_used = 0;
		_init(_bpa);
		std::cout << "[wt_huff] stream mode constructor activated\n";
		clock_t start = clock(), stamp = clock();	
		get_freq(filename, name_len, 1);
		std::cout << "get_freq: " << spent_time(stamp) << " seconds\n";
		stamp = clock();
		construct_tree();
		std::cout << "construct_tree: " << spent_time(stamp) << " seconds\n";
		stamp = clock();	
		gen_code();
		std::cout << "gen_code: " << spent_time(stamp) << " seconds\n";
		stamp = clock();
		fill_data(filename, name_len, 1);
		std::cout << "fill_data: " << spent_time(stamp) << " seconds\n";
		stamp = clock();
		std::cout << "total: " << spent_time(start) << " seconds\n";
		account_mem();	
	}

}; // end struct wt_huff

} // end namespace
#endif
