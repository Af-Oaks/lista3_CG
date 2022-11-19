CC = g++
OUTPUT=main
FLAGS=-lGL -lGLU -lglut -lm -lSOIL -lGLEW
#all:
#		$(CC) *.c */*.c -lGL -lGLU -lglut -lm -o main

compilar:
		$(CC) -g *.cpp */*.cpp $(FLAGS) -o $(OUTPUT)

executar: compilar
	./$(OUTPUT)

limpar:
		rm *.o -f
