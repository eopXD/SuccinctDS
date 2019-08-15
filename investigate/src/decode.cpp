// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file decode.cpp
    \brief decode result data (decompressed 4x5), only take the bits i want
	to save into a seperate file.
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/

// C99
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
// C++
#include <cstdio>
#include <iostream>
#include <ctime>
// eopxd
#include "include/util.hpp"
using namespace eopxd;
#define PERMISSION S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IROTH

typedef uint64_t UINT64;
int main ()
{
	clock_t start = clock();
	char filename[100];
	char write_filename[2][100];
	unsigned char buf[65536];
	unsigned char write_buf[2][65536];
	int buf_size = 65536, read_size;
	sprintf(filename, "/tmp2/b04705001/4x5/decompressed/concat_all.dat");
	std::cout << "decoding file: " << filename << "\n";		
	struct stat st;
	stat(filename, &st);
	std::cout << "file size: " << st.st_size << ", " << (double)st.st_size/(1<<30) << "G\n";
	
	int in_fd = open(filename, O_RDONLY);
	if ( in_fd < 0 ) {
		std::cout << "open file fail\n";
		exit(1);
	}	
	sprintf(write_filename[0], "/tmp2/b04705001/4x5/decompressed/concat_all.decode0");
	sprintf(write_filename[1], "/tmp2/b04705001/4x5/decompressed/concat_all.decode1");
	int write_fd[2];
	write_fd[0] = open(write_filename[0], O_CREAT|O_TRUNC|O_WRONLY, PERMISSION);
	write_fd[1] = open(write_filename[1], O_CREAT|O_TRUNC|O_WRONLY, PERMISSION);
	if ( write_fd[0] < 0 ) {
		std::cout << "write_fd[0] fail\n";
		exit(1);
	}
	if ( write_fd[1] < 0 ) {
		std::cout << "write_fd[1] fail\n";
		exit(1);
	}
	std::cout << "writing file (decode0): " << write_filename[0] << "\n";
	std::cout << "writing file (decode1): " << write_filename[1] << "\n";
	UINT64 progress = 0;
	while ( (read_size=read(in_fd, buf, buf_size)) > 0 ) {
		progress += read_size;
		int now = 0;
		for ( int i=0; i<read_size; i+=8 ) {
			unsigned char c0 = buf[i+5]; // 40~47 bit
			unsigned char c1 = buf[i+6]; // 48~55 bit
			// win value (6 bit): 45~50
			// board statues, win/lose/draw (2 bit): 51, 52
			c0 <<= 5; c0 >>= 5;
			c1 >>= 3; 
			unsigned char decode0 = (c0<<3)+(c1>>2); // 6 bits with leading zeros
			unsigned char decode1 = (c0<<5)+c1; // 8 bits (6+2)
			/*decimal_to_binary(8, c0);
			decimal_to_binary(8, c1);
			star_line();
			decimal_to_binary(8, decode0);
			decimal_to_binary(8, decode1);
			line(); line();*/
				
			write_buf[0][now] = decode0;
			write_buf[1][now++] = decode1;
		}	
		for ( int i=0; i<2; ++i ) {
			write(write_fd[i], write_buf[i], now);
		}
		if ( progress%(1LL<<28) == 0 ) {
			std::cout << "progress: " << (double)progress/st.st_size << "\n";
		}
	}
	std::cout << "\nprogram successful execute\n";
	std::cout << "time: " << spent_time(start) << " seconds\n";
	return (0);
}
