#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <openssl/aes.h>


#define KeyLength 			16
#define BlockSize 			2 		// in bytes
#define NumRounds 			4
#define KnownPlaintextNum 	100000 //10000
#define sboxSize 256

int getbits(char, int, int);
void exchange (char*, int, int, char*);
void encryption_Game(char*,char*, char*);
void differential_attack(int);
