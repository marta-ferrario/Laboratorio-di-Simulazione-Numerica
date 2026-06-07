CC = g++
CFLAGS = -Wall -O3 --std=c++11

esercizio1.3.exe : esercizio1.3.o ../../Generatore_random/random.o
	$(CC) ../../Generatore_random/random.o esercizio1.3.o -o esercizio1.3.exe
esercizio1.3.o : esercizio1.3.cpp
	$(CC) -c esercizio1.3.cpp -o esercizio1.3.o $(CFLAGS)
random.o : ../../Generatore_random/random.cpp ../../Generatore_random/random.h
	$(CC) -c ../../Generatore_random/random.cpp -o ../../Generatore_random/random.o $(CFLAGS)
clean :
	rm *.o esercizio1.3.exe ../../Generatore_random/seed.out
