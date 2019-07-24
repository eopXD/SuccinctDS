// C99
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
// C++
#include <iostream>
// eopxd
#include "include/wt.hpp"
// sdsl
#include "sdsl/wt_helper.hpp" // 2.1.1
#include "sdsl/wavelet_trees.hpp" // 2.1.1

using timer = std::chrono::high_resolution_clock;
using namespace std;
using namespace sdsl;
int main ()
{
	char filename[25] = "test_data.small";
	int bpa = 1;
	
	auto start = timer::now();
	eopxd::wt<eopxd::wt_huff<eopxd::bv_lookup>, eopxd::bv_lookup> 
		wt(filename, bpa, 1); 
	memory_monitor::stop();

	cout << "init\n";
	cout << "time: " <<chrono::duration_cast<chrono::milliseconds>
		(stop-start).count()/(double)1000 << "\n";


	return (0);
}
