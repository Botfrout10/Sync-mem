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

char *out;
int client_pid;
int shmid;

void sigint_handler(int signum)
{
    std::cout<<"SIGNIT est intercepte"<<std::endl;
    if(client_pid == 0)
    {
        std::cerr<<"Client pid n'existe pas!!"<<std::endl;
        exit(2);
    }
    kill(client_pid, SIGINT);
    if(shmdt(out) == -1)
    {
        std::cerr<<"Erreur shmdt";
        exit(3);
    }
    if(shmctl( shmid , IPC_RMID , NULL) == -1)
    {
        std::cerr<<"Erreur shmctl";
        exit(4);
    }
    std::cout<<"La SHM n°: "<<shmid<<"est liberée"<<std::endl;
}

void sigcont_handler(int signum)
{
        char buffer[NBR_OCTET]={};
        // Lire le pid client depuis la SHM
        std::memcpy(&client_pid, out, sizeof(int));
        std::cout<<"pid du client:"<<client_pid<<std::endl;
        // Copier le message dans buffer
        std::memcpy(&buffer, out+sizeof(int)*2, sizeof(char)*NBR_OCTET-(sizeof(int)*2));
        std::cout<<"Afficher le SHM: "<<buffer<<std::endl;
}

int main()
{
    signal(SIGINT,sigint_handler);
    signal(SIGCONT,sigcont_handler);
    // Serveur
    // Allocation de la memoire partagé:
    shmid= shmget((key_t)SHM_KEY, sizeof(char)*NBR_OCTET, IPC_CREAT| S_IRUSR | S_IWUSR );
    if(shmid == -1) 
    {
        std::cerr<<"Erreur shmget"<<std::endl;
        return 1;
    }
    
    out=(char*)shmat(shmid, NULL, 0);
    if((int*)out == (int*)-1)
    {
        std::cerr<<"Erreur shmgat:"<<errno<<std::endl;
        return 2;
    }
    

    // fgets(out,sizeof(int),stdin);
    
    // Init shm
    for(int i= sizeof(int); i < NBR_OCTET; ++i)
        out[i]=(char)i-4+65;

    pid_t pid= getpid();
    std::cout<<"Le pid serveur:"<<pid<<std::endl;
    // Ecrire le pid Serveur dans la SHM
    std::memcpy(out+sizeof(int), &pid, sizeof(int));

    std::cout<<"Waiting for request..."<<std::endl;
    while(true)
    {
        pause();
    }
    
    /**
     * Remplacer par sigcont_handler
     */
    // while(true)
    // {
    //     // Ecrire le pid client dans la SHM
    //     std::memcpy(&client_pid, out, sizeof(int));
    //     std::cout<<"pid du client:"<<client_pid<<std::endl;
    //     // Ecrire le pid Serveur dans la SHM
    //     std::memcpy(out+sizeof(int), &pid, sizeof(int));
    //     // Copier le message dans buffer
    //     std::memcpy(&buffer, out+sizeof(int)*2, sizeof(char)*NBR_OCTET-(sizeof(int)*2));
    //     std::cout<<"Afficher le SHM: "<<buffer<<std::endl;
    //     sleep(1);
    // }
    return 0;
}
