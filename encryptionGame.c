#include "differential-cryptanalysis.h"
#include "boxes_references.h"


/* getbits: return n bits from the position p */
int getbits(char x, int p, int n) {
  return ((int)x >> (7-p)) & ~(~0 << n);
} // bits are counted from left to right

/* prints the bits sequence given as input  */
void print_bits(int i)
{
  for ( int x= 0; x < 8 ; x++){
     printf("%d\t",(int)((i >> ( 7-x )) & 0x01)); 
    }
    printf("\n");
}

void exchange (char* chiphertext, int s/*char* s_box*/, int p /*char* permutation*/, char* t)
{
    int i;
    if (s > 7) i=1; else i=0;
    if( p > 7)
        //move bit s to position 7-p 
        t[1] = (int)t[1]^(int)(((0x01 & getbits(chiphertext[i],s%8,1)) << 7) >> (p%8) );
     
    else
        t[0] = (int)t[0]^(int)(((0x01 & getbits(chiphertext[i],s%8,1)) <<  7) >> (p%8) );
       
}

void encryption_Game(char * plaintext , char* ciphertext, char * key)
{
    char t0,t1,t2,t3; //input S-Boxes
    char* t = malloc(2);
    char* tmpchiper = malloc(2);    
    memcpy(tmpchiper, plaintext, 2);
    memset(ciphertext,0,2);

#ifdef VERBOSE
    printf("Plaintext\n");
    print_bits(plaintext[0]);
    print_bits(plaintext[1]);
#endif  

    int round,j,i;   
    for( round = 0 ; round < (NumRounds-1) ; round ++)
   {

#ifdef VERBOSE
    printf("\nround: %d\n", round );
    printf("\nKey\n");
#endif

    for (j=0; j<2; j++) {
      tmpchiper[j]=tmpchiper[j]^key[2*round+j];
    }

#ifdef VERBOSE    
    printf("\nCiphertext XOR Key\n");
    print_bits(tmpchiper[0]);
    print_bits(tmpchiper[1]);          
#endif

    // s-box takes in input 4 bit, tmpchiper has to be divided in 2 block of 4 bits.
    t0 = s[(tmpchiper[0]>> 4) & (0x0F)]<<4; // bit 0 to 3

#ifdef VERBOSE
    printf("\n output s-box 1\n");
    print_bits(t0);  
#endif

    t1 = s[tmpchiper[0] & 0x0F] ; // bit 4 to 7

#ifdef VERBOSE
    printf("\n output s-box 2\n");
    print_bits(t1);
#endif

    t2 = s[(tmpchiper[1] >> 4) & (0x0F)] <<4; // bit 8 to 11
  
#ifdef VERBOSE
    printf("\n output s-box 3\n");
    print_bits(t2);
#endif    

    t3 = s[tmpchiper[1] & (0x0F)]; // bit 12 to 15

#ifdef VERBOSE
  printf("\n output s-box 4\n");
  print_bits(t3);
#endif    

    t[0] = t0 ^ t1;
    t[1] = t2 ^ t3;
    
    memset(tmpchiper, 0, 2);

#ifdef VERBOSE
    printf("\noutput s-box\n");
    print_bits(t[0]);
    print_bits(t[1]);
#endif

   for (i=0; i<16; i++) {
       exchange(t, i, p[i], tmpchiper);
    }

#ifdef VERBOSE
   printf(" \n ... after the permutation is executed ... \n");
   print_bits(tmpchiper[0]);
   print_bits(tmpchiper[1]);
   printf("\n------------------------------\n");
#endif
    
   }
    
    //last round's key
    for(j = 0; j < 2; j++)
        tmpchiper[j] ^= key[2*round+j];
    
    t0 = s[(tmpchiper[0] >> 4) & 0x0F] << 4;
    t1 = s[tmpchiper[0] & 0x0F];
    t2 = s[(tmpchiper[1] >> 4) & 0x0F] << 4;
    t3 = s[tmpchiper[1] & 0x0F];
    ciphertext[0] = t0 ^ t1;
    ciphertext[1] = t2 ^ t3;
    
    //xor last round's ciphertext and the key
    round++;
    for(j = 0; j < 2; j++)
        ciphertext[j] ^=key[2*round+j];

  #ifdef VERBOSE
   printf("%s\n","last round key (5) " );
   print_bits(key[2*round]);
   print_bits(key[2*round+1]);
  #endif


}




