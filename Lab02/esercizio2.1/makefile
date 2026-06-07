CC = g++
CFLAGS = -Wall -O3 --std=c++11

esercizio2.1.exe : esercizio2.1.o ../../Generatore_random/random.o
	$(CC) ../../Generatore_random/random.o esercizio2.1.o -o esercizio2.1.exe
esercizio2.1.o : esercizio2.1.cpp
	$(CC) -c esercizio2.1.cpp -o esercizio2.1.o $(CFLAGS)
random.o : ../../Generatore_random/random.cpp ../../Generatore_random/random.h
	$(CC) -c ../../Generatore_random/random.cpp -o ../../Generatore_random/random.o $(CFLAGS)
clean :
	rm *.o esercizio2.1.exe ../../Generatore_random/seed.out
