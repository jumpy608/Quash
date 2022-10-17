quash: Main.o Executive.o
	g++ -std=c++17 -g -Wall Main.o Executive.o -o quash

Main.o: Main.cpp Executive.h
	g++ -std=c++17 -g -Wall -c Main.cpp

Executive.o: Executive.h Executive.cpp
	g++ -std=c++17 -g -Wall -c Executive.cpp 


clean: 
	rm *.o quash
