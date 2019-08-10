C=gcc -std=C99 -O2
CPLUSPLUS=g++ -std=c++17 -O3 -Wall
SUCCINCT=-I../../..

# this is configuration on linux5.csie (./install from sdsl-2.1.1)
SDSL_L=-I/tmp2/b04705001/include -L/tmp2/b04705001/lib 
SDSL_R=-lsdsl -ldivsufsort -ldivsufsort64

all:
	$(CPLUSPLUS) $(SUCCINCT) suc.cpp -o test.suc

suc: 
	$(CPLUSPLUS) $(SUCCINCT) suc.cpp -o test.suc
clean:
	rm tset.*
