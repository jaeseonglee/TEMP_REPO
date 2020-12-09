#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <time.h>

#define DECK 52
#define BEGIN_MONEY 100
#define MAX_CARDS_PER_PLAYER 9
#define MINIMUM_BET 10
#define BLACKJACK 21
#define DEALER_MINIMUM 17

enum suits { club = 0, diamond, heart, spade };
enum status { live = 0, blackjack, bust };

typedef struct card {
        int number;
        enum suits suit;	//모양
        int color;		//색갈
        int flag;  		//
}CARD;

typedef struct player {
        int money;		//돈
        int bet;		//베팅
        CARD cards[MAX_CARDS_PER_PLAYER];	//카드
        int card_num;				//카드의 갯수
        enum status status;			//상태
}PLAYER;


void player_set(PLAYER *player);
void deckMaker(CARD *deck);
void game_start();
int money_bet();
void draw_card(PLAYER *player);
int print_game(int turn);
void game_result();
int add_card(PLAYER,int);
void dealer_draw();
int ben();
int card_sum(PLAYER p);
int check_bj(PLAYER p);

PLAYER *player;
PLAYER *dealer;
CARD deck[DECK];

int main(void) {
	
	//카드 정보
	player = (PLAYER*)malloc(sizeof(PLAYER));
	dealer = (PLAYER*)malloc(sizeof(PLAYER));
	
	system("clear");
	srand(time(NULL));
 
	printf("Welcome to Blackjack Station");
	
	game_start();	

	sleep(1);

	free(player);
	free(dealer);
	player = NULL;
	dealer = NULL;

	return 0;
}

void player_set(PLAYER *player) {
        player->status = 0;
        player->card_num = 0;
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

void game_start() {
	char buf[256];

	//처음 게임 돈
	player->money = BEGIN_MONEY;

	while(1){

		int i=0;
		player_set(player);
		player_set(dealer);
	        deckMaker(deck);

		printf("ww");	
		

		if( (i= money_bet()) == -1) {
			printf("Bye!\n");
			break;
		}

		//처음 두장 드로우
		draw_card(player);
		draw_card(dealer);
		
		//블랙잭 나올 시 종료
		if(check_bj(*player) || check_bj(*dealer)) {

			print_game(0);

			if(check_bj(*player))
				printf("===you got BlackJack===\n\n\n");
			else
				printf("===Dealer got BlackJack===\n\n\n");
			continue;

			game_result();

		}
			
		//카드 뽑기
		print_game(1);
		while(add_card(*player,1) != 0){
			print_game(1);
		}
		
		//딜러 뽑기
		dealer_draw();	
		

		//출력
		print_game(0);
		game_result();

		if(ben() == 1)
			break;
		
		printf("press Enter");
		getchar();
		system("clear");
	}

}

int money_bet() {
	int num = 0;
        printf("\nyour money: %d\n", player->money);
	
	while(1) {
		printf("\ninput bet: ");
		scanf("%d", &num);
		getchar();
        	           	
                if(num  > player->money) {
			// 돈보다 많은 배팅
			 continue;
		} else if(num < 0) {
			// 잘못된 배팅
			continue;
		} else if(num == 0) {
			return -1;
		}else if(num >= MINIMUM_BET) {
			player->bet = num;
			break;
		}
	}
        player->bet = num;
        player->card_num = 0;

	return 0;
}

//처음 카드 두장 뽑기
void draw_card(PLAYER *p) {
	int i = 0;

        while (p->card_num < 2) {
		 int r = rand() % DECK;       

	 	 if (deck[r].flag == 0) { // flag : 카드 사용중
			 //if (deck[r].number > 10) deck[r].number = 10;
			 deck[r].flag = 1;
			 p->cards[i] = deck[r];
			 i++;
			 p->card_num++;
       		 }
       	}

  	// 1 check  
	//if (p->cards[0].number == 1) {
	//	p->cards[0].number = 11;
	//}
	//if (p->cards[1].number == 1) {
	//	if (11 + p->cards[0].number <= BLACKJACK) {
	//		p->cards[1].number = 11;
	//	}
	//}

	//if (p->cards[0].number + p->cards[1].number < BLACKJACK)
	//	p->status = live;
	//else if (p->cards[0].number + p->cards[1].number == BLACKJACK)
        //        p->status = blackjack;
	//else 
	//	     	p->status = bust;
}

//게임 정보 출력(카드정보, 딜러정보, 플레이어 정보)
int print_game(int turn) {
	int i, result = 0;	
	char str[256];
        char temp[256]= "";

	//내 카드 출력
	printf("\nyour cards:\n");
	for(i = 0 ; i < player->card_num; i++ ) {
                //enum suits { club = 0, diamond, heart, spade };
                if(player->cards[i].suit == club)
                        strcat(str, "club" );
                else if(player->cards[i].suit == diamond)
                        strcat(str, "diamond" );
                else if(player->cards[i].suit == heart)
                        strcat(str, "heart" );
                else if(player->cards[i].suit == spade)
                        strcat(str, "spade" );
                strcat(str, " ");

                // 11일 경우
                //if(player->cards[i].number == 1 || player->cards[i].number == 11)
                if(player->cards[i].number == 1)
			strcat(str, "Ace ");
		else if(player->cards[i].number == 11)
			strcat(str, "Jack ");
		else if(player->cards[i].number == 12)
                        strcat(str, "Queen ");
		else if(player->cards[i].number == 13)
                        strcat(str, "King ");
                else {
                        sprintf(temp, "%d", player->cards[i].number);
			strcat(str, temp );
			strcat(str, " ");
			strcpy(temp,"");
		}
	}
        printf("%s \n", str);
	strcpy(str ,"");
        
	//딜러카드 출력
	printf("\ndealer's card:\n");

	if(turn == 1) {
		printf("X \t");
		i = 1;
	} else 
		i = 0;


	for( ; i < dealer->card_num; i++ ) {
                //enum suits { club = 0, diamond, heart, spade };
                if(dealer->cards[i].suit == club)
                        strcat(str, "club" );
                else if(dealer->cards[i].suit == diamond)
                        strcat(str, "diamond" );
                else if(dealer->cards[i].suit == heart)
                        strcat(str, "heart" );
                else if(dealer->cards[i].suit == spade)
                        strcat(str, "spade" );
                strcat(str, " ");

                // 11일 경우
                //if(dealer->cards[i].number == 1 || dealer->cards[i].number == 11)
                //        strcat(str, "Ace ");
                if(dealer->cards[i].number == 1)
                        strcat(str, "Ace ");
                else if(dealer->cards[i].number == 11)
                        strcat(str, "Jack ");
                else if(dealer->cards[i].number == 12)
                        strcat(str, "Queen ");
                else if(dealer->cards[i].number == 13)
                        strcat(str, "King ");
		else {
			sprintf(temp, "%d", dealer->cards[i].number);
                        strcat(str, temp );
                        strcat(str, " ");
			strcpy(temp,"");
                }
        }
	printf("%s \n", str);
        strcpy(str ,"");
	
	printf("\ndealer's status : ");
	if(dealer->status == live)
		printf("low\n");
	else if(dealer->status == blackjack) {
		printf("BlackJack!!\n");
		result = 1;
	}
	else {
		printf("bust\n");
		result = 1;
	}

	printf("your's status : ");
        if(dealer->status == live)
                printf("low\n");
        else if(dealer->status == blackjack) {
                printf("BlackJack!!\n");
                result = 1;
        }
	else {
                printf("bust\n");
		result = 1;
	}
	
	return result;
}

//카드 추가
int add_card(PLAYER p,int mode) {
	int i, j, r;
        int sum = 0;
	char str[100];		

	//카드 뽑을지 질문
	if(mode == 1){
		printf("\nadd card? y or n :");
		fgets(str,sizeof(str), stdin);	
	}
	//안 뽑을시 탈출
	if(str[0] == 'n')
		return 0;	
	
	//뽑을시

	//카드 랜덤
	while(1){
		i = p.card_num;
	        r = rand() % DECK;
	
		//안뽑은 카드일 때
        	if (deck[r].flag == 0) {
        	        deck[r].flag = 1;
        	        p.cards[i] = deck[r];
        	        p.card_num++;
			break;
        	}
	}
	sum = card_sum(p);
	check_bj(p);

	return 1;
}
  
//게임 결과
void game_result() {

	char buf[20];

	 if(dealer->status == blackjack) {
                if(player->status == blackjack)	//비김
			{player->bet = 0; strcpy(buf,"draw");}
                else if(player->status == live || player->status == bust) //플레이어 패배
			{player->money -= player->bet; strcpy(buf,"player lose");}
        }
        else if(dealer->status == live) {
                if(player->status == blackjack)		//플레이어 블랙잭 승리
			{player->money += player->bet; strcpy(buf,"player win");}
                else if(player->status ==  bust)	//플레이어 패배
			{player->money -= player->bet; strcpy(buf,"player lose");}
                else {					//둘다 살아있을 때
                        int i, dealerSum = 0, sum = 0;
                        for (i = 0; i < dealer->card_num; i++)
				{dealerSum += dealer->cards[i].number;}
                        for (i = 0; i < player->card_num; i++)
                                sum += player->cards[i].number;

                        if(sum < dealerSum)		//승패 비교
				{player->money -= player->bet; strcpy(buf,"player lose");}
                        else if(sum > dealerSum)
				{player->money += player->bet; strcpy(buf,"player win");}
                }
        }
        else
                player->money += player->bet;

	printf("%s\n",buf);
}


void dealer_draw() {
        int i, j, r;
        int sum = 0;

        if (dealer->status == live) {
                for (i = 0; i < dealer->card_num; i++)
                        sum += dealer->cards[i].number;
        }
        else if(dealer->status == blackjack)
                sum = BLACKJACK;

        if (sum < DEALER_MINIMUM) {
                while (1) {
                        r = rand() % DECK;
                        if (deck[r].flag == 0) {
                                if (deck[r].number > 10)
                                        deck[r].number = 10;

                                dealer->cards[i] = deck[r];
                                dealer->card_num++;

                                sum += dealer->cards[i].number;
                                i++;

                                if (sum < DEALER_MINIMUM)
                                     continue;
                                else if(sum >= DEALER_MINIMUM) {
                                        for(j = 0; j < dealer->card_num; j++) {
                                                if(dealer->cards[j].number == 11){
                                                        dealer->cards[j].number = 1;
                                                        sum -= 10;
                                                        break;
                                                }
                                        }// for
                                        if(sum >= DEALER_MINIMUM)
                                                break;
                                        else
                                                continue;
                                }// else if
                        }// if
                }// while       
        }//if

        if(sum > BLACKJACK)
                dealer->status = bust;
        else if( sum == BLACKJACK)
                dealer->status = blackjack;
        else
                dealer->status = live;
}
             

int ben() {
	if(player->money == 0) {
		printf("youer money : 0\n");
		return 1;
	}
	return 0;
}

//카드들의 합
int card_sum(PLAYER p)
{
	int i=0,sum=0,k=0;
	for(; i<p.card_num; i++){
		k = p.cards[i].number;
		if(k>10)
			sum += 10;
		else
			sum += k;
	}
	if(sum > BLACKJACK)
		p.status = bust;
	
	return sum;
}

//블랙잭인지 확인하고 status 바꿔주기
int check_bj(PLAYER p)
{
	int i=0,sum=0,k=0,boolean=1;
	//return 0;
	//지금 21인가?
	if(card_sum(p) == 21)
	{
		p.status = blackjack;
		return 1;
	}	

	//a를 고려해야 할 때
	for(;i<p.card_num;i++)
	{
		k= p.cards[i].number;
		if(k==1 && boolean ==1){
			sum += 11;
			boolean = 0;
			continue;
		}
		else if(k>10)
			sum += 10;
		else sum += k;
	}
	if(sum == 21)
	{
                p.status = blackjack;
                return 1;
        }
	return 0;
}

