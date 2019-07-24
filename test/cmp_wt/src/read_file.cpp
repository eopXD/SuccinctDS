// C
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <ctime>
#include <cmath>
// C++ 
#include <iostream>

#include "include/util.hpp"

int main ()
{
	clock_t start = clock();
	char filename[25] = "test_data.big";
	int fd = open(filename, O_RDONLY);
	if ( fd < 0 ) {
	 	std::cout << "fail to open\n";
		return (0);	
	}
	int buf_size = 65536; // 2^20
	char *buf = new char [buf_size];
	int sz;
	int cnt = 0;
	while ( (sz=read(fd, buf, buf_size)) > 0 ) {
		cnt++;
		//std::cout << sz << " ";
	} puts("");
	std::cout << "times read: " << cnt << "\n";
	std::cout << "total: " << spent_time(start) << " seconds\n";
	close(fd);
	return (0);
}
