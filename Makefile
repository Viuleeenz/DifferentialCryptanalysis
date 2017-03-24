#CFLAGS=-c -std=c99 -g -VERBOSE -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib
CFLAGS=-c -std=c99 -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib
LDFLAGS= -ldl -lm
OBJ=encryptionGame.o differential_attack.o
EXE=main

all: ${EXE}

main: ${OBJ} main.o 
	gcc ${OBJ} main.o -o $@ ${LDFLAGS} 

clean:  
	rm -f ${OBJ} ${EXE} main.o encryptionGame.o differential_attack.o 
	rm -f CPA



