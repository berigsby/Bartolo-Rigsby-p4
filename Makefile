1730sh: shell.o eval.o jobs.o
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors jobs.o shell.o eval.o -o 1730sh

shell.o: shell.cpp
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -c shell.cpp

eval.o:eval.cpp eval.h
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -c eval.cpp

jobs.o:jobs.cpp jobs.h
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -c jobs.cpp

clean:
	rm *.o 1730sh 