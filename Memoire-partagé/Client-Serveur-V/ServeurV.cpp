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
#include <set>

#include "config.hpp"


using namespace std;

char *out;
int last_client_pid;
int shmid;

// TODO: Completer cela:
// set<pid_t> pids_client; 

void sigint_handler(int signum)
{
    cout<<"SIGNIT est intercepte"<<endl;
    if(last_client_pid == 0)
    {
        cerr<<"Les pids des clients n'existent pas!!"<<endl;
        exit(2);
    }

    // Si la SHM contient plusiuers proc attaché detache le client courant.
    shmid_ds buffer;
    shmctl(shmid,IPC_STAT,&buffer);
    cout<<"Nombre de processus attaché "<<buffer.shm_nattch<<endl;
    
    // Afficher les pids des processus attaché
    // cout<<"Les pids client attachés au SHM:"<<endl;
    // for(const pid_t pid_client : pids_client)
    //     cout<<pid_client<<" | ";
    // cout<<endl;

    if(buffer.shm_nattch == 1 /* && pids_client.size() == 0*/)
    {
        // Détacher le serveur de la SHM.
        if(shmdt(out) == -1)
        {
            cerr<<"Erreur shmdt"<<endl;
            exit(3);
        }
        // Supprimer la SHM.
        if(shmctl( shmid , IPC_RMID , NULL) == -1)
        {
            cerr<<"Erreur shmctl"<<endl;
            exit(4);
        }
        cout<<"La SHM n°: "<<shmid<<" est liberée"<<endl;
        exit(0);
    }else{
        cout<<"La memoire contient encore des processus attaché, impossible de supprimer la SHM!!"<<endl;
    }
}


// Le pid du client est stocker dès qu'il envoye un message.
void sigcont_handler(int signum)
{
        char buffer[NBR_OCTET]={};
        // Lire le pid client depuis la SHM
        memcpy(&last_client_pid, out, sizeof(int));
        cout<<"pid du client:"<<last_client_pid<<endl;

        // Stocker le processus client dans la set
        // pids_client.insert(last_client_pid);

        // Copier le message dans buffer
        memcpy(&buffer, out+sizeof(int)*2, sizeof(char)*NBR_OCTET-(sizeof(int)*2));
        cout<<"Afficher le SHM: "<<buffer<<endl;
}

// void sigterm_handler(int signum)
// {

// }

int main()
{

    signal(SIGINT,sigint_handler);
    signal(SIGCONT,sigcont_handler);
    // signal(SIGTERM,sigterm_handler);
    // Serveur
    // Allocation de la memoire partagé:
    shmid= shmget((key_t)SHM_KEY, sizeof(char)*NBR_OCTET, IPC_CREAT| S_IRUSR | S_IWUSR );
    if(shmid == -1) 
    {
        cerr<<"Erreur shmget"<<endl;
        return 1;
    }
    
    out=(char*)shmat(shmid, NULL, 0);
    if((int*)out == (int*)-1)
    {
        cerr<<"Erreur shmgat:"<<errno<<endl;
        return 2;
    }
    


    pid_t pid= getpid();
    cout<<"Le pid serveur:"<<pid<<endl;
    // Ecrire le pid Serveur dans la SHM
    memcpy(out+sizeof(int), &pid, sizeof(int));

    cout<<"Waiting for request..."<<endl;
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
    //     memcpy(&client_pid, out, sizeof(int));
    //     cout<<"pid du client:"<<client_pid<<endl;
    //     // Ecrire le pid Serveur dans la SHM
    //     memcpy(out+sizeof(int), &pid, sizeof(int));
    //     // Copier le message dans buffer
    //     memcpy(&buffer, out+sizeof(int)*2, sizeof(char)*NBR_OCTET-(sizeof(int)*2));
    //     cout<<"Afficher le SHM: "<<buffer<<endl;
    //     sleep(1);
    // }
    return 0;
}
