# Memory usage analysis on RRR BitVector


## Basic Block = `15` bit

| Usage        | Optimal                | Implementation         |
|--------------|------------------------|------------------------|
| Popcount     | 4 bit                  | 8 bit                  |
| Permutation  | 13 bit                 | 16 bit                 |
| SuperBlkPop  | 0.5 bit                | 0.5 bit                |
| Total        | 17.5 bit               | 24.5 bit               |
| Ratio        | 1.167 (16.7% overhead) | 1.633 (63.3% overhead) |

For the lookup table, 

- step & persum: $8 * (15+5) * 2 = 320\ Byte$
- lookpop: $2 * (2^{15} + 5) = 32773\ Byte$
- lookperm & revlookup:  $4 * (2^{15} + 5) * 2 = 131092\ Byte$

Total of `164185` ~ `160.3369` KB

## Basic Block = `31` bit 

| Usage        | Optimal                | Implementation         |
|--------------|------------------------|------------------------|
| Popcount     | 5 bit                  | 8 bit                  |
| Permutation  | 29 bit                 | 32 bit                 |
| SuperBlkPop  | 0.5 bit                | 0.5 bit                |
| Total        | 34.5 bit               | 40.5 bit               |
| Ratio        | 1.113 (11.3% overhead) | 1.306 (30.6% overhead) |

For the lookup table, 

- step & persum: $8 * (31+5) * 2 = 576\ Byte$
- lookpop: $2 * (2^{31} + 5) = 4294967306\ Byte$
- lookperm & revlookup:  $4 * (2^{31} + 5) * 2 = 17179869224\ Byte$

Total of `21474837106` ~ `20` GB


## Conclusion

Optimal rate can be approached via "Variable-Length Encoding", after optimal coding the compression rate only improves by 1~5%. So the next main task shall be building up an variable coding library.