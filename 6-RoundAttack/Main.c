#include "PreComputes.c"
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

void initSuggestionCount(){
	int val=0;
	for (int var = 0; var < 65536; ++var) {
		SuggestionCount[var].Value=0;
		val=var;
		for (int j = 0; j < 4; ++j) {
			SuggestionCount[var].canBytes[j]=val%16;
			val=val/16;
		}
	}
}

int main() {
	srand(time(0));
	initSuggestionCount();
	for (int rowNum = 0; rowNum < 4; ++rowNum) {
		for (int colNum = 0; colNum < 4; ++colNum) {
			initKey[rowNum][colNum] = randomNibble();
		}
	}
	printf("Key\n");
	printtt(initKey);
	printf("\n");
	initialization2(&(key[0][0][0]), initKey);
	for (int i = 1; i < N_Round + 1; i++) {
		generationRoundKey(&(key[0][0][i]), i, &(key[0][0][i - 1]));
	}
	int i;
	pthread_t tid;
	for (i = 0; i < Num_Threads; i++) {
		 int *arg = malloc(sizeof(*arg));
		*arg = i;
		pthread_create(&tid, NULL, Attack_6_check, arg);
		sleep(1);
	}

	pthread_join(tid, NULL);
	printf("Conforming Pairs: %d", count);
	printf("\nThread Complete\n");
	quickSortIterative(SuggestionCount, 0, 65535);
	int tempCount = 0;
	for (int var = 65535; var > 0; --var) {
		printf("\n0x%X 0x%X 0x%X 0x%X: %d\n", SuggestionCount[var].canBytes[0],
				SuggestionCount[var].canBytes[1],
				SuggestionCount[var].canBytes[2],
				SuggestionCount[var].canBytes[3], SuggestionCount[var].Value);
		tempCount++;
		if (SuggestionCount[var].Value <= 0) {
			break;
		}
	}
}
