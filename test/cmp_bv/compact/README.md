# compact rrr

With `misalign.hpp`, rrr bit vector can reach to its optimal extent of compression rate.

## Basic Block = `15` bit

| Usage        | Optimal                |
|--------------|------------------------|
| Popcount     | 4 bit                  |
| Permutation  | 13 bit                 |
| SuperBlkPop  | 0.5 bit                |
| Total        | 17.5 bit               |
| Ratio        | 1.167 (16.7% overhead) |

## Speed difference

| # of access | original RRR<15>   | compact RRR<15>     | ratio | 
|-------------|--------------------|---------------------|-------|
| 100000000   | 26.5677            | 28.2317             | 1.062 |
| 200000000   | 40.9602            | 42.22               | 1.03  |
| 400000000   | 78.5998            | 84.5876             | 1.076 |
| 800000000   | 159.596            | 168.934             | 1.059 |

| # of rank   | original RRR<15>   | compact RRR<15>     | ratio | 
|-------------|--------------------|---------------------|-------|
| 100000000   | 51.2577            | 68.7298             | 1.34  |
| 200000000   | 77.1612            | 112.499             | 1.458 |
| 400000000   | 154.828            | 226.282             | 1.46  |
| 800000000   | 315.236            | 450.332             | 1.428 |

access shows minor speed difference, approximately of 5%~8% slower than the original uncompact access. rank operation, however, based on access operation, accumulates the delay of access operation, hencse show a severe delay of 35%~47%.