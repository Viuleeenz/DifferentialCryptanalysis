#include "boxes_references.h"
#include "differential-cryptanalysis.h"


/*
	fd = file descriptor of plaintext-ciphertext samples to use in the attack
 */


void differential_attack(int fd)
{

    int subkey,j;
    char u1;
    char u2;
    char u3;
    char u4;
    char deltaU;
    char deltaV=0x66;
    float count[256];
    int i=0;
    char* ciphertext_1=malloc(BlockSize);
    char* ciphertext_2=malloc(BlockSize);

    memset(count, 0, 256*sizeof(float));
    lseek(fd, 0, SEEK_SET);

    while (read(fd, ciphertext_1, BlockSize) > 0 && read(fd, ciphertext_2,BlockSize) > 0 ) {
          
          lseek(fd, 1, SEEK_CUR);
    
           for( subkey = 0 ; subkey < 256 ; subkey++){

            u1 = sinv[(ciphertext_1[0] & 0xF0) >> 4 ^ (subkey)] << 4;
            u1 ^= sinv[(ciphertext_1[1] & 0xF0) >> 4 ^ (subkey & 0xF0 )>> 4];
              
            u2 = sinv[(ciphertext_2[0] & 0xF0) >> 4 ^ (subkey)] << 4;
            u2 ^= sinv[(ciphertext_2[1] & 0xF0) >> 4 ^ (subkey & 0xF0) >> 4];
      
           deltaU = u1^u2; 

           if(deltaU == 0x00)
           {

          
            u1 = sinv[(ciphertext_1[0] & 0x0F) ^ (subkey >> 4)] << 4;
            u1 ^= sinv[(ciphertext_1[1] & 0x0F) ^ (subkey & 0x0F)];
              
            u2 = sinv[(ciphertext_2[0] & 0x0F) ^ (subkey >> 4 )] << 4;
            u2 ^= sinv[(ciphertext_2[1] & 0x0F) ^ (subkey & 0x0F)];
     
            deltaU = u1^u2; 

    

            if(deltaU == deltaV)
                  count[subkey]++;
            
            } // end deltaU == 0x00
            } //end for
            } // end while

    int index =  0;
    float max = (count[1]/10000); 
      for(i = 0; i < 256; i++){
        if( i %5 == 0) printf("\n");
        if( max < (count[i]/10000))
        {
          max = (count[i]/10000);
          index = i ;
        }
      printf("%f %02x ",count[i]/1000000, i );      
          }
      

    printf("\n SUBKEY FOUND!  %02x\n" , index);
    }
