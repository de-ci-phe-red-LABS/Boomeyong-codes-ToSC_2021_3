//

#include <stdio.h>
#include <stdlib.h>

#include "MultTables.c"

#define N_Round 5
//S-box
const unsigned char sBox[16] = { 0x6, 0xB, 0x5, 0x4, 0x2, 0xE, 0x7, 0xA, 0x9,
		0xD, 0xF, 0xC, 0x3, 0x1, 0x0, 0x8 };

//Inverse S-box
const unsigned char inv_s[16] = { 0xE, 0xD, 0x4, 0xC, 0x3, 0x2, 0x0, 0x6, 0xF,
		0x8, 0x7, 0x1, 0xB, 0x9, 0x5, 0xA };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*initialization state*/

void initialization(word4 *p, const word4 initialMessage[4][4]) {

	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			*(p + j + 4 * i) = initialMessage[i][j];
		}
	}

}

void initialization2(word4 *p, const word4 initialMessage[4][4]) {

	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			*(p + (N_Round + 1) * j + (N_Round + 1) * 4 * i) =
					initialMessage[i][j];
		}
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*print the state*/

void printtt(const word4 file[4][4]) {

	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			printf("0x%0X, ", file[i][j]);
		}
		printf("\n");
	}

}

void printForLatex(const word4 file[4][4]) {

	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			printf("0x%X & ", file[i][j]);
		}
		printf("\n");
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Byte sub transformation with S-box*/

word4 byteTransformation(word4 byte) {

	return sBox[byte];

}

/*Inverse byte sub transformation with Inverse S-box*/

word4 inverseByteTransformation(word4 byte) {

	return inv_s[byte];

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*ByteTransformation*/

void byteSubTransformation(word4 p[4][4]) {

	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++)
			p[i][j] = byteTransformation(p[i][j]);
	}
}

/*Inverse Byte Transformation*/

void inverseByteSubTransformation(word4 p[4][4]) {

	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++)
			p[i][j] = inverseByteTransformation(p[i][j]);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Generation Round key*/

void newColumn(word4 *pCol, int numRound) {
	word4 temp, rCon, colTemp[4];
	int i;
	temp = *pCol;
	for (i = 0; i < 3; i++) {
		colTemp[i] = *(pCol + i + 1);
	}
	colTemp[3] = temp;
	//S-box
	for (i = 0; i < 4; i++)
		colTemp[i] = byteTransformation(colTemp[i]);

	if (numRound == 0)
		rCon = 0x01;
	else {
		rCon = 0x02;
		for (i = 1; i < numRound; i++)
			rCon = multiplicationX(rCon);
	}
	colTemp[0] ^= rCon;
	for (i = 0; i < 4; i++) {
		*(pCol + i) = colTemp[i];
	}
}

