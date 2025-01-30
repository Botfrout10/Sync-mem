/**
 * Sujet 3 : Gestion de la connexion d’utilisateur à un service
 * Écrire un serveur permettant d’afficher le nom et prénom des personnes connectées,
 * suivi du dernier message envoyé par chacun. Ces informations seront renseignées par
 * les clients, qui s’attacheront au segment de mémoire partagée créé par le serveur. Le
 * client dispose du menu suivant :
 * 1. Rejoindre le serveur
 * 2. Envoyer un message
 * 3. Quitter le serveur
 * Le serveur mettre à jour l’affichage à chaque fois que quelqu’un se connecte, se
 * déconnecte ou envoie un message.
*/

#include <iostream>
#include <cstdlib>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <cstring>



#include "configConn.hpp"

// La SHM= pid_serveur pid_client choix  buffer
//         int         int        int    char*
// buffer contient des infos selon le choix 
// choix= 1 => buffer= nom+prenom
// choix= 2 => buffer= message


using namespace std;

pid_t pid_serveur=0,pid_client= getpid();
bool connected= false;
char *out={}; // SHM
Choix choix;

void afficher_menu()
{
    system("clear");
    cout<<"*******Menu*******"<<endl;
    cout<<"1. Rejoindre le serveur"<<endl;
    cout<<"2. Envoyer un message"<<endl;
    cout<<"3. Quitter le serveur"<<endl;
}


int rejoindre_serveur()
{
    // si l'utilisateur est deja conneté au serveur.
    if(connected)
        return 0;
    
    int shmid= shmget((key_t)SHM_KEY, 0, S_IRUSR | S_IWUSR);
    if(shmid == -1) 
    {
        cerr<<"Erreur shmget"<<std::endl;
        return 1;
    }
    
    out=(char*)shmat(shmid, NULL, 0);
    if((int*)out == (int*)-1)
    {
        cerr<<"Erreur shmgat:"<<errno<<std::endl;
        return 2;
    }
    connected= true;
    // Lire le pid du server
    memcpy(&pid_serveur,out,sizeof(int));

    // Ecrire les infos client dans la SHM
    char nom[MAX_SIZE],prenom[MAX_SIZE];

    // Ecrire le pid_client dans la SHM.
    memcpy(out+sizeof(int),&pid_client,sizeof(int));
    // Ecrire le choix dans la SHM.
    choix= Choix::REJONDRE_SERV;
    memcpy(out+(sizeof(int)*2),&choix,sizeof(int));

    cout<<"Entrez votre nom: ";
    cin>>nom;
    cout<<"Entrez votre prenom: ";
    cin>>prenom;

    memcpy(out+(sizeof(int)*3),&nom,sizeof(char)*MAX_SIZE);
    memcpy(out+(sizeof(int)*3)+sizeof(char)*MAX_SIZE,&prenom,sizeof(char)*MAX_SIZE);

    kill(pid_serveur, SIGCONT);
    return 0;
}

int envoyer_message()
{
    if(!connected || pid_serveur == 0)
        return 1;

    // Ecrire le pid_client dans la SHM.
    memcpy(out+Memory::PID_CLIENT,&pid_client,sizeof(int));
    // Ecrire le choix dans la SHM.
    choix= Choix::ENVOYER_MSG;
    memcpy(out+Memory::CHOIX,&choix,sizeof(int));
    // Ecrire le message dans la SHM.
    string msg={};
    cout<<"Entrez votre message: ";
    cin.ignore();
    getline(cin, msg);
    memcpy(out+Memory::MSG,msg.c_str(),sizeof(char)*MAX_SIZE_MSG);

    kill(pid_serveur, SIGCONT);
    return 0;
}

int quitter_serveur()
{
    if(!connected || pid_serveur == 0)
        return 1;
    // Ecrire le pid_client dans la SHM.
    memcpy(out+Memory::PID_CLIENT,&pid_client,sizeof(int));
    // Ecrire le choix dans la SHM.
    choix= Choix::QUITTER_SERV;
    memcpy(out+Memory::CHOIX,&choix,sizeof(int));

    kill(pid_serveur, SIGCONT);
    connected= false;
    return 0;
}



void sigcont_handler(int signum)
{
    if(!connected || pid_serveur == 0)
        return;
    int choix;
    memcpy(&choix,out+Memory::CHOIX,sizeof(int));
    if(choix != Choix::QUITTER_SERV)
        return;
    connected= false;
    cout<<"Serveur est deconnecté"<<endl;
    cout<<"Deconnection du serveur..."<<endl;
    // exit(0);
}

int main()
{
    bool running= true;
    int choix;
    signal(SIGCONT,sigcont_handler);
    while(running)
    {
        choix= 0;
        // Afficher le menu
        afficher_menu();
        cin>>choix;
        // traiter le choix d'utilisateur
        switch(choix)
        {
            case 1:
                if(rejoindre_serveur() != 0)
                {
                    cerr<<"Erreur dans la connection au serveur!!"<<endl;
                    return 1;
                }
                break;
            case 2:
                if(envoyer_message() != 0)
                {
                    cerr<<"Erreur lors de l'envoye du message!!"<<endl;
                    return 2;
                }
                break;
            case 3:
                if(quitter_serveur() != 0)
                {
                    cerr<<"Erreur serveur n'est plus disponible!!"<<endl;
                    return 3;
                }
                break;
            default:
                continue;
        }
    }
    return 0;
}