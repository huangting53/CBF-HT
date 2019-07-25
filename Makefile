CPP=g++
CFLAGS = -g -w -std=c++11 -fpermissive -O3 $(INCLUDE)

# LIBS = -lghthash
fib:main.cpp PrefixGenerator.cpp city-hash.cpp hashFuncs.cpp BloomFilter.cpp hashtable.cpp BinarySearch.cpp CBF-HT.cpp
	${CPP} ${CFLAGS} -o fib main.cpp PrefixGenerator.cpp city-hash.cpp hashFuncs.cpp BloomFilter.cpp hashtable.cpp BinarySearch.cpp CBF-HT.cpp

all:fib
clean:
	rm fib
