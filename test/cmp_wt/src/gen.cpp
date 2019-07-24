#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iostream>

typedef uint64_t INT;
INT file_size = 100000;
int main ()
{
	srand(112358);
	freopen("small.txt", "w", stdout);
	for ( INT i=0; i<file_size; ++i ) {
		int c = rand()%256;
		putchar(c);
	}
	fclose(stdout);
	return (0);
}
