#include "Helper.c"
#include <math.h>
#include "Constants.c"

void genKeyCan(int candNum, word4 key[4]) {
	for (int i = 0; i < 4; ++i) {
		key[i] = (word4) candNum % 16;
		candNum /= 16;
	}
}

int check_5_RoundProperty(word4 p1[4][4], word4 p2[4][4],
		word4 key[4][4][N_Round + 1]) {
	word4 tp1[4][4], tp2[4][4];
	for (int rowNum = 0; rowNum < 4; ++rowNum) {
		for (int colNum = 0; colNum < 4; ++colNum) {
			tp1[rowNum][colNum] = p1[rowNum][colNum];
			tp2[rowNum][colNum] = p2[rowNum][colNum];
		}
	}
	addRoundKey(tp1, key, N_Round);
	inverseByteSubTransformation(tp1);
	inverseShiftRows(tp1);
	addRoundKey(tp1, key, N_Round - 1);
	inverseMixColumn(tp1);

	addRoundKey(tp2, key, N_Round);
	inverseByteSubTransformation(tp2);
	inverseShiftRows(tp2);
	addRoundKey(tp2, key, N_Round - 1);
	inverseMixColumn(tp2);

	if ((tp1[0][3] != tp2[0][3]) && (tp1[1][3] == tp2[1][3])
			&& (tp1[2][3] == tp2[2][3]) && (tp1[3][3] == tp2[3][3])
			&& (tp1[1][2] == tp2[1][2]) && (tp1[2][1] == tp2[2][1])
			&& (tp1[3][0] == tp2[3][0])) {
		return 1;
	} else if ((tp1[0][3] == tp2[0][3]) && (tp1[1][3] != tp2[1][3])
			&& (tp1[2][3] == tp2[2][3]) && (tp1[3][3] == tp2[3][3])
			&& (tp1[2][2] == tp2[2][2]) && (tp1[3][1] == tp2[3][1])
			&& (tp1[0][0] == tp2[0][0])) {
		return 1;
	} else if ((tp1[0][3] == tp2[0][3]) && (tp1[1][3] == tp2[1][3])
			&& (tp1[2][3] != tp2[2][3]) && (tp1[3][3] == tp2[3][3])
			&& (tp1[3][2] == tp2[3][2]) && (tp1[0][1] == tp2[0][1])
			&& (tp1[1][0] == tp2[1][0])) {
		return 1;
	} else if ((tp1[0][3] == tp2[0][3]) && (tp1[1][3] == tp2[1][3])
			&& (tp1[2][3] == tp2[2][3]) && (tp1[3][3] != tp2[3][3])
			&& (tp1[0][2] == tp2[0][2]) && (tp1[1][1] == tp2[1][1])
			&& (tp1[2][0] == tp2[2][0])) {
		return 1;
	} else
		return 0;
}

int checkOneRoundInside_ENC(word4 p1[4][4], word4 p2[4][4],
		word4 key[4][4][N_Round + 1]) {
	word4 tp1[4][4], tp2[4][4];
	for (int rowNum = 0; rowNum < 4; ++rowNum) {
		for (int colNum = 0; colNum < 4; ++colNum) {
			tp1[rowNum][colNum] = p1[rowNum][colNum];
			tp2[rowNum][colNum] = p2[rowNum][colNum];
		}
	}
	addRoundKey(tp1, key, 0);
	byteSubTransformation(tp1);
	shiftRows(tp1);
	mixColumn(tp1);

	addRoundKey(tp2, key, 0);
	byteSubTransformation(tp2);
	shiftRows(tp2);
	mixColumn(tp2);
	int colCount = 0;
	for (int rowNum = 0; rowNum < 4; ++rowNum) {
		if (tp1[rowNum][diaNum] == tp2[rowNum][diaNum]) {
			colCount++;
		}
	}
	if (colCount < 3) {
		return 0;
	} else
		return 1;
}

void *Attack_6_check(void *threadNum) {
	double pairs_per_thread = pow(2, Pairs - log2(Num_Threads));
	double pairs_per_structure = pow(2, 31);
	word4 plaintexts[65536][4][4], ciphertexts[65536][4][4], c1[4][4], c2[4][4],keyCan[4];
	int numPlaintexts, diffVal,index_p1_p2,limit=pow(2,16),index_p3_p4;
	int tempFlag=0;
	//word4 tempP1[4], tempP2[4], tempP3[4], tempP4[4];
	word4 tpc3[4], tpc4[4], tInputDiff[4], diffStates[4][4],tpc1[4],tpc2[4],XorState_1c[4];
	do {
		if (pairs_per_thread < pairs_per_structure) {
			numPlaintexts = sqrt(pairs_per_thread) + 1;
		} else {
			numPlaintexts = 65536;
		}
		for (int row = 0; row < 4; ++row) {
			for (int col = 0; col < 4; ++col) {
				plaintexts[0][row][col] = randomNibble();
			}
		}
//		encryption(plaintexts[0], key, ciphertexts[0]);
		for (int var = 0; var < numPlaintexts; ++var) {
			for (int rowNum = 0; rowNum < 4; ++rowNum) {
				for (int colNum = 0; colNum < 4; ++colNum) {
					plaintexts[var][rowNum][colNum] =
							plaintexts[0][rowNum][colNum];
				}
			}
			diffVal = var;
			for (int rowIndex = 0; rowIndex < 4; ++rowIndex) {
				plaintexts[var][rowIndex][(diaNum + rowIndex) % 4] = (diffVal
						% 15);
				diffVal /= 15;
			}
			encryption(plaintexts[var], key, ciphertexts[var]);
		}
		for (int indexI = 0; indexI < numPlaintexts - 1; ++indexI) {
			for (int indexJ = indexI + 1; indexJ < numPlaintexts; ++indexJ) {
				if (isEqualInvDiagonal(ciphertexts[indexI], ciphertexts[indexJ],
				SwapInvDiaNum)) {
					continue;
				}
				if ((plaintexts[indexI][0][diaNum % 4]
						== plaintexts[indexJ][0][diaNum % 4])
						|| (plaintexts[indexI][1][(1 + diaNum) % 4]
								== plaintexts[indexJ][1][(1 + diaNum) % 4])
						|| (plaintexts[indexI][2][(2 + diaNum) % 4]
								== plaintexts[indexJ][2][(2 + diaNum) % 4])
						|| (plaintexts[indexI][3][(3 + diaNum) % 4]
								== plaintexts[indexJ][3][(3 + diaNum) % 4])) {
					continue;
				}
				/*if (!checkOneRoundInside_ENC(plaintexts[indexI],
						plaintexts[indexJ], key)) {
					//	printf("\nFound 4 TO 1\n");
					continue;
				}
				if (!check_5_RoundProperty(ciphertexts[indexI],
						ciphertexts[indexJ], key)) {
					continue;
				
				}*/
				for (int rowNum = 0; rowNum < 4; ++rowNum) {
					for (int colNum = 0; colNum < 4; ++colNum) {
						c1[rowNum][colNum] =
								ciphertexts[indexI][rowNum][colNum];
						c2[rowNum][colNum] =
								ciphertexts[indexJ][rowNum][colNum];
					}
				}
				swapInverseDiagonal(c1, c2, SwapInvDiaNum);
				decryption(c1, key, c1);
				decryption(c2, key, c2);

				if (isDiaInActive(c1, c2, diaNum)) {
					continue;
				}
				if (isDiaInActive(c1, c2, 0) || isDiaInActive(c1, c2, 1)
						|| isDiaInActive(c1, c2, 2)
						|| isDiaInActive(c1, c2, 3)) {
					count++;
					for (int canIndex = 0; canIndex < limit; ++canIndex) {

						genKeyCan(canIndex, keyCan);
						for (int rowIndex = 0; rowIndex < 4; ++rowIndex) {
							tpc1[rowIndex] = plaintexts[indexI][rowIndex][(diaNum + rowIndex) % 4]
									^ keyCan[rowIndex];
							tpc2[rowIndex] = plaintexts[indexJ][rowIndex][(diaNum + rowIndex) % 4]
									^ keyCan[rowIndex];
							tpc3[rowIndex] = c1[rowIndex][(diaNum + rowIndex) % 4]
									^ keyCan[rowIndex];
							tpc4[rowIndex] = c2[rowIndex][(diaNum + rowIndex) % 4]
									^ keyCan[rowIndex];
						}
						byteSubTransformation_1C(tpc1);
						byteSubTransformation_1C(tpc2);
						byteSubTransformation_1C(tpc3);
						byteSubTransformation_1C(tpc4);
						MC_1_Col(tpc1);
						MC_1_Col(tpc2);
						MC_1_Col(tpc3);
						MC_1_Col(tpc4);
						getXor_1C(tpc1, tpc2,XorState_1c);
						index_p1_p2 = getIndex_of_SingleActiveByte_1C(
								XorState_1c);
						getXor_1C(tpc3, tpc4,XorState_1c);
						index_p3_p4 = getIndex_of_SingleActiveByte_1C(
								XorState_1c);
						if ((index_p1_p2 != -1)
								&& (index_p1_p2
										== index_p3_p4)) {
							tempFlag=1;
							SuggestionCount[keyCan[0] + keyCan[1] * 16
									+ keyCan[2] * 256 + keyCan[3] * 4096].Value+=1;
						}
					}
				}
			}
		}

		pairs_per_thread -= pairs_per_structure;
	} while (pairs_per_thread > 0);
}
