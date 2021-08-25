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

void getXor_1C(word4 p1[4], word4 p2[4], word4 np[4]) {
	for (int var = 0; var < 4; ++var) {
		np[var] = p1[var] ^ p2[var];
	}
}

int getIndex_of_SingleActiveByte_1C(word4 diff[4]) {
	int nZCount = 0, index;
	for (int var = 0; var < 4; ++var) {
		if (diff[var] != 0) {
			nZCount++;
			index = var;
		}
	}
	if (nZCount == 1) {
		return index;
	} else {
		return -1;
	}
}


void byteSubTransformation_1C(word4 p[4]) {
	int i;
	for (i = 0; i < 4; i++) {
		p[i] = byteTransformation(p[i]);
	}
}

void MC_1_Col(word4 p[4]) {
	word4 np[4];
	np[0] = mult_2X[p[0]] ^ mult_3X[p[1]] ^ p[2] ^ p[3];
	np[1] = p[0] ^ mult_2X[p[1]] ^ mult_3X[p[2]] ^ p[3];
	np[2] = p[0] ^ p[1] ^ mult_2X[p[2]] ^ mult_3X[p[3]];
	np[3] = mult_3X[p[0]] ^ p[1] ^ p[2] ^ mult_2X[p[3]];
	for (int j = 0; j < 4; j++) {
		p[j] = np[j];
	}
}
