#include "differential-cryptanalysis.h"

int main(int argc, const char * argv[]) {
    
    char *plaintext_1=malloc(2);
    char *plaintext_2=malloc(2);
    char *ciphertext_1=malloc(2);
    char *ciphertext_2=malloc(2);
    char *c_plaintext=malloc(2);
    char *indiff = malloc(2);

    //set del differenziale
    indiff[0]=0x0B;
    indiff[1]=0x00;
    
    int i;
    int count;
    int fd;
    
    //AES_KEY ekey, dkey;
    
    /*char key[]={0x00,0x01,0x02,0x03,
     0x04,0x05,0x06,0x08,
     0x14,0x15,0x16,0x17,
     0xa4,0xa5,0xa6,0xa7};*/
    
    char key[10];
    time_t t;
    srand((unsigned) time(&t)); //Intializes random number generator
    for(i = 0; i < 10; i++)
        key[i] = rand() % 128;
    
    /*
     AES_set_encrypt_key(key, KeyLength, &ekey);
     AES_set_decrypt_key(key, KeyLength, &dkey);
     */
   
    fd = open("CPA", O_CREAT|O_RDWR|O_TRUNC);
    unsigned int seedp = 0;
    for(count = 0; count < 100000; count++) {

        plaintext_1[0] = rand() % 256;
        plaintext_1[1] = rand() % 256;

        plaintext_2[0] = plaintext_1[0]^indiff[0];
        plaintext_2[1] = plaintext_1[1]^indiff[1];

        encryption_Game(plaintext_1,ciphertext_1,key);
        encryption_Game(plaintext_2,ciphertext_2,key);
       
        
        write(fd, ciphertext_1, 2);
        write(fd, ciphertext_2, 2);
        write(fd, " ", 1);

    }	
    fsync(fd);
    
    printf("LAST KEY : %02x %02x\n", key[8], key[9]);
    printf("\n");
    differential_attack(fd);
    
}


