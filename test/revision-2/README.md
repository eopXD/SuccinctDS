sdsl version 2.1.1

## Test data

1.8 G (2000000000 bytes) randomly generated data (seed `112358`).

## Revisions

- passing the result pointer into the function

The cost of return values of access/rank verses putting it inside the function argument and make the functiona a void function.

## Result

I then retested both revisions on the same machine, my previous observation may be inaccurate because it is tested on different environments. Here is the following result: 

| Opertaion       | Revision-1 | Revision-2 | Speedup |
|-----------------|------------|------------|---------|
| Initialization  | 113.38     | 112.417    | 1.0085  |
| Support rank    | 40.6891    | 43.4096    | 0.9373  |
| access 1e8      | 236.444    | 223.05     | 1.06    |
| access 2e8      | 458.417    | 442.764    | 1.035   |
| access 4e8      | 915.245    | 885.567    | 1.033   |
| rank 1e8        | 271.526    | 249.224    | 1.089   |
| rank 2e8        | 514.915    | 497.532    | 1.035   |
| rank 4e8        | 1027.72    | 998.297    | 1.029   |

There is approximately 3% of speedup compared to the first revision. I have expected more of an improvement.