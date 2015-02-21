/* Trivial RC5-like program: the encryption step is exactly RC5,
  but we use a manually entered key schedule.

 Dr. Orion Lawlor, lawlor@alaska.edu, 2015-01-20 (Public Domain)
*/
#include <cstdio>
#include <cstdlib>
#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>

// Define the round count
enum {n_rounds=12, n_keys=2*n_rounds+2};

// Circular bit rotate left: rotate v left by count bits
unsigned int ROTL(unsigned int v,unsigned int count)
{
	return (v<<count) | (v>>(32-count));
}

unsigned int ROTR(unsigned int v,unsigned int count)
{
	return (v>>count) | (v<<(32-count));
}

void decrypt(unsigned int &A,unsigned int &B,const unsigned int *keys)
{
	for (int round=n_rounds;round>=1;round--) {
		if (false)  // show progress (useful for debug)
			printf(" round %d: %08x %08x\n", round, A,B);
		
		B = ROTR(B,A) + keys[2*round+1];
		B^=A;
		A = ROTR(A,B) + keys[2*round];
		A^=B;
		
	A -= keys[0];
	B -= keys[1];
	}
}

// Mix two integers worth of data, A and B, with these keys.
void RC5_rounds(unsigned int &A,unsigned int &B,const unsigned int *keys) {
	A += keys[0];
	B += keys[1];
	for (int round=1;round<=n_rounds;round++) {
		if (false)  // show progress (useful for debug)
			printf(" round %d: %08x %08x\n", round, A,B);
		A^=B;
		A = ROTL(A,B) + keys[2*round];
		B^=A;
		B = ROTL(B,A) + keys[2*round+1];
	}
}

class inputs {
public:
			unsigned int A;
			unsigned int B;
		};


char dec[9] = "xxxxxxxx";

// Encrypt the command-line arguments
int main(int argc,char *argv[]) {
	/*
	if (argc<=1) {
		printf("Usage: rc5 '<plaintext>' 0x<key0> 0x<key1> 0x<key2> ...\n");
		exit(1);
	}
	*/
	unsigned int keys[n_keys];
	for (int k=0;k<n_keys;k++) keys[k]=0; // add real key schedule here
	// Key schedule is passed as hex command line arguments, after plaintext
	// for (int argi=2;argi<argc;argi++) sscanf(argv[argi],"%x",&keys[argi-2]);
	
	// First command line argument is the string to encrypt.
	//const char *plaintext=argv[1];
	//while (strlen(plaintext)>0) {
		// A and B are the state for the cipher.
		
		
		// Copy bytes of the string into the bytes of A, then B.
		//   In a language without memcpy, you'd use something like:
		//     A = 0, B=0;
		//     A = A|(plaintext[0]<<0);
		//     A = A|(plaintext[1]<<8);
		//     A = A|(plaintext[2]<<16);
		//     A = A|(plaintext[3]<<24);
		//     B = B|(plaintext[4]<<0); ... and so on for 8 bytes ...
		//memcpy(&AB,plaintext,std::min(sizeof(AB),strlen(plaintext)));
		inputs AB = {0, 0};	
		AB.A = 0x7d9a30d7;
		AB.B = 0x61fa8a11;

		decrypt(AB.A,AB.B,keys);

		memcpy(dec, &AB, std::min(sizeof(AB),strlen(dec)));
		//RC5_rounds(AB.A,AB.B,keys);
		
		// Dump encrypted hex integers
		printf("0x%08x 0x%08x\n",AB.A,AB.B);
		printf("%s", dec);
		/*
		if (strlen(plaintext)<=sizeof(AB)) break;
		else plaintext+=sizeof(AB);
		*/
	}



