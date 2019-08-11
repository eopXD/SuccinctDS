C=gcc -std=C99 -O2
CPLUSPLUS=g++ -std=c++17 -O3 -Wall
SUCCINCT=-I../../..

decode:
	$(CPLUSPLUS) $(SUCCINCT) decode.cpp -o test.decode

all:
	$(CPLUSPLUS) $(SUCCINCT) decode.cpp -o test.decode
	
clean:
	rm test.*