void generationRoundKey(word4 *pKey, int numRound, word4 *pKeyPrecedente) {

	int i, j;

	word4 colTemp[4];

	numRound--;
	for (i = 0; i < 4; i++)
		colTemp[i] = *(pKeyPrecedente + 3 * (N_Round + 1)
				+ 4 * i * (N_Round + 1));

	newColumn(&(colTemp[0]), numRound);

	for (i = 0; i < 4; i++)
		*(pKey + 4 * (N_Round + 1) * i) = *(pKeyPrecedente
				+ 4 * (N_Round + 1) * i) ^ colTemp[i];

	for (i = 1; i < 4; i++) {

		for (j = 0; j < 4; j++) {
			*(pKey + i * (N_Round + 1) + 4 * (1 + N_Round) * j) =
					*(pKeyPrecedente + i * (N_Round + 1) + 4 * (1 + N_Round) * j)
							^ *(pKey + (i - 1) * (1 + N_Round)
									+ 4 * (1 + N_Round) * j);
		}

	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*shift rows*/

void shiftRows(word4 p[4][4]) {

	word4 temp[3];
	int i, j;

	for (i = 1; i < 4; i++) {
		for (j = 0; j < i; j++)
			temp[j] = p[i][j];

		for (j = 0; j < (4 - i); j++)
			p[i][j] = p[i][j + i];

		for (j = (4 - i); j < 4; j++)
			p[i][j] = temp[j - 4 + i];
	}

}

/*inverse shift rows*/

void inverseShiftRows(word4 p[4][4]) {

	word4 temp[3];
	int i, j;

	for (i = 1; i < 4; i++) {
		for (j = 3; j > (3 - i); j--)
			temp[j - 1] = p[i][j];

		for (j = 3; j > i - 1; j--)
			p[i][j] = p[i][j - i];

		for (j = 0; j < i; j++)
			p[i][j] = temp[3 - i + j];
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*MixColumn*/

void mixColumn(word4 p[4][4]) {

	int i, j;
	word4 newCol[3];

	for (i = 0; i < 4; i++) {
		newCol[0] = mult_2X[p[0][i]] ^ mult_3X[p[1][i]] ^ p[2][i] ^ p[3][i];
		newCol[1] = p[0][i] ^ mult_2X[p[1][i]] ^ mult_3X[p[2][i]] ^ p[3][i];
		newCol[2] = p[0][i] ^ p[1][i] ^ mult_2X[p[2][i]] ^ mult_3X[p[3][i]];
		p[3][i] = mult_3X[p[0][i]] ^ p[1][i] ^ p[2][i] ^ mult_2X[p[3][i]];

		for (j = 0; j < 3; j++) {
			p[j][i] = newCol[j];
		}
	}
}

/*inverse MixColumn*/

void inverseMixColumn(word4 p[4][4]) {

	int i, j;
	word4 newCol[4];

	for (i = 0; i < 4; i++) {

		newCol[0] = mult_EX_BX_DX_9X[p[0][i] * 16 * 16 * 16 + p[1][i] * 16 * 16
				+ p[2][i] * 16 + p[3][i]]; //mult_EX_BX[p[0][i]][p[1][i]] ^ mult_DX_9X[p[2][i]][p[3][i]];

		newCol[1] = mult_EX_BX_DX_9X[p[1][i] * 16 * 16 * 16 + p[2][i] * 16 * 16
				+ p[3][i] * 16 + p[0][i]]; //mult_EX_BX[p[1][i]][p[2][i]] ^ mult_DX_9X[p[3][i]][p[0][i]];

		newCol[2] = mult_EX_BX_DX_9X[p[2][i] * 16 * 16 * 16 + p[3][i] * 16 * 16
				+ p[0][i] * 16 + p[1][i]]; //mult_DX_9X[p[0][i]][p[1][i]] ^ mult_EX_BX[p[2][i]][p[3][i]];

		p[3][i] = mult_EX_BX_DX_9X[p[3][i] * 16 * 16 * 16 + p[0][i] * 16 * 16
				+ p[1][i] * 16 + p[2][i]]; //mult_DX_9X[p[1][i]][p[2][i]] ^ mult_EX_BX[p[3][i]][p[0][i]];
		for (j = 0; j < 3; j++) {
			p[j][i] = newCol[j];
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Add Round Key*/

void addRoundKey(word4 p[4][4], word4 key[][4][N_Round + 1], int costante) {

	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			p[i][j] ^= key[i][j][costante];
		}
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Encryption*/

void encryption(const word4 initialMessage[][4], word4 key[4][4][N_Round + 1],
		word4 ciphertext[4][4]) {

	int i, j;

	//initialization state
	word4 state[4][4];
	initialization(&(state[0][0]), initialMessage);

	//Initial Round
	addRoundKey(state, key, 0);

	//Round
	for (i = 0; i < N_Round - 1; i++) {
		byteSubTransformation(state);
		shiftRows(state);
		mixColumn(state);
		addRoundKey(state, key, i + 1);

	}

	//Final Round
	byteSubTransformation(state);
	shiftRows(state);
	addRoundKey(state, key, N_Round);

	//store the ciphertext
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++)
			ciphertext[i][j] = state[i][j];
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Decryption*/

void decryption(const word4 initialMessage[][4], word4 key[4][4][N_Round + 1],
		word4 plaintext[4][4]) {

	int i, j;

	//initialization state
	word4 state[4][4];
	initialization(&(state[0][0]), initialMessage);

	//Initial Round
	addRoundKey(state, key, N_Round);

	//Round
	for (i = N_Round - 1; i > 0; i--) {
		inverseByteSubTransformation(state);
		inverseShiftRows(state);
		addRoundKey(state, key, i);
		inverseMixColumn(state);

	}

	//Final Round
	inverseByteSubTransformation(state);
	inverseShiftRows(state);
	addRoundKey(state, key, 0);

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++)
			plaintext[i][j] = state[i][j];
	}

}
