CC = g++
CFLAGS = -Wall -O3 --std=c++11
LIBOBJ = lib8.2.o random.o

all: esercizio8.2.exe ottimizzazione.exe

# =======================
# esercizio8.2
# =======================
esercizio8.2.exe : esercizio8.2.o $(LIBOBJ)
	$(CC) $^ -o $@

esercizio8.2.o : esercizio8.2.cpp lib8.2.h
	$(CC) $(CFLAGS) -c $< -o $@

# =======================
# ottimizzazione
# =======================
ottimizzazione.exe : ottimizzazione.o $(LIBOBJ)
	$(CC) $^ -o $@

ottimizzazione.o : ottimizzazione.cpp lib8.2.h
	$(CC) $(CFLAGS) -c $< -o $@

# =======================
# libreria comune
# =======================
lib8.2.o : lib8.2.cpp lib8.2.h
	$(CC) $(CFLAGS) -c lib8.2.cpp -o lib8.2.o

# random generator
random.o : ../../Generatore_random/random.cpp ../../Generatore_random/random.h
	$(CC) $(CFLAGS) -c ../../Generatore_random/random.cpp -o random.o

# =======================
# clean
# =======================
clean:
	rm -f *.o *.exe ../../Generatore_random/seed.out