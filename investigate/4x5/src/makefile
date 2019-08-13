C=gcc -std=C99 -O2
CPLUSPLUS=g++ -std=c++17 -O3 -Wall
SUCCINCT=-I../../..

decode:
	$(CPLUSPLUS) $(SUCCINCT) decode.cpp -o test.decode
try_compress:
	$(CPLUSPLUS) $(SUCCINCT) try_compress.cpp -o test.try_compress

all:
	$(CPLUSPLUS) $(SUCCINCT) decode.cpp -o test.decode
	$(CPLUSPLUS) $(SUCCINCT) try_compress.cpp -o test.try_compress
	
clean:
	rm test.*
