#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h> 
/*please use -std=c99 to compile this C*/
/*This program has failed to achieve the descend sorting. There are */

//Custom a Card Object using typedef, containing the suit and the value attributes of the card
typedef struct
{
    char suit;
    char val;
}Card;

//Developed a function that deal the Cards from the Card Stack
void Distribute(Card* Stack, Card* HandStack,int playerIndex){
    for(int i=playerIndex;i<52;i+=4){
        HandStack[i/4]=Stack[i];
    }
}

//A funtion that print the cards on the player's hand.
void ShowCard(Card* HandStack,int playerIndex){
    printf("Child %d, pid %d: ",playerIndex+1,getpid());
    for(int i=0;i<13;i++){ //assume that each player got 13 cards (52 cards in stack)
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
    for(int i=0;i<size;i++){
        printf("%c%c ",SelectStack[i].suit,SelectStack[i].val);
    }
    printf("> ");
}

//get the calculated point in the SortCard() and print them out according to the format
void ShowPoints(int playerIndex,int points,int adjPoints){
    printf("\nChild %d, pid %d: ",playerIndex+1,getpid());
    printf("%d points, %d adjusted points",points,adjPoints);
}


// void CardSwap(Card* A,Card* B){
//     Card* temp;
//     temp->suit=A->suit;
//     temp->val=A->val;
//     A->suit=B->suit;
//     A->val=B->val;
//     B->suit=temp->suit;
//     B->val=temp->val;
// }

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


    int Scount=0,Ccount=0,Dcount=0,Hcount=0;
    for(int i=0;i<13;i++){
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

    //calculate the points of honor cards
    for(int i=0;i<13;i++){
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
    for(int i=0;i<4;i++){
        if(Counts[i]>=5){
            Suitpoints+=1;
        }if(Counts[i]>=6){
            Suitpoints+=2;
        }if(Counts[i]>=7){
            Suitpoints+=3;
        }
    }

    //calculate the initial adjusted point (would be varied later)
    int adjPoints=Suitpoints+Valpoints;

    //count 
    int Single=0;
    int Double=0;
    int Zero=0;
    
    for(int i=0;i<4;i++){
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

    
    ShowPoints(playerIndex,Valpoints,adjPoints);

    

//Give up on descending sort
/*I don't know why the program wouldn't work after adding the sorting function
below to each suit of them, the logic should be correct but it just doesn't get
anything printed on the terminal...*/

// J=74, Q=81,
// K=75 (+10) -->85, A=65 (+25) -->90, T=84(-20) -->64 [smallest]
// So that T<J<Q<K<A

    // InnerSort(Sstack,playerIndex,Scount);
    

    // for(int i=0;i<Scount-1;i++){
    //     for(int j=i+1;j<Scount;j++){
    //         char pre=Sstack[i].val;
    //         char next=Sstack[j].val;
    //         if(pre=='K'){
    //             pre+=10;
    //         }if(next=='K'){
    //             next+=10;
    //         }if(pre=='A'){
    //             pre+=25;
    //         }if (next=='A'){
    //             next+=25;
    //         }if(pre=='T'){
    //             pre-=20;
    //         }if (next=='T'){
    //             next+=20;
    //         }  
    //         if(pre<next){
    //             CardSwap(&Sstack[i],&Sstack[j]);
    //         }
    //     }
    // }
    // LimitShow(Sstack,playerIndex,Scount);
        

    // for(int i=0;i<Hcount-1;i++){
    //     for(int j=i+1;j<Hcount;j++){
    //         char pre=Hstack[i].val;
    //         char next=Hstack[j].val;
    //         if(pre=='K'){
    //             pre+=10;
    //         }else if(next=='K'){
    //             next+=10;
    //         }else if(pre=='A'){
    //             pre+=25;
    //         }else if (next=='A'){
    //             next+=25;
    //         }else if(pre=='T'){
    //             pre-=20;
    //         }else if (next=='T'){
    //             next+=20;
    //         }  
    //         if(pre<next){
    //             CardSwap(&Hstack[i],&Hstack[j]);
    //         }
    //     }
    // }
            

    // for(int i=0;i<Ccount-1;i++){
    //     for(int j=i+1;j<Ccount;j++){
    //         char pre=Cstack[i].val;
    //         char next=Cstack[j].val;
    //         if(pre=='K'){
    //             pre+=10;
    //         }else if(next=='K'){
    //             next+=10;
    //         }else if(pre=='A'){
    //             pre+=25;
    //         }else if (next=='A'){
    //             next+=25;
    //         }else if(pre=='T'){
    //             pre-=20;
    //         }else if (next=='T'){
    //             next+=20;
    //         }  
    //         if(pre<next){
    //             CardSwap(&Cstack[i],&Cstack[j]);
    //         }
    //     }
    // }
        

    // for(int i=0;i<Dcount-1;i++){
    //     for(int j=i+1;j<Dcount;j++){
    //         char pre=Dstack[i].val;
    //         char next=Dstack[j].val;
    //         if(pre=='K'){
    //             pre+=10;
    //         }else if(next=='K'){
    //             next+=10;
    //         }else if(pre=='A'){
    //             pre+=25;
    //         }else if (next=='A'){
    //             next+=25;
    //         }else if(pre=='T'){
    //             pre-=20;
    //         }else if (next=='T'){
    //             next+=20;
    //         }  
    //         if(pre<next){
    //             CardSwap(&Dstack[i],&Dstack[j]);
    //         }
    //     }
    // }
        

    // printf("<");
    // for(int i=0;i<Scount;i++){
    // printf("%c%c ",Sstack[i].suit,Sstack[i].val);
    // }
    // printf("> \n");


    printf("\n");
    // printf("S:%d H:%d C:%d D:%d \n",Scount,Hcount,Ccount,Dcount);


}

int main(int argc, char *argv[]){

    // //已知会有52张牌被存入argv[]中(所有花色牌)
    Card Stack[52];
    for(int i=0;i<argc-1;i++){
        Stack[i].suit=argv[i+1][0];
        Stack[i].val=argv[i+1][1];
    }

    // Card HandStack[13];
    // Distribute(Stack,HandStack,0);
    // ShowCard(HandStack,0);
    for(int i=0;i<4;i++){
    int player=fork();
    if(player<0){
        printf("Failed to create the player, the program will terminate now.");
        return 0;
    }else if(player==0){
        Card HandStack[13]; //建立当前玩家手牌[container]
        Distribute(Stack,HandStack,i); //给当前玩家发牌
        ShowCard(HandStack,i); //展示当前玩家所持有的卡
        SortCard(HandStack,i);
        return 0;
    }
            
    }

    //receive exit status from the 4 players
    for(int i=0;i<4;i++){
        wait(NULL);
    }


}

