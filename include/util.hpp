// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file util.hpp
    \brief util.hpp contains functions for convenience
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/

#ifndef EOPXD_UTIL_HPP
#define EOPXD_UTIL_HPP

#include <ctime>
#include <cmath>
#include <fstream>

namespace eopxd {

std::ifstream::pos_type get_size ( const char* filename ) {
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return (in.tellg()); 
}


double spent_time ( clock_t stamp ) {
	clock_t cost = clock() - stamp;
	return ((double)cost/CLOCKS_PER_SEC);
}

void line () {
	std::cout << "===========================================\n";
}
void star_line () {
	std::cout << "*******************************************\n";
}
void decimal_to_binary ( int length, uint64_t x ) {
	int str[100] = {};
	int now = 0;
	while ( x > 0 ) {
		str[now++] = x%2;
		x /= 2;
	}
	for ( int i=length-1; i>=0; i-- ) {
		std::cout << str[i];
	} std::cout << "\n";
}

} // end namespace
#endif
