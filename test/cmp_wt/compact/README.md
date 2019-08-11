# wavelet aggregating compact rrr bv

Aggregate compact rrr bitvector into wavelet tree

## Speed difference

| # of access | original wt<15>    | compact wt<15>      | ratio | 
|-------------|--------------------|---------------------|-------|
| 100000000   | 231.64             | 441.291             | 1.905 |
| 200000000   | 460.637            | 781.556             | 1.696 |
| 400000000   | 906.981            | 1567.29             | 1.728 |

| # of rank   | original RRR<15>   | compact RRR<15>     | ratio | 
|-------------|--------------------|---------------------|-------|
| 100000000   | 310.191            | 402.87              | 1.299 |
| 200000000   | 605.8              | 832.547             | 1.374 |
| 400000000   | 1311.41            | 1661.59             | 1.267 |


- access operation shows heavy delay 70%. 
- rank operation shows delay of 25% ~ 40%.