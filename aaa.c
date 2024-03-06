/*This program has failed to achieve the level 3 requirement, there are several bugs when I'm trying to write the descend sorting algorithm. 
  I think my function should be logically correct but it turns out that all of the sorted outputs just mysteriously disappeared.

  Examples below:

  Before:
  (Input)./queue S5 C7 CQ SK HT HA H5 D4 DK D6 S3 C8 S4 S7 HQ DJ H7 SQ C5 D9 S6 HK D8 C4 S8 H6 H8 HJ D2 H4 CT SA H3 C3 D5 ST H2 CK C2 D7 DT S2 H9 D3 DA C9 CJ DQ S9 SJ C6 CA
  (Output) 
  Child 2, pid 1633: C7 HA D6 S7 SQ HK H6 H4 C3 CK S2 C9 SJ
  Child 2, pid 1633: <S7 SQ S2 SJ > <HA HK H6 H4 > <C7 C3 CK C9 > <D6 >
  Child 2, pid 1633: 13 points, 15 adjusted points
  Child 1, pid 1632: S5 HT DK S4 H7 S6 S8 D2 H3 H2 DT DA S9
  Child 1, pid 1632: <S5 S4 S6 S8 S9 > <HT H7 H3 H2 > <> <DK D2 DT DA >
  Child 1, pid 1632: 7 points, 11 adjusted points
  Child 3, pid 1634: CQ H5 S3 HQ C5 D8 H8 CT D5 C2 H9 CJ C6
  Child 3, pid 1634: <S3 > <H5 HQ H8 H9 > <CQ C5 CT C2 CJ C6 > <D8 D5 >
  Child 3, pid 1634: 5 points, 11 adjusted points
  Child 4, pid 1635: SK D4 C8 DJ D9 C4 HJ SA ST D7 D3 DQ CA
  Child 4, pid 1635: <SK SA ST > <HJ > <C8 C4 CA > <D4 DJ D9 D7 D3 DQ >
  Child 4, pid 1635: 15 points, 20 adjusted points

  After:
  (Output)
  Child 1, pid 1654: S5 HT DK S4 H7 S6 S8 D2 H3 H2 DT DA S9
  Child 1, pid 1654: <S5 S4 S6 S8 S9 > <HT H7 H3 H2 > <> <DK D2 DT DA >
  Child 2, pid 1655: C7 HA D6 S7 SQ HK H6 H4 C3 CK S2 C9 SJ
  Child 2, pid 1655: <S7 SQ S2 SJ > <HA HK H6 H4 > <C7 C3 CK C9 > <D6 >
  Child 3, pid 1656: CQ H5 S3 HQ C5 D8 H8 CT D5 C2 H9 CJ C6
  Child 3, pid 1656: <S3 > <H5 HQ H8 H9 > <CQ C5 CT C2 CJ C6 > <D8 D5 >
  Child 4, pid 1657: SK D4 C8 DJ D9 C4 HJ SA ST D7 D3 DQ CA
  Child 4, pid 1657: <SK SA ST > <HJ > <C8 C4 CA > <D4 DJ D9 D7 D3 DQ >

  I cannot figure out a resolution so I just left all the sorting funtions commented in this source file 
  I've tried, sorry T^T...*/
/*------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h> 

//Custom a Card Object using typedef, containing the suit and the value attributes of the card
typedef struct
{
  char suit;
  char val;
}Card;

//Developed a function that deal the Cards from the Card Stack
void Distribute(Card* Stack, Card* HandStack,int playerIndex){
  int i;
  for(i=playerIndex;i<52;i+=4){
    HandStack[i/4]=Stack[i];
  }
}

//A funtion that print the cards on the player's hand.
void ShowCard(Card* HandStack,int playerIndex){
  printf("Child %d, pid %d: ",playerIndex+1,getpid());
  int i;
  for(i=0;i<13;i++){ //assume that each player got 13 cards (52 cards in stack)
    printf("%c%c ",HandStack[i].suit,HandStack[i].val);
  }
  printf("\n");
}

//Another print function to show all the card being devied into different groups
void LimitShow(Card* SelectStack,int playerIndex,int size){
  printf("<");
  if(size==0){
    printf("> ");
    return;
  }
  int i;
  for(i=0;i<size;i++){
    printf("%c%c ",SelectStack[i].suit,SelectStack[i].val);
  }
  printf("> ");
}

//get the calculated point in the SortCard() and print them out according to the format
void ShowPoints(int playerIndex,int points,int adjPoints){
  printf("\nChild %d, pid %d: ",playerIndex+1,getpid());
  printf("%d points, %d adjusted points",points,adjPoints);
}


/*These functions were failed*/
void CardSwap(Card* A,Card* B){
  Card* temp = malloc(sizeof(Card));
  *temp = *A;
  *A = *B;
  *B = *temp;
}

/* J=74, Q=81,
   K=75 (+10) -->85, A=65 (+25) -->90, T=84(-20) -->64 [smallest]
   So that T<J<Q<K<A */

void DescendSort(Card* SelectStack,int size){
  int i,j;
  for(i=0;i<size-1;i++){
    for(j=i+1;j<size;j++){
      char pre=SelectStack[i].val;
      char next=SelectStack[j].val;
      if(pre=='K'){
        pre+=10;
      }if(next=='K'){
        next+=10;
      }if(pre=='A'){
        pre+=25;
      }if (next=='A'){
        next+=25;
      }if(pre=='T'){
        pre-=20;
      }if (next=='T'){
        next-=20;
      }
      if(pre<next){
        CardSwap(SelectStack + i,SelectStack+j);
      }
    }
  }
}
/* ------------------------------------------------------------------------------------ */

//A function that sort the cards in player's hand and calculate the value for the hand
void SortCard(Card* HandStack,int playerIndex){
  printf("Child %d, pid %d: ",playerIndex+1,getpid());

  //Create 4 arrays to store different suit of cards
  Card Sstack[13];
  Card Cstack[13];
  Card Dstack[13];
  Card Hstack[13];

  int Valpoints=0;
  int Suitpoints=0;

  DescendSort(HandStack, 13);

  int Scount=0,Ccount=0,Dcount=0,Hcount=0;
  int i;
  for(i=0;i<13;i++){
    if(HandStack[i].suit == 'S'){
      Sstack[Scount]=HandStack[i];
      Scount++;
    }else if(HandStack[i].suit == 'C'){
      Cstack[Ccount]=HandStack[i];
      Ccount++;
    }
    else if(HandStack[i].suit == 'D'){
      Dstack[Dcount]=HandStack[i];
      Dcount++;
    }
    else if(HandStack[i].suit == 'H'){
      Hstack[Hcount]=HandStack[i];
      Hcount++;
    }
  }

  //Showing player's hand
  LimitShow(Sstack,playerIndex,Scount);
  LimitShow(Hstack,playerIndex,Hcount);
  LimitShow(Cstack,playerIndex,Ccount);
  LimitShow(Dstack,playerIndex,Dcount);

  //calculate the points of honor cards, and count the numbers for each honor cards
  for(i=0;i<13;i++){
    if(HandStack[i].val=='J'){
      Valpoints+=1;
    }
    else if(HandStack[i].val=='Q'){
      Valpoints+=2;
    }
    else if(HandStack[i].val=='K'){
      Valpoints+=3;
    }
    else if(HandStack[i].val=='A'){
      Valpoints+=4;
    }
  }

  //calculate the five,six,seven-card suit
  int Counts[4]={Scount,Hcount,Ccount,Dcount};
  for(i=0;i<4;i++){
    if(Counts[i]==5){
      Suitpoints+=1;
    }if(Counts[i]==6){
      Suitpoints+=2;
    }if(Counts[i]>=7){
      Suitpoints+=3;
    }
  }

  //calculate the initial adjusted point (would be varied later)
  int adjPoints=Suitpoints+Valpoints;

  //count the number of singleton, doubleton and Zero
  int Single=0;
  int Double=0;
  int Zero=0;
    
  for(i=0;i<4;i++){
    if(Counts[i]==0){
      Zero++;
    }
    if(Counts[i]==1){
      Single++;
    }
    if(Counts[i]==2){
      Double++;
    }
  }

  adjPoints+=3*Zero+2*Single+Double;

  if (Scount == 1) {
    if (Sstack[0].val == 'J' || Sstack[0].val == 'Q' || Sstack[0].val == 'K' || Sstack[0].val == 'A') {
      adjPoints --;
    }
  }
  if (Ccount == 1) {
    if (Cstack[0].val == 'J' || Cstack[0].val == 'Q' || Cstack[0].val == 'K' || Cstack[0].val == 'A') {
      adjPoints --;
    }
  }
  if (Dcount == 1) {
    if (Dstack[0].val == 'J' || Dstack[0].val == 'Q' || Dstack[0].val == 'K' || Dstack[0].val == 'A') {
      adjPoints --;
    }
  }
  if (Hcount == 1) {
    if (Hstack[0].val == 'J' || Hstack[0].val == 'Q' || Hstack[0].val == 'K' || Hstack[0].val == 'A') {
      adjPoints --;
    }
  }
    

  //Print the Score
  ShowPoints(playerIndex,Valpoints,adjPoints);
  printf("\n");
}

int main(int argc, char *argv[]){

  //Known that a stack of 52 cards will be input in the command line
  Card Stack[52];
  //Store all the input cards.
  int i;
  for(i=0;i<argc-1;i++){
    Stack[i].suit=argv[i+1][0];
    Stack[i].val=argv[i+1][1];
  }

  //Using for-loop to create 4 players(childs)
  for(i=0;i<4;i++){
    int player=fork();
    if(player<0){
      printf("Failed to create the player, the program will terminate now.");
      return 0;
    }else if(player==0){//What would be done to each player
      Card HandStack[13]; //Construct the hand stack for the player
      Distribute(Stack,HandStack,i); //Extract the specific card from the Stack to player's hand
      ShowCard(HandStack,i); //Initially print the player's hand
      SortCard(HandStack,i); //Group and Calculate the player's hand
      exit(0); //termination of a child process
    }
  }

  //receive exit status from the 4 players (parent)
  for(i=0;i<4;i++){
    wait(NULL);
  }

  printf("I've written some problems encountered in the source code, please check it.\n");
}

