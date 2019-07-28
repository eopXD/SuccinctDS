sdsl version 2.1.1

## Test data

1.8 G (2000000000 bytes) randomly generated data (seed `112358`).

## Revisions

- passing the result pointer into the function

The cost of return values of access/rank verses putting it inside the function argument and make the functiona a void function.

## Result

It shows worse performance than the 1st revision. Therefore we are adjusting back to the previous edition. This also gives me a lesson that to add into a new addup or revision, using git branch shall be better and easier to maintain.
