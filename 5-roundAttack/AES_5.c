#include "Helper.c"
#include <math.h>
#include "Constants.c"

void Attack_5() {
	int numPlaintexts=2896, diffVal;
	word4 plaintexts[numPlaintexts][4][4], ciphertexts[numPlaintexts][4][4], c1[4][4], c2[4][4];
	word4 initKey[4][4], key[4][4][N_Round + 1];
	for (int rowNum = 0; rowNum < 4; ++rowNum) {
		for (int colNum = 0; colNum < 4; ++colNum) {
			initKey[rowNum][colNum] = randomNibble();
		}
	}
	//printf("Key\n");
	//printtt(initKey);
	initialization2(&(key[0][0][0]), initKey);
	for (int i = 1; i < N_Round + 1; i++) {
		generationRoundKey(&(key[0][0][i]), i, &(key[0][0][i - 1]));
	}
	for(int var=0;var<numPlaintexts;var++){
		int val=var;
		for(int i=0;i<4;i++){
			plaintexts[var][i][(diaNum+i)%4]=val%15;
			val/=15;	
		}
		encryption(plaintexts[var], key, ciphertexts[var]);
	}
	for (int indexI = 0; indexI < numPlaintexts - 1; ++indexI) {
		for (int indexJ = indexI+1; indexJ < numPlaintexts; ++indexJ) {
			for (int rowNum = 0; rowNum < 4; ++rowNum) {
				for (int colNum = 0; colNum < 4; ++colNum) {
					c1[rowNum][colNum] =ciphertexts[indexI][rowNum][colNum];
					c2[rowNum][colNum] =ciphertexts[indexJ][rowNum][colNum];
				}
			}
			if(isEqualInvDiagonal(c1, c2,SwapInvDiaNum)){
				continue;
			}
			swapInverseDiagonal(c1, c2, SwapInvDiaNum);
			decryption(c1, key, c1);
			decryption(c2, key, c2);
			int isRightPair=1;
			for(int k=0;k<4;k++){
				if(k==diaNum) continue;
				if(isDiaInActive(c1, c2, k)==0){
					isRightPair=0;
					break;	
				}
			}
			if(isRightPair){
				printf("Distinguisher Found\n");
				word4 diffState[4][4];
				printf("\nDifference Between Final Pair of States\n");
				getDiffStates(c1, c2, diffState);
				printtt(diffState);
				printf("\nDifference Between Initial Pair of States\n");
				getDiffStates(plaintexts[indexI], plaintexts[indexJ], diffState);
				printtt(diffState);
				printf("\nDifference Between Ciphertexts\n");
				getDiffStates(ciphertexts[indexI], ciphertexts[indexJ], diffState);
				printtt(diffState);
				return ;
			}
		}
	}
	printf("Distinguisher Not Found\n");						
}
