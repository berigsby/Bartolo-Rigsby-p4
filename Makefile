output: shell.o eval.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors shell.o eval.o -o output

shell.o: shell.cpp
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -c shell.cpp

eval.o:eval.cpp eval.h
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -c eval.cpp

clean:
	rm *.o output