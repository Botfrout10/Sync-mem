/**
 * Réaliser une application en C qui crée N processus fils, tous en pause.
 * Le processus père effectue continuellement les opérations suivantes :
 * il réveille le premier processus fils par l’envoi d’un signal SIGCONT,
 * s’endort 1 seconde, puis envoie SIGSTOP au même processus fils et
 * un SIGCONT au fils suivant. Lorsqu’un processus fils reçoit le signal
 * SIGCONT, il affiche un message indiquant qu’il a intercepté le signal
 * avant de continuer en affichant, en continu, son numéro
**/

#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

#define NBR_PROCESS 10
// bash : killall -9 -r Exo3 : This commande kill all ps with Exo3 name.

void sigcont_handler(int signum)
{
    std::cout<<"En continu:"<<getpid()<<'\n';
}

void sigstp_handler(int signum)
{
    std::cout<<"Stope:"<<getpid()<<'\n';
}

int main()
{
    pid_t fpids[NBR_PROCESS];
    // Creation de N processus
    // init_processus(NBR_PROCESS, fpids);
    int i= 0; 
    pid_t tmp=1;
    while(i < NBR_PROCESS && tmp != 0 )
    {
        tmp=fork();
        
        if(tmp > 0 )
        {
            fpids[i]=tmp;
        }
        if(tmp == 0)
        {
            signal(SIGCONT, sigcont_handler);
            signal(SIGSTOP, sigstp_handler);
        }
        ++i;
    }

    i=0;
    while(i < NBR_PROCESS)
    {
        if(fpids[i] == 0)
        {
            // std::cout<<"Le proc:"<<getpid()<<" est en pause\n";
            pause();
        }   
        ++i;
    }
    i=0;
    while(1 && i < NBR_PROCESS)
    {
        if(tmp != 0)
        {
            kill(fpids[i],SIGCONT);
            sleep(1);
            kill(fpids[i],SIGSTOP);
            ++i;
        }
    }
    return 0;
}