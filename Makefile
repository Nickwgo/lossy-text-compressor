CC=gcc

# I don't know how makefiles work for other operating systems.
# If you are not using a Unix-like system that supports the 'make' command,
# then you're on your own.

lts: main.c
	$(CC) main.c -oltc -Os
