#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

void sigint_handler(int signum){
    std::cout<<"Le SIGINT traite..."<<std::endl;
}

void sigquit_handler(int signum){
    std::cout<<"Le SIGQUIT traite..."<<std::endl;
}
int main()
{
    pid_t fpid= fork();
    if (fpid == 0) // On est dans le processus fils
    {
        std::cout<<"pid fils:"<<getpid();
        signal(SIGINT,sigint_handler);
        signal(SIGQUIT,sigquit_handler);
        std::cout<<"Le processus fils va dormir 10s"<<std::endl;
        sleep(10);
        std::cout<<"Le processus fils va dormir 100s"<<std::endl;
        sleep(100);
        std::cout<<"Le processus fils va dormir 200s"<<std::endl;
        sleep(200);
    }else{ // On est dans le processus pere.
        std::cout<<"pid pere:"<<getpid();
        sleep(1);
        kill(fpid, SIGINT);
        sleep(2);
        kill(fpid, SIGQUIT);
        sleep(5);
        kill(fpid, SIGKILL);
    }
    std::cout<<"Le traitement est terminer"<<std::endl;
    return 0;
}