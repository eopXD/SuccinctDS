C=gcc -std=C99 -O2
CPLUSPLUS=g++ -std=c++17 -O3 -Wall
SUCCINCT=-I../../..

# this is configuration on linux5.csie (./install from sdsl-2.1.1)
SDSL_L=-I/tmp2/b04705001/include -L/tmp2/b04705001/lib 
SDSL_R=-lsdsl -ldivsufsort -ldivsufsort64

suc:
	$(CPLUSPLUS) $(SUCCINCT) suc_bv.cpp -o test.suc_bv
sdsl:
	$(CPLUSPLUS) $(SDSL_L) sdsl_bv.cpp $(SDSL_R) -o test.sdsl_bv

all:
	$(CPLUSPLUS) $(SDSL_L) sdsl_bv.cpp $(SDSL_R) -o test.sdsl_bv
	$(CPLUSPLUS) $(SUCCINCT) suc_bv.cpp -o test.suc_bv
clean: 
	rm test.*
