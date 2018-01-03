# Makefile dilemme

cc=gcc
CFLAGS=-std=c99 -o

dilemme1.c:
	$(CC) $(CFLAGS) $@ dilemme1

dilemme2.c:
	$(CC) $(CFLAGS) $@ dilemme2
