CPLUSPLUS=g++ -std=c++17 -O3 -Wall -I../..
C=gcc -std=C99 -O2

all:
	$(CPLUSPLUS) bv_lookup.cpp -o test.bv_lookup
	$(CPLUSPLUS) wt_bv_naive.cpp -o test.wt_bv_naive
	$(CPLUSPLUS) wt_bv_lookup.cpp -o test.wt_bv_lookup
	$(CPLUSPLUS) stream.cpp -o test.stream
	$(CPLUSPLUS) mem_used_bv_lookup.cpp -o test.mem_used_bv_lookup
bv_lookup:
	$(CPLUSPLUS) bv_lookup.cpp -o test.bv_lookup
wt_bv_naive:
	$(CPLUSPLUS) wt_bv_naive.cpp -o test.wt_bv_naive
wt_bv_lookup:
	$(CPLUSPLUS) wt_bv_lookup.cpp -o test.wt_bv_lookup
stream:
	$(CPLUSPLUS) stream.cpp -o test.stream
mem_used_bv_lookup:
	$(CPLUSPLUS) mem_used_bv_lookup.cpp -o test.mem_used_bv_lookup
mem_used_wt:
	$(CPLUSPLUS) mem_used_wt.cpp -o test.mem_used_wt

clean:
	rm test.*
