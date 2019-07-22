// Copyright (C) 2019 Yueh-Ting Chen (eopXD)
/*! \file util.hpp
    \brief util.hpp contains functions for convenience
	\author Yueh-Ting Chen (eopXD)
	\project with Professor Tsan-sheng Hsu
*/


#include <ctime>
#include <cmath>
#include <string>
#include <fstream>

std::streamsize get_size ( std::string filename ) {
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	return (file.tellg());
}

std::ifstream* start_stream( std::string filename ) {
	return (new std::ifstream(filename, std::ios::binary));
}

double spent_time ( clock_t stamp ) {
	clock_t cost = clock() - stamp;
	return ((double)cost/CLOCKS_PER_SEC);
}