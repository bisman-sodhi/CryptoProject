.PHONY: clean


main: main.cpp
	g++ -g -Wall -std=c++11 -o main main.cpp -I/usr/lib64/ -I/usr/local/lib64/python3.8/site-packages/tensorflow/include/external/boringssl/src/include/ -l:libcrypto.so.1.1

clean: 
	rm -f *.o