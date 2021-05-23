all: Game.o main.o
	g++ -g -o a Game.o main.o  -lglut -lGL -lGLU -lm

Game.o: Game.h Game.cpp 
	g++ -g -c Game.h Game.cpp
	
main.o: main.cpp
	g++ -g -c main.cpp 