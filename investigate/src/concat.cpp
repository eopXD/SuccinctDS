// concat all decompress files into one

// C99
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
// C++
#include <iostream>
#include <cmath>
#include <iomanip>
#include <map>

#define PERMISSION S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IROTH
int main ()
{
	char output_file[105] = "/tmp2/b04705001/4x5/decompressed/concat_all.dat";
	int out_fd = open(output_file, O_CREAT|O_TRUNC|O_WRONLY, PERMISSION);
	if ( out_fd < 0 ) {
		std::cout << "output file open fail\n";
		exit(1);
	}
	char buffer[65536];
	char filename[105];
	for ( int i=0; i<30; ++i ) {
		sprintf(filename, 
				"/tmp2/b04705001/4x5/decompressed/result4x5-256.part%03d", i);
		int in_fd = open(filename, O_RDONLY);
		if ( in_fd < 0 ) {
			std::cout << "read file open fail\n";
			exit(1);
		}
		int read_size, buf_size = 65536;
		while ( (read_size=read(in_fd, buffer, buf_size)) > 0 ) {
			write(out_fd, buffer, buf_size);
		}
		close(in_fd);
		std::cout << "part " << i << " complete\n";
	}
	close(out_fd);
	return (0);
}
