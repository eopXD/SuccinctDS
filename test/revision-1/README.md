sdsl version 2.1.1

## Test data

1.8 G (2000000000 bytes) randomly generated data (seed `112358`).

## Revisions

- avoid `std::string` uses in all cause
- avoid conditional branch in `rank()`

## Initialization time

| Operation         | Original | Revision | Speedup |
|-------------------|----------|----------|---------|
| get frequency     | 352.662  | 6.3998   | 55.105  |
| fill bit          | 600.254  | 100.496  | 5.9729  |
| rrr construction  | 38.647   | 38.4102  | 1       |
| Total             | 991.567  | 145.3112 | 6.824   |

### "get_frequency" vs. "fill bit"

Ratio = `15.7`

The ratio don't fit the estimation of `x8`, which is the tree height. Further investigation needed.

## access operation

| # of access       | Original | Revision | Speedup |
|-------------------|----------|----------|---------|
| 100000000         | 231.64   | 226.641  | 1.022   |
| 200000000         | 460.637  | 451.828  | 1.019   |
| 400000000         | 906.981  | 902.297  | 1.005   |

There is no real improvement here, though for access I also eliminated the conditional branch. So this means that the overhead of conditional branch is actually trivial since showing only slight speedup.

### rank operation 

| # of rank         | Original | Revision | Speedup |
|-------------------|----------|----------|---------|
| 100000000         | 620.191  | 502.474  | 1.234   |
| 200000000         | 1210.8   | 994.896  | 1.217   |
| 400000000         | 2622.41  | 1982.43  | 1.323   |

For rank operation the speed increases by 20%, I think the boost comes from the elimination of `std::string`. 

## access vs. rank

| operation | bit_vector<rrr15> | wavelet<rrr15>    | ratio |
|-----------|-------------------|-------------------|-------|
| access    | 2.65677e-7        | 2.314e-6          | 8.709 |
| rank      | 5.12577e-7        | 5.024e-6          | 9.801 |

`rank` / `access` = `x2.17`

The perfermance ratio between `rank` and `access` still doesn't match up. This means that apart from the underlying bitvector, the function calling logics and implementation heavily affects the performance. This leads me to the next revision below.

## More to revise

By taking another look to my functions, I found out that overhead my come from my ways of returning the value. 

Also since I am dynamically allocating everything, potential memory leak hazards shall all be checked again.

<!--For the access function, it returns the a `char*`.

For the rank function, it returns a `INT`, this seems to be a potential cost, since for every recursion, an `INT` is created and copies value from its function call to the next iterations. -->