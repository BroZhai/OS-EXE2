#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>//pid_t 变量(可不用?换int存pid)
#include <sys/wait.h> //wait()函数
#include <unistd.h>

#include <string.h> //字符串处理

#define NUM_PLAYERS 4
#define NUM_CARDS 52

//用<stdio.h>库定义了"卡"这个对象
typedef struct {
    char suit; //花色
    char rank; //数
} Card;

//写了一个发牌的方法
void dealCards(Card* deck, Card* hand, int playerIndex) {
    //每个玩家按照这个特定的"跳牌"技巧取得
    for (int i = playerIndex; i < NUM_CARDS; i += NUM_PLAYERS) {
        hand[i / NUM_PLAYERS] = deck[i];
    }
}

//算J,Q,K,A牌的点数
int calculateHighCardPoints(Card* hand) {
    int points = 0;
    for (int i = 0; i < NUM_CARDS / NUM_PLAYERS; i++) {
        char rank = hand[i].rank;
        switch (rank) {
            case 'A':
                points += 4;
                break;
            case 'K':
                points += 3;
                break;
            case 'Q':
                points += 2;
                break;
            case 'J':
                points += 1;
                break;
            default:
                break;
        }
    }
    return points;
}

int calculateDistributionPoints(Card* hand) {
    int points = 0;
    int suitCount[4] = {0}; // S, H, C, D

    for (int i = 0; i < NUM_CARDS / NUM_PLAYERS; i++) {
        char suit = hand[i].suit;
        switch (suit) {
            case 'S':
                suitCount[0]++;
                break;
            case 'H':
                suitCount[1]++;
                break;
            case 'C':
                suitCount[2]++;
                break;
            case 'D':
                suitCount[3]++;
                break;
            default:
                break;
        }
    }

    //数上面四种花色的各个的数量，相同花色>5及以上"有奖励"
    for (int i = 0; i < 4; i++) {
        if (suitCount[i] >= 5)
            points += 1;
        if (suitCount[i] >= 6)
            points += 2;
        if (suitCount[i] >= 7)
            points += 3;
    }

    int singletonCount = 0;
    int doubletonCount = 0;

    for (int i = 0; i < 4; i++) {
        if (suitCount[i] == 1)
            singletonCount++;
        if (suitCount[i] == 2)
            doubletonCount++;
        //哥们手动补充的
        if(suitCount[i]==0){
            points+=3;
        }
    }

    /*注意，Ai写的这里少考虑了void suit的情况，当没有一张花色的任意牌的时候，玩家总分+3*/

    points += singletonCount * 2;
    points += doubletonCount;

    //没看懂
    for (int i = 0; i < NUM_CARDS / NUM_PLAYERS; i++) {
        char suit = hand[i].suit;
        char rank = hand[i].rank;
        if (rank != 'A' && suitCount[suit - 'S'] == 1)
            points--;
    }

    return points;
}

//子进程内的打印手牌
void printHand(Card* hand, int playerIndex) {
    printf("Child %d, pid %d: ", playerIndex + 1, getpid());
    //和上边发牌的"顺序"逻辑是一样的——跳牌，这几样就能输出"特定"玩家的hand
    for (int i = 0; i < NUM_CARDS / NUM_PLAYERS; i++) {
        printf("%c%c ", hand[i].suit, hand[i].rank); //两个char
    }
    //格式符换行
    printf("\n");
}

//执行main()函数，实现游戏的逻辑机制(读牌，创建玩家进程-->建立玩家的hand手牌13张-->)
int main(int argc, char* argv[]) {
    Card deck[NUM_CARDS];

    if (argc != NUM_CARDS + 1) {//+1是为了包含了“第一个无效”的执行参数
        printf("Invalid number of argumentys.\n");
        return 1; //结束程序
    }

    //取得 命令行输入的所有牌 的 花色 和 数
    for (int i = 0; i < NUM_CARDS; i++) {
        deck[i].suit = argv[i + 1][0];
        deck[i].rank = argv[i + 1][1];
    }

    //for循环，循环创建4个玩家进程 (NUM_PLAYERS=4)
    for (int i = 0; i < NUM_PLAYERS; i++) {
        int pid = fork();

        if (pid < 0) {
            printf("Fork error.\n"); //玩家子进程创建失败
            return 1;
        } else if (pid == 0) {
            Card hand[NUM_CARDS / NUM_PLAYERS];
            dealCards(deck, hand, i); //给当前玩家发牌(对玩家已有的hand手牌做修改) [具体实现看上面的发牌方法]
            printHand(hand, i); //子进程中调用打印玩家手牌的函数[逻辑同上]
            int highCardPoints = calculateHighCardPoints(hand); //计算玩家[手牌中]的J,Q,K,A(高卡)得分
            int distributionPoints = calculateDistributionPoints(hand); //计算花色牌输的"奖励分" 及一些其他细节()
            int adjustedPoints = highCardPoints + distributionPoints; //adjusted points即为 高卡得分 + 奖励分
            //按格式打印出子进程的信息，'人为调整'进程id,子进程内部id，高卡得分，adjusted points
            printf("Child %d, pid %d: %d points, %d adjusted points\n", i + 1, getpid(), highCardPoints, adjustedPoints);
            return 0;
        }
    }

    for (int i = 0; i < NUM_PLAYERS; i++) {
        wait(NULL);
    }

    return 0;
}