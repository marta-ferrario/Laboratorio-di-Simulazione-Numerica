CC = g++
CFLAGS = -Wall -O3 --std=c++11

esercizio8.1.exe : esercizio8.1.o ../../Generatore_random/random.o
	$(CC) ../../Generatore_random/random.o esercizio8.1.o -o esercizio8.1.exe
esercizio8.1.o : esercizio8.1.cpp
	$(CC) -c esercizio8.1.cpp -o esercizio8.1.o $(CFLAGS)
random.o : ../../Generatore_random/random.cpp ../../Generatore_random/random.h
	$(CC) -c ../../Generatore_random/random.cpp -o ../../Generatore_random/random.o $(CFLAGS)
clean :
	rm *.o esercizio8.1.exe ../../Generatore_random/seed.out
