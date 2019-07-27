#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iostream>

typedef uint64_t INT;
INT file_size = 2000000000;
int main ()
{
	srand(112358);
	freopen("test_data.big", "w", stdout);
	for ( INT i=0; i<file_size; ++i ) {
		unsigned char c = rand()%256;
		putchar(c);
	}
	fclose(stdout);
	return (0);
}
