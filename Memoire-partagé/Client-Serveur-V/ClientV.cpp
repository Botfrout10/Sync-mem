/**
 * Sujet 2 : Création d’une application "client/serveur" - variante
 * Lors de la fermeture d’un client ou du serveur, tester combien de processus sont
 * encore connectés à la mémoire partagée. S’il n’y en a plus, détruire le segment de
 * mémoire partagée.
*/
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <cstring>

#include "config.hpp"

using namespace std;

pid_t  server_pid;

// void sigint_handler(int signum)
// {
//     if( server_pid == 0)
//     {
//         cerr<<"Le pid serveur n'existe pas !! Veuillez envoyer une message au serveur"<<endl;
//         exit(1);
//     }
//     kill(SIGTERM,server_pid);
//     exit(0);
// }

int main()
{
    // signal(SIGINT, sigint_handler);
    int shmid= shmget((key_t)SHM_KEY, 0, S_IRUSR | S_IWUSR);
    if(shmid == -1) 
    {
        cerr<<"Erreur shmget"<<endl;
        return 1;
    }
    
    char* out=(char*)shmat(shmid, NULL, 0);
    if((int*)out == (int*)-1)
    {
        cerr<<"Erreur shmgat:"<<errno<<endl;
        return 2;
    }


    pid_t pid= getpid();
    // SHM= pidclient pidserveur buffer
    //     int        int       char*
    cout<<"Le pid du client:"<<pid<<endl;
    char buffer[NBR_OCTET]={};
    while(true)
    {
        // Ecrire le pid client dans la SHM
        memcpy(out,&pid,sizeof(int));
        // Lire le pid serveur depuis la SHM
        memcpy(&server_pid,out+sizeof(int),sizeof(int));
        if(server_pid == 0)
        {
            cerr<<"Le serveur n'est pas connecté!!"<<endl;
            sleep(5);
            continue;
        }
        cout<<"Le pid serveur:"<<server_pid<<endl;

        // Lire le message
        cout<<"Entrer un text:";
        fgets(out+(sizeof(int)*2),sizeof(char)*NBR_OCTET- (sizeof(int)*2),stdin);
        // Copier le message dans buffer
        memcpy(&buffer,out+sizeof(int)*2,sizeof(char)*NBR_OCTET-(sizeof(int)*2));
        // Envoyer le signale au serveur pour lui dire un message arrive.
        kill(server_pid,SIGCONT);

        cout<<"Print:"<<buffer<<endl;
        sleep(1);
    }
    
    return 0;
}