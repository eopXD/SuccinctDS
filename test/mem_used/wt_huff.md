# Memory usage analysis on Huffman-shaped Wavelet Tree

## Pointers & Hash table

| type    | variable name  | length   | size      |
|---------|----------------|----------|-----------|
| `int`   | alphabet       | 65537    | 262148    |
| `bool*` | huffcode       | 65537    | 524296    | 
| `int`   | huff_len       | 65537    | 262148    |

Total of `1048592 Byte ~ 1KB`

`int` variables: `idx`, `bpa`.

## Per Node

### Leaf node

72 Bytes for pointers and flags, another `bpa+1` bytes for recording the alphabet on the leaf node.  

## Internal Node

72 Bytes for pointers and flags, 65 Bytes for `bv_lookup` pointers and flags, and entra `bitvector_length` bytes for bit vector.

For a wavlet tree of height `h`, there would at most be `data_length * h` bits of bit vector. 

For $\Sigma$ alphabets, there would be $\Sigma - 1$ internal nodes.

## Conclusion

After RRR compression, there would be an extra 60% of overhead for supporting rank. Due to the $H_0$ compression by the wavelet tree, the actual memory usage can be calculated via its entropy.