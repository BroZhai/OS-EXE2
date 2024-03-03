#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    for(int i=1;i<=4;i++){
    
    int player=fork();
    if(player<0){
        printf("Fork Failed...");
        exit(1);
    } else if(player==0){
        int cid=getpid();
        printf("现在是子进程，玩家是: %d \n",player);
        printf("玩家内部ID为: %d \n",cid);
        exit(0);
    }

    }

    return 0;
}

    //int max_processes = 5;  // 最大子进程数量
    // int current_processes = 0;  // 当前子进程数量

    // for (int i = 0; i < max_processes; i++) {
    //     pid_t pid = fork();

    //     if (pid < 0) {
    //         // 创建子进程失败
    //         fprintf(stderr, "创建子进程失败\n");
    //         return 1;
    //     } else if (pid == 0) {
    //         // 子进程代码
    //         printf("子进程 %d 正在运行\n", getpid());
    //         // 子进程执行其他操作...
    //         return 0;  // 子进程终止
    //     } else {
    //         // 父进程代码
    //         current_processes++;  // 增加当前子进程数量
    //         printf("父进程，当前子进程数量：%d\n", current_processes);

    //         if (current_processes >= max_processes) {
    //             // 达到最大子进程数量，父进程等待子进程终止
    //             while (current_processes > 0) { //循环判断时候还有子进程的条件
    //                 int status;
    //                 pid_t child_pid = wait(&status);  // 等待任一子进程终止
    //                 if (child_pid != -1) {
    //                     printf("子进程 %d 终止\n", child_pid);
    //                     current_processes--;  // 减少当前子进程数量
    //                 }
    //             }
    //         }
    //     }
    // }

    // return 0;