typedef struct suggestionElem {
	int Value;
	word4 canBytes[4];
} suggestionElem;

#define UNDEFINED 0xFF

#define Num_Threads 4
#define diaNum 0
#define SwapInvDiaNum 3
static int count = 0;
#define Pairs 37.81
static suggestionElem SuggestionCount[65536];
word4 initKey[4][4],key[4][4][N_Round + 1];
