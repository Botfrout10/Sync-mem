#include <iostream>
#include <unistd.h>
#include <signal.h>

int count_num= 0;

void sigIntHandler(int signum){
    std::cout<<"Le signal SIGINT de num:"<<signum<<std::endl;
    ++count_num;
}

int main()
{

    if(signal(SIGINT, sigIntHandler) == SIG_ERR){
        std::cerr<<"Err: d'affectation de handler au SIGINT"<<std::endl;
        return EXIT_FAILURE;
    }
    while(count_num < 5)
        pause();
    return EXIT_SUCCESS;
}