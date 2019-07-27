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

} // end namespace
#endif
