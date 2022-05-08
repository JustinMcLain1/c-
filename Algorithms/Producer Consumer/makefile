.DEFAULT_GOAL := Assign4

Assign4.o: Assign4.cpp
	g++ -c Assign4.cpp -Wall -std=c++11

Assign4: Assign4.o
	g++ -pthread -o Assign4 Assign4.o -Wall -std=c++11

clean:
	rm *.o Assign4
