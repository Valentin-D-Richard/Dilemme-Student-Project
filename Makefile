# Makefile dilemme

CC=gcc
CFLAGS=-Wall -std=c99

# target: dépendances

all: dilemme1 dilemme2 client server

dilemme1 : dilemme1.c dilemmelib.h
	$(CC) $(CFLAGS) $< -o $@

dilemme2 : dilemme2.c dilemmelib.h
	$(CC) $(CFLAGS) $< -o $@ -lm

packets.o : packets.c packets.h
	$(CC) $(CFLAGS) -g $< -c -o $@

client : client.c packets.o dilemmelib.h
	$(CC) $(CFLAGS) packets.o $< -o $@ -lm

server : server.c packets.o dilemmelib.h
	$(CC) $(CFLAGS) packets.o $< -o $@ -lm

# $@ raccourci pour la target
# $^ raccourci pour toutes les dépendances
# $< raccourci pour la dépendance la plus à gauche
