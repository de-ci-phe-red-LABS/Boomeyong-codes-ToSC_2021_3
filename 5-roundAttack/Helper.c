#include "AES_common.c"
#include <stdlib.h>

word4 randomNibble() {
	int a = rand();
	a = a % 16;
	return (word4) a;
}


void swapInverseDiagonal(word4 p1[4][4], word4 p2[4][4], int diaNum) {
	int colNum = diaNum + 4;
	for (int rowNum = 0; rowNum < 4; ++rowNum) {
		word4 temp = p1[rowNum][colNum % 4];
		p1[rowNum][colNum % 4] = p2[rowNum][colNum % 4];
		p2[rowNum][colNum % 4] = temp;
		colNum--;
	}
}

void getDiffStates(word4 p1[4][4], word4 p2[4][4], word4 diffStates[4][4]) {

	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			diffStates[row][col] = p1[row][col] ^ p2[row][col];
		}
	}
}

int isDiaInActive(word4 p1[4][4], word4 p2[4][4], int diaNum) {
	int colNum = diaNum;
	for (int rowNum = 0; rowNum < 4; ++rowNum) {
		if (p1[rowNum][colNum % 4] != p2[rowNum][colNum % 4]) {
			return 0;
		}
		colNum++;
	}
	return 1;
}


int isEqualInvDiagonal(word4 p1[4][4], word4 p2[4][4], int diaNum) {
	int colNum = diaNum + 4;
	int colCount = 0;
	for (int rowNum = 0; rowNum < 4; ++rowNum) {
		if (p1[rowNum][colNum % 4] == p2[rowNum][colNum % 4]) {
			colCount++;
		}
		colNum--;
	}
	if (colCount == 4) {
		return 1;
	} else
		return 0;
}
