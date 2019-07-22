sdsl version 2.1.1

## bit vector

I am comparing between my rrr lookup table bit vector (support rank) with the rrr vector of sdsl library. The results are shown in `*.result`. Comparing the 2 implementations, with the same blocksize both rrr vectors show similar result.

sdsl supports bigger block size, which is a tradeoff between time and space. it shows that with bigger blocksize the time of access/rank also increases linearly (while the initialization time is x2 faster)

## inital compression

| self-implemented RRR | sdsl RRR<15> | sdsl RRR<63> |
|----------------------|--------------|--------------|
| 21.481               | 37.587       | 11.9815      |

## access() operation

| access time | self RRR<15> | sdsl RRR<15> | sdsl RRR<63> |
|-------------|----------------------|--------------|--------------|
| 100000000   | 26.5677              | 26.3428      | 53.8254      |
| 200000000   | 40.9602              | 51.7332      | 104.854      |
| 400000000   | 78.5998              | 105.318      | 219.478      |
| 800000000   | 159.596              | 215.323      | 440.612      |

![](img/access operation.png)

## rank() operation

| rank time | self RRR<15> | sdsl RRR<15> | sdsl RRR<63> |
|-------------|----------------------|--------------|--------------|
| 100000000   | 51.2577              | 37.7385      | 67.9364      |
| 200000000   | 77.1612              | 75.1629      | 151.004      |
| 400000000   | 154.828              | 162.457      | 262.063      |
| 800000000   | 315.236              | 306.403      | 535.298      |

![](img/rank operation.png)
