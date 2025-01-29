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

// La SHM= pid_serveur pid_client choix  buffer
//         int         int        int    char*
// buffer contient des infos selon le choix 
// choix= 1 => buffer= nom+prenom
// choix= 2 => buffer= message
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>

#include "configConn.hpp"

// TODO: Complete
// Creation d'une map qui lie les pids des clients avec leur nom et prenom (Voir la possibilite d'utiliser un HASHmap)
// Traiter tous les requete client par le signal SIGCONT selon le choix
// Rajouter le traitement SIGINT pour informer tous les clients que le serveur n'est plus disponible

int main()
{
    signal(SIGCONT, sigcont_handler);
    return 0;
}