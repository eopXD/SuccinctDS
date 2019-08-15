// C99
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
// C++
#include <iostream>
#include <iomanip>
#include <map>
// eopxd
#include "include/util.hpp"
using namespace std;
typedef unsigned char UCHAR;
typedef uint64_t UINT64;
int main ()
{
	char filename[100];
	cout << "enter filename(absolute path): ";
	cin >> filename;

	cout << "\n\nreading file: " << filename << "\n";
	struct stat st;
	stat(filename, &st);
	cout << "file size: " << st.st_size << ", " << (double)st.st_size/(1<<30) << "G\n";

	map<UINT64, UINT64> mp;
	UINT64 blk;
	int progress = 0;
	int buf_size = 65536;
	UCHAR buf[buf_size];
	int read_size, fd;
	while ( 1 ) {
		mp.clear();
		fd = open(filename, O_RDONLY);
		if ( fd < 0 ) {
			cout << "open file fail\n";
			exit(1);
		}
		int l, r;
		cout << "0~63, l = starting bit, r = ending bit, inclusive\n";
		cout << "insert l r (seperate with space): ";
		cin >> l >> r;
		for ( int i=0; i<(1<<(r-l+1)); ++i ) {
			UINT64 tmp = i;
			//std::cout << tmp << "XDDD\n";
			mp[tmp] = 0;
		}
		while ( (read_size=read(fd, buf, buf_size)) > 0 ) {
			for ( int i=0; i<read_size; i+=8 ) { // every 64-bit a data
				blk = 0;
				for ( int j=0; j<8; ++j ) {
					blk = (blk<<8)+buf[i+j];
				}		
				blk <<= l;
				blk >>= (l+63-r);
				//blk <<= (63-r);
				mp[blk]++;
			}
			progress += read_size;
			if ( progress%(1LL<<28) == 0 ) {
				std::cout << "progress: " << (double)progress/(UINT64)st.st_size << "\n";
			}
		}
		UINT64 total_blk = 0;
		for ( map<UINT64, UINT64>::iterator it=mp.begin(); it!=mp.end(); ++it ) {
			std::cout << std::setfill('0') << std::setw(5) << it->first << ": ";
			eopxd::decimal_to_binary(r-l+1, it->first);
			std::cout << ": \t" << it->second << "\n";
			total_blk += it->second;
		}
		std::cout << "total blocks: " << total_blk << "\n";
		close(fd);
	}
	return (0);
}
