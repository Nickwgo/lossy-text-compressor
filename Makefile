CC = cc
ltc : main.o compress.o decompress.o help.o
	${CC} -oltc -Os main.o compress.o decompress.o help.o

main.o : ./src/main.c ./src/compress.h ./src/decompress.h ./src/help.h
	${CC} -c -Os ./src/main.c
compress.o : ./src/compress.c ./src/compress.h
	${CC} -c -Os ./src/compress.c
decompress.o : ./src/decompress.c ./src/decompress.h
	${CC} -c -Os ./src/decompress.c
help.o : ./src/help.c ./src/help.h
	${CC} -c -Os ./src/help.c

# There must be an easier way of doing that

clean :
	rm ltc main.o compress.o decompress.o help.o
