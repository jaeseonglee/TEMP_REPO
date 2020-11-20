#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DECK 52

#define MAX_CARDS_PER_PLAYER 5

#define BEGIN_MONEY 100.0

#define BLACKJACK 21
#define DEALER_MINIMUM 17


enum suits { club = 0, diamond, heart, spade };
enum status { low = 0, blackjack, bust };
enum moves { hit = 0, stand, double_down, info };

typedef struct card {
	int number;
	enum suits suit;
	int color;
	int flag;
}CARD;

typedef struct player {
	double money;
	double bet;
	CARD cards[MAX_CARDS_PER_PLAYER];
	int card_num;
	enum status status;
}PLAYER;


void deckMaker(CARD* deck);
void playGame(int num);
void card_betting(int num);
void print_cards(int num);
void add_card(int num);
int checkBJ(int num);

CARD deck[DECK];
PLAYER* PLAYERS;

int main(void) {
	int i, num = 0;
	srand(time(NULL));
	
	printf("check Player: ");
	scanf("%d", &num);
	getchar();

	PLAYERS = (PLAYER*)malloc(sizeof(PLAYER) * (num + 1));
	deckMaker(deck);

	for (i = 0; i <= num; i++) {
		PLAYERS[i].money = BEGIN_MONEY;
		PLAYERS[i].bet = 0;
		PLAYERS[i].card_num = 0;
		PLAYERS[i].status = low;
	}

	playGame(num);

	PLAYERS = NULL;
	free(PLAYERS);
	return 0;
}


void deckMaker(CARD* deck) {
	int i;
	for (i = 0; i < 13; i++) { // 0 ~ 12 clubs 13 ~ 25  diamonds 26 ~38  hearts 39 ~ 51 spades
		deck[i].number = i + 1;
		deck[i].suit = club;
		deck[i].color = 0;
		deck[i].flag = 0;

		deck[i + 13].number = i + 1;
		deck[i + 13].suit = diamond;
		deck[i + 13].color = 1;
		deck[i + 13].flag = 0;

		deck[i + 26].number = i + 1;
		deck[i + 26].suit = heart;
		deck[i + 26].color = 1;
		deck[i + 26].flag = 0;

		deck[i + 39].number = i + 1;
		deck[i + 39].suit = spade;
		deck[i + 39].color = 0;
		deck[i + 39].flag = 0;
	}
}



void playGame(int num) {
	int i, result = 0;
	
	
	//while (1) {
		// bet

		card_betting(num);
		
		
		result = checkBJ(num);
		if (result != 0) {
			printf("!!BLACKJACK!!\n");
			return;
		}

		add_card(num);
		print_cards(num);

		result = checkBJ(num);
		if (result != 0) {
			printf("!!BLACKJACK!!\n");
			return;
		}

}

void card_betting(int num) {
	int i, j;
	printf("\n");
	for (i = 0; i <= num; i++) {
		if (PLAYERS[i].money > 0) {
			j = 0;
			while (PLAYERS[i].card_num < 2) {
				int r = rand() % DECK;
				if (deck[r].flag == 0) {
					if (deck[r].number > 10) deck[r].number = 10;
					deck[r].flag = 1;
					PLAYERS[i].cards[j] = deck[r];
					j++;
					PLAYERS[i].card_num++;
				}
			}
		}
	}

	for (i = 0; i <= num; i++) {
		if (PLAYERS[i].cards[0].number == 1) {
			if (11 + PLAYERS[i].cards[1].number <= 21) {
				PLAYERS[i].cards[0].number = 11;
			}
		}

		if (PLAYERS[i].cards[1].number == 1) {
			if (11 + PLAYERS[i].cards[0].number <= 21) {
				PLAYERS[i].cards[1].number = 11;
			}
		}

		if (i == 0) {
			printf("DEALER   :  X ");
			printf("%2d\t\t\t", PLAYERS[i].cards[1].number);
		}
		else {
			printf("PLAYER %d : %2d %2d\t", i, PLAYERS[i].cards[0].number, PLAYERS[i].cards[1].number);
			printf("sum : %2d\t", PLAYERS[i].cards[0].number + PLAYERS[i].cards[1].number);
		}

		if (PLAYERS[i].cards[0].number + PLAYERS[i].cards[1].number < BLACKJACK) {
			PLAYERS[i].status = low;
			printf("result : low\n");
		}
		else if (PLAYERS[i].cards[0].number + PLAYERS[i].cards[1].number == BLACKJACK) {
			PLAYERS[i].status = blackjack;
			printf("result : blackjack!!\n");
		}
		else {
			PLAYERS[i].status = bust;
			printf("result : bust\n");
		}
	}
	printf("\n");
}

void add_card(int num) {
	int i, j, r, sum = 0;

	for (i = 0; i <= num; i++) {
		if (PLAYERS[i].status == low) {
			if (i == 0) {
				for (j = 0; j < PLAYERS[i].card_num; j++) {
					sum += PLAYERS[i].cards[j].number;
				}

				if (sum < DEALER_MINIMUM) {
					while (1) {
						r = rand() % DECK;
						if (deck[r].flag == 0) {
							if (deck[r].number > 10)
								deck[r].number = 10;

							deck[r].flag = 1;
							PLAYERS[i].cards[j] = deck[r];
							PLAYERS[i].card_num++;

							sum += PLAYERS[i].cards[j].number;

							if (sum < DEALER_MINIMUM)
								continue;
							else
								break;
						}
					}
				}
			}
			else {
				char c;
				while (1) {
					printf("PLAYER %d, add card?(y or n) : ", i);
					
					scanf("%c", &c);
					
					fflush(stdin);

					if (c == 'y') {
						j = PLAYERS[i].card_num;
						while (1) {
							r = rand() % DECK;
							if (deck[r].flag == 0) {
								if (deck[r].number > 10)
									deck[r].number = 10;
								deck[r].flag = 1;
								PLAYERS[i].cards[j] = deck[r];
								PLAYERS[i].card_num++;
								break;
							}
						}
						printf("added card number: %d\n", PLAYERS[i].cards[j].number);
						sum += PLAYERS[i].cards[j].number;
						if (sum >= BLACKJACK) 
							break;
					}
					else
						break;
				}
			}
			printf("\n");
		}
	}
}


void print_cards(int num) {
	int i, j ,sum = 0;
	
	printf("\n");
	for (i = 0; i <= num; i++) {
		int index = -1;
		for (j = 0; j < PLAYERS[i].card_num; j++) {
			sum += PLAYERS[i].cards[j].number;
			if (PLAYERS[i].cards[j].number == 1) {
				index = j;
			}
		}

		if (index > -1 && sum <= 21) {
			if (sum + 10 <= 21) {
				PLAYERS[i].cards[index].number = 11;
			}
		}

		sum = 0;
		if (i == 0) 
			printf("DEALER   : ");
		else 
			printf("PLAYER %d : ", i);
		
		for (j = 0; j < PLAYERS[i].card_num; j++) {
			sum += PLAYERS[i].cards[j].number;
			printf("%2d ", PLAYERS[i].cards[j].number);
		}
		printf("\tsum : %2d\t", sum);

		if (sum < BLACKJACK) {
			PLAYERS[i].status = low;
			printf("result : low\n");
		}
		else if (sum == BLACKJACK) {
			PLAYERS[i].status = blackjack;
			printf("result : blackjack!!\n");
		}
		else {
			PLAYERS[i].status = bust;
			printf("result : bust\n");
		}
	}
	printf("\n");
}

int checkBJ(int num) {
	int i;
	for (i = 0; i <= num; i++) {
		if (PLAYERS[i].status == blackjack) {
			if (i == 0)
				printf("DEALER BALCKJACK!!\n");
			return 1;
		}
	}
	return 0;
}


