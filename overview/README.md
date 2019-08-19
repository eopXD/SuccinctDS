# Structure overview

This is a documentation on the succinct data structure.

## `misalign.hpp`

#### Creation

- `misalign<compact_bit_length, MEDIUM, medium_bit_length>`
- ex: `misalign<4, uint8_t, 8>`

#### Structure

```
template<uint64_t needed_bit_len, class medium, uint64_t medium_bit_len> 
struct misalign { // needed_bit_len < medium_bit_len
	typedef unsigned char UCHAR;
	typedef uint64_t UINT64;
	typedef medium VAR;

	VAR *data;
	UINT64 data_len;

	UINT64 max_pos;
	VAR max_val;

	UINT64 mem_used; // memory used
	
	// utility numbers
	VAR diff; // difference between medium length and compact length
	VAR full; // 64-bit full oness
	VAR ones[64+1][64+1];

}
```

#### Overhead

Main overhead is the `65x65` array `ones`.

Assuning variable `VAR` takes `x` bytes...

The basic overhead is $32 + 4225 \cdot x$ bytes.

## `bv_lookup.hpp` - lookup table

Block bitstring into fixed length blocks, for the blocks we need the following
lookup table, n-bit block takes memory of:

- $2^n \cdot \log n$ bit: [bitstring] = popcount
- $2^n \cdot \log n$ bit: [bitstring] = permutation
- $2^n \cdot n$ bit: [popcount][permutation] = bitstring

It is appropriate to set BasicBlock size to 15.

$b = 15$, we would need $2^{15} \cdot (4+4+15)$ bit ~ 1MB

#### Implementation

```
blk_size = 15

step = new uint64_t [blk_size+5];
permsum = new uint64_t [blk_size+5];
lookpop = new uint8_t [(1<<blk_size)+5];
lookperm = new uint16_t [(1<<blk_size)+5];
revlookup = new uint16_t [(1<<blk_size)+5];
```

Implementation takes memory $164201$ bytes $= 160.3369$ KB

## `bv_lookup.hpp` - bit vector

#### Creation

- `bv_lookup(allocate_length)`
- ex: `bv_lookup(1000)`

#### Structure

```
struct bv_lookup {

	typedef uint8_t UINT8;
	typedef uint16_t UINT16;
	typedef uint64_t UINT64;
	
	typedef misalign<1, UINT8, 8> COMPACT_BOOL;
	typedef misalign<4, UINT8, 8> COMPACT_POP;
	typedef misalign<13, UINT16, 16> COMPACT_PERM;

	UINT64 len;
	
	COMPACT_BOOL *bv;
	COMPACT_POP *pop;
	COMPACT_PERM *perm;
	UINT64 *superpop;

	UINT64 blkcnt, superblkcnt;
	
	UINT64 mem_used;
	bool rank_support;
}
```

#### Overhead

There are 3 underlying compact arrays - `bv`, `pop`, `perm`, bringing the overhead of $4257$ bytes before calling `support_rank()` and $12739$ bytes after calling `support_rank()`.

Besides the compact arrays, pointers and variables cost $65$ bytes.

Summing it up:

- Before `support_rank()`: $4362$
- After `support_rank()`: $12806$


## `wt_node`.hpp

#### Structure

```
template<class bitvector_type>
struct wt_node {
	typedef uint64_t UINT64;
	typedef unsigned char UCHAR;
	typedef bitvector_type BV;

	int bpa;
	wt_node *mama;
	wt_node *child[2]; // [0] = left, [1] = right (to avoid conditional branch)
	BV *bitmap;
	UINT64 bitmapcnt; // for filling bit 1 by 1 ( check huff_wt->fill_bit() )
	UINT64 len;
	int blk_hash;
	UCHAR *blk;

	UINT64 mem_used;
}
```

#### Overhead

Contains a bit vector pointer, so right here having the overhead of `bv_lookup`. 

Pointers and variables cost $72$ bytes.

Adding with the underlying bit vector:

- Before `support_rank()`: $4434$
- After `support_rank()`: $12878$

## `wt_huff.hpp`

#### Structure

```
template<class bitvector_type>
struct wt_huff {

	typedef uint64_t INT;
	typedef bitvector_type BV;

	typedef std::pair<INT, int> PII;
	typedef std::vector<PII> VII;
	
	typedef wt_node<BV> NODE;
	typedef std::vector< NODE* > VNODE; // vector of node pointers

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
}
```

#### Overhead

The main overhead is the hash table to saving alphabets and huffman codes. 

$65537 \cdot 16 = 1048592 = 1.00001$ MB

There are also `VII` and `VNODE` in the Huffman Tree construction, which depends on the size of the alphabet set. Assuming there are $x$ alphabets.

- `VII`: $24 + 16 \cdot x$
- `VNODE`: $24 + 8 \cdot x$

Other pointers and variables take $20$ bytes. (Overhaed of nodes are in the previous section)

#### Sample Calculation

For a tree with an alphabet size $| \Sigma |$, there will be $| \Sigma |-1$ internal nodes and $| \Sigma |$ leaf nodes. 

##### Before `support_rank()`

$4434 \cdot | \Sigma | + (| \Sigma |-1) \cdot 72 + 1048592$

##### After `support_rank()`

$12878 \cdot | \Sigma | + (| \Sigma |-1) \cdot 72 + 1048592$

## Huffman-shaped Wavelet Tree

Also, since this is a Huffman-shaped Wavelet tree, we can also know the length of bits we are going to have by its entropy. Assume entropy $H_0$, the total events $E$, the total bit will be $H_0 \cdot E$. This amount of bits will be stored, and $1.16 \cdot H_0 \cdot E$ will be the amount after `support_rank()`. (document of this constant overhead is in `test/mem_used/bv_lookup.md`)
