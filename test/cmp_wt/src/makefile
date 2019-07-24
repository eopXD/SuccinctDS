C=gcc -std=C99 -O2
CPLUSPLUS=g++ -std=c++17 -O3 -Wall
SUCCINCT=-I../../..

# this is configuration on linux5.csie (./install from sdsl-2.1.1)
SDSL_L=-I/tmp2/b04705001/include -L/tmp2/b04705001/lib 
SDSL_R=-lsdsl -ldivsufsort -ldivsufsort64

gen:
	$(CPLUSPLUS) gen.cpp -o test.gen

mm: 
	$(CPLUSPLUS) $(SUCCINCT) $(SDSL_L) mm.cpp $(SDSL_R) -o test.mm

suc:
	$(CPLUSPLUS) $(SUCCINCT) suc_wt.cpp -o test.suc_wt

sdsl:
	$(CPLUSPLUS) $(SUCCINCT) $(SDSL_L) sdsl_wt.cpp $(SDSL_R) -o test.sdsl_wt

all:
	$(CPLUSPLUS) $(SUCCINCT) suc_wt.cpp -o test.suc_wt
	$(CPLUSPLUS) $(SUCCINCT) $(SDSL_L) sdsl_wt.cpp $(SDSL_R) -o test.sdsl_wt

clean: 
	rm test.*
