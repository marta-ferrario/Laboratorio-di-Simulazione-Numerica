CC = g++
CFLAGS = -Wall -O3 --std=c++11

main.exe : main.o City.o Individual.o Population.o GeneticAlgorithm.o ../../Generatore_random/random.o
	$(CC) ../../Generatore_random/random.o main.o City.o Individual.o Population.o GeneticAlgorithm.o -o main.exe

main.o : main.cpp
	$(CC) -c main.cpp -o main.o $(CFLAGS)

City.o : City.cpp City.h
	$(CC) -c City.cpp -o City.o $(CFLAGS)

Individual.o : Individual.cpp Individual.h
	$(CC) -c Individual.cpp -o Individual.o $(CFLAGS)

Population.o : Population.cpp Population.h
	$(CC) -c Population.cpp -o Population.o $(CFLAGS)

GeneticAlgorithm.o : GeneticAlgorithm.cpp GeneticAlgorithm.h
	$(CC) -c GeneticAlgorithm.cpp -o GeneticAlgorithm.o $(CFLAGS)

random.o : ../../Generatore_random/random.cpp ../../Generatore_random/random.h
	$(CC) -c ../../Generatore_random/random.cpp -o ../../Generatore_random/random.o $(CFLAGS)

clean :
	rm *.o main.exe ../../Generatore_random/seed.out