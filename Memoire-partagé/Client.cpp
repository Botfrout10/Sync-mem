/**
 * Sujet 1 : Création d’une application "client/serveur"
 * Écrire 2 processus qui communiquent entre eux par mémoire partagée.
 * Le premier processus (serveur), se chargera d’initialiser la mémoire partagée et
 * chaque seconde affichera le contenu de la mémoire.
 * Le second processus (client), se chargera de demander continuellement à
 * l’utilisateur de saisir du texte et placera le texte saisi en mémoire partagé.
 * Remarque : Lors de la fermeture du serveur, envoyer un signal au client pour
 * qu’il se ferme également puis détruire le segment de mémoire partagée.
*/
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <cstring>

#include "config.hpp"


int main()
{
    int shmid= shmget((key_t)SHM_KEY, 0, S_IRUSR | S_IWUSR);
    if(shmid == -1) 
    {
        std::cerr<<"Erreur shmget"<<std::endl;
        return 1;
    }
    
    char* out=(char*)shmat(shmid, NULL, 0);
    if((int*)out == (int*)-1)
    {
        std::cerr<<"Erreur shmgat:"<<errno<<std::endl;
        return 2;
    }


    pid_t pid= getpid(), server_pid;
    // SHM= pidclient pidserveur buffer
    //     int        int       char*
    std::cout<<"Le pid du client:"<<pid<<std::endl;
    char buffer[NBR_OCTET]={};
    while(true)
    {
        // Ecrire le pid client dans la SHM
        std::memcpy(out,&pid,sizeof(int));
        // Lire le pid serveur depuis la SHM
        std::memcpy(&server_pid,out+sizeof(int),sizeof(int));
        std::cout<<"Le pid serveur:"<<server_pid<<std::endl;

        // Lire le message
        std::cout<<"Entrer un text:";
        fgets(out+(sizeof(int)*2),sizeof(char)*NBR_OCTET- (sizeof(int)*2),stdin);
        // Copier le message dans buffer
        std::memcpy(&buffer,out+sizeof(int)*2,sizeof(char)*NBR_OCTET-(sizeof(int)*2));
        // Envoyer le signale au serveur pour lui dire un message arrive.
        kill(server_pid,SIGCONT);

        std::cout<<"Print:"<<buffer<<std::endl;
        sleep(1);
    }
    
    return 0;
}