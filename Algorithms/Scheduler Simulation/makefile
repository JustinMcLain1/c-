CCFLAGS = -Wall -std=c++11

Assign3: Assign3.o Process.o
	g++ $(CCFLAGS) -o Assign3 Assign3.o Process.o

Assign3.o: Assign3.cpp Assign3.h Process.h
	g++ $(CCFLAGS) -c Assign3.cpp

Process.o: Process.cpp Process.h
	g++ $(CCFLAGS) -c Process.cpp

Process.h: Assign3.h

clean:
	-rm *.o Assign3
