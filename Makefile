all: test pacsolver

clean: 
	rm -f *.o pacsolver pac-test

test: pac-test
	./pac-test

pacsolver : pacsolver.o pacboard.o main.o
	$(LINK.cpp) $^ -o $@ 

pac-test : PacTest.o TestMain.o pacboard.o pacsolver.o 
	$(LINK.cpp) $^ /usr/local/lib/libcppunit.a -o $@ 