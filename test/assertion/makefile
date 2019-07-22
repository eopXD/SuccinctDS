CPLUSPLUS=g++ -std=c++17 -O3 -Wall -I../..
C=gcc -std=C99 -O2

all:
	$(CPLUSPLUS) bv_lookup.cpp -o test.bv_lookup
	$(CPLUSPLUS) wt_bv_naive.cpp -o test.wt_bv_naive
	$(CPLUSPLUS) wt_bv_lookup.cpp -o test.wt_bv_lookup
	$(CPLUSPLUS) stream.cpp -o test.stream
bv_lookup:
	$(CPLUSPLUS) bv_lookup.cpp -o test.bv_lookup
wt_bv_naive:
	$(CPLUSPLUS) wt_bv_naive.cpp -o test.wt_bv_naive
wt_bv_lookup:
	$(CPLUSPLUS) wt_bv_lookup.cpp -o test.wt_bv_lookup
stream:
	$(CPLUSPLUS) stream.cpp -o test.stream

clean:
	rm test.*