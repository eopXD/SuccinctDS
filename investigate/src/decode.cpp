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
#include <stdint.h>
// C++
#include <cstdio>
#include <iostream>
#include <ctime>
// eopxd
#include "include/util.hpp"
using namespace eopxd;

#define PERMISSION S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IROTH
int main ()
{
	clock_t start = clock(), stamp = clock();
	char filename[100];
	char write_filename[2][100];
	unsigned char buf[65536];
	unsigned char write_buf[2][65536];
	int buf_size = 65536, read_size;
	for ( int i=0; i<30; ++i ) {
		sprintf(filename, "/tmp2/b04705001/4x5_result/decomp_result4x5-256.part0%02d", i);
		puts(filename);
		int fd = open(filename, O_RDONLY);
		if ( fd < 0 ) {
			std::cout << "open file fail\n";
			exit(1);
		}	
		
		sprintf(write_filename[0], "/tmp2/b04705001/4x5_result_decoded0/part0%02d.dat", i);
		sprintf(write_filename[1], "/tmp2/b04705001/4x5_result_decoded1/part0%02d.dat", i);
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

		while ( (read_size=read(fd, buf, buf_size)) > 0 ) {
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
				//break;
			}	
			//break;
			for ( int i=0; i<2; ++i ) {
				write(write_fd[i], write_buf[i], now);
			}
			//break;
		} 
		//break;
		std::cout << "complete part " << i << "\n";
		std::cout << "time: " << spent_time(stamp) << " seconds\n";
		stamp = clock();
		//break;
	}
	std::cout << "\nprogram successful execute\n";
	std::cout << "time: " << spent_time(start) << " seconds\n";
	return (0);
}
