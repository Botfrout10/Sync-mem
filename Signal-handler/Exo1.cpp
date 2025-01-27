#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <string.h>

// Exercice 1 : Écrire un programme C qui compte les signaux qu’il reçoit et affiche ces compteurs.

int sig_count[NSIG];
int allsig_count= 0;
void sig_handler(pid_t signum)
{
    ++allsig_count;
    ++sig_count[signum];
    std::cout<<"Le signal reçu est:"<<strsignal(signum)<<" de numero:"<<signum<<std::endl;
    std::cout<<"Le compteur du signal:"<<sig_count[signum]<<std::endl;
}

bool allSigCalled()
{
    int i=0;
    while (i < NSIG)
    {
        if (!sig_count[i])
            break;
        ++i;
    }
    return i == NSIG;
}

int main()
{
    std::cout<<"Le programe est entrain d'ecouter les signaux..."<<getpid()<<std::endl;
    for(pid_t i=1; i< NSIG; ++i)
        signal(i, sig_handler);
    int i=0;
    while(!allSigCalled() && allsig_count < 10)
        pause();
    std::cout<<"Le programe est terminé"<<std::endl;
    return 0;
}
