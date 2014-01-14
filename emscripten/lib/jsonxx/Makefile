CXXFLAGS=-Werror -Wall -g

jsonxx_test: jsonxx_test.cc jsonxx.o

jsonxx.o: jsonxx.h jsonxx.cc

test: jsonxx_test
	./jsonxx_test

.PHONY: clean
clean:
	rm -f jsonxx_test *.o *~
