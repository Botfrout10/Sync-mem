/**
 * Sujet 3 : Gestion de la connexion d’utilisateur à un service
 * Écrire un serveur permettant d’afficher le nom et prénom des personnes connectées,
 * suivi du dernier message envoyé par chacun. Ces informations seront renseignées par
 * les clients, qui s’attacheront au segment de mémoire partagée créé par le serveur. Le
 * client dispose du menu suivant :
 * 1. Rejoindre le serveur
 * 2. Envoyer un message
 * 3. Quitter le serveur
 * Le serveur mettra à jour l’affichage à chaque fois que quelqu’un se connecte, se
 * déconnecte ou envoie un message.
*/
<<<<<<< HEAD
=======

// La SHM= pid_serveur pid_client choix  buffer
//         int         int        int    char*
// buffer contient des infos selon le choix 
// choix= 1 => buffer= nom+prenom
// choix= 2 => buffer= message
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <unordered_map>
#include <cstring>
#include <sys/stat.h>
#include <utility>  // for std::pair



#include "configConn.hpp"

using namespace std;

// TODO: Complete
// Creation d'une map qui lie les pids des clients avec leur nom et prenom (Voir la possibilite d'utiliser un HASHmap)
// Traiter tous les requete client par le signal SIGCONT selon le choix
// Rajouter le traitement SIGINT pour informer tous les clients que le serveur n'est plus disponible

char *out;
pid_t pid_serveur= getpid();
typedef pair<string,string> user_t;
// La map contient l'pid du client comme clé et le paire nom, prenom (user_t) comme valeur 
unordered_map<pid_t,user_t> users;


void afficher_utilisateur(pid_t pid_client)
{
    auto it = users.find(pid_client);
    if (it != users.end())
        cout << "[PID: " << it->first << "] {Nom: " << it->second.first << ", Prenom: " << it->second.second<<"}" << endl;
    else
        cerr<<"Erreur inatendue utilisateur n'exsite pas, PID:"<<pid_client<<endl;
}

void souscrire_client(pid_t pid_client)
{
    // user_t user_info{nom,prenom};
    // users[pid_client]= user_info;
    // users[pid_client]= make_pair(nom, prenom);

    char nom[MAX_SIZE], prenom[MAX_SIZE];
    memcpy(nom,out+Memory::NOM,sizeof(char)*MAX_SIZE);
    memcpy(prenom,out+Memory::PRENOM,sizeof(char)*MAX_SIZE);

    // Stocker l'utilisateur dans la var users
    // TODO: Fix this.
    users[pid_client]= {nom, prenom};
   
    // Afficher l'utilisateur souscrit
    afficher_utilisateur(pid_client);
    // cout<<'['<<pid_client<<"] : {"<<nom<<", "<<prenom<<'}'<<endl;

}

void lire_message_client(pid_t pid_client)
{
    if(users.find(pid_client) == users.end())
    {
        // TODO:
        // Gerer le cas ou l'utilisateur n'est pas souscit et arrive a envoyer un message,
        // vue que l'utilisateur est bloquer d'envoyer un message si il n'est pas souscrit
        cerr<<"Client n'existe pas: "<<pid_client<<endl;
        return;
    }

    char msg[MAX_SIZE_MSG];
    memcpy(msg,out+Memory::MSG,sizeof(char)*MAX_SIZE_MSG);

    // afficher le client avec le message
    afficher_utilisateur(pid_client);
    cout<<'"'<<msg<<'"'<<endl;
    // const auto& user= users[pid_client];
    // cout<<'['<<pid_client<<"] : {"<<user.first<<", "<<user.second<<"} \""<<msg<<'\"'<<endl;
}

void detache_client(pid_t pid_client)
{
    size_t users_count= users.size();
    afficher_utilisateur(pid_client);
    users.erase(pid_client);
    if(users_count == users.size() )
    {
        cerr<<"Erreur lors de la suppression du client!!"<<endl;
        exit(1);
    }
    cout<<"Client supprimer avec success:"<<endl;
    // const auto& user= users[pid_client];
    // cout<<'['<<pid_client<<"] : {"<<client.first<<", "<<client.second<<'}'<<endl;
}

void sigcont_handler(int signum)
{
    pid_t pid_client;
    int choix;

    // Lire le pid client depuis la SHM.
    memcpy(&pid_client, out+Memory::PID_CLIENT,sizeof(int));
    // Lire le choix du client depuis la SHM.
    memcpy(&choix,out+Memory::CHOIX,sizeof(int));
    // Traiter la requete client.
    switch (choix)
    {
    case Choix::REJONDRE_SERV:
        souscrire_client(pid_client);
        break;
    case Choix::ENVOYER_MSG:
        lire_message_client(pid_client);
        break;
    case Choix::QUITTER_SERV:
        detache_client(pid_client);
        break;
    default:
        break;
    }
}

void sigstop_handler(int signum)
{
    if(users.size() == 0)
    {
        cout<<"Aucun utilisateur est connecté au serveur"<<endl;
        return;
    }
    cout<<"Liste des utilisateurs connectés:"<<endl;
    for(const auto& [pid_client,usr_info] : users)
    {
        const auto& [nom,prenom]= usr_info;
        // cout<<'['<<pid_client<<"] : {"<<usr_info.first<<", "<<usr_info.second<<'}'<<endl;
        cout<<'['<<pid_client<<"] : {"<<nom<<", "<<prenom<<'}'<<endl;
    }
}

int main()
{
    // Traitement de signal
    signal(SIGCONT, sigcont_handler);
    // Traitment du signal pour afficher tous les utilisateurs.
    signal(SIGUSR1, sigstop_handler);
    // Allocation de la memoire partage SHM
    int shmid= shmget((key_t)SHM_KEY, sizeof(char)*NBR_OCTET,IPC_CREAT| S_IRUSR | S_IWUSR);
    if(shmid == -1)
    {
        cerr<<"Erreur shmget"<<endl;
        exit(1);
    }
    // Acces au pointeur de la SHM
    out= (char*)shmat(shmid,NULL,0);
    if((int*)out == (int*)-1)
    {
        cerr<<"Erreur shmat"<<endl;
        exit(1);
    }
    // Ecrire le pid du serveur dans la SHM.
    memcpy(out,&pid_serveur, sizeof(int));

    cout<<"Le pid serveur:"<<pid_serveur<<endl;
    cout<<"Waiting for request..."<<endl;
    while(true)
    {
        pause();
    }

    return 0;
}
>>>>>>> 063bc21243eb649e5da3160bfd6f32c35eef44a7
