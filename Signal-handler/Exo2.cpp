/** Exo2:
 *  Ecrire un programme C qui crée deux processus à l’aide de l’appel
 *  système fork(). Le père affichera les entiers pairs compris entre 1 et
 *  100, le fils affichera les entiers impairs compris dans le même
 *  intervalle. Synchroniser les processus à l’aide des signaux pour que
 *  l’affichage soit 1 2 3 ... 100.
*/
#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>


// La synchronisation est faite par le signal <SIGINT>
        // Le processus ecrit le nombre courant
        // Il envoit le signal SIGINT à l'autre processus
        // Il se met en pause


void sig_handler(int signum){
    return;    
}
int main()
{
    pid_t fpid= fork();
    signal(SIGCONT,sig_handler);
    if(fpid == 0) // Le processus fils.
    {
        for (size_t i = 1; i <= 100; i+=2)
        {
            std::cout<<i<<std::endl;
            kill(getppid(),SIGCONT);
            pause();
        }

    }else // Le processus père.
    {
        // pause();
        for (size_t i = 2; i <= 100; i+=2)
        {
            kill(fpid,SIGCONT);
            pause();
            std::cout<<i<<std::endl;
        }       
    }
    return 0;
}