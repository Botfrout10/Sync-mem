#ifndef CONFIGCONN_SHM
#define CONFIGCONN_SHM

#define NBR_OCTET 200 // 200 char ou 50 int 
#define SHM_KEY 51
#define MAX_SIZE 50 // Maximume d'une string nom et prenom
#define MAX_SIZE_MSG 100 // Maximume d'une string message

enum Choix{
    REJONDRE_SERV= 1,
    ENVOYER_MSG= 2,
    QUITTER_SERV= 3
};
enum Memory{
    PID_SERV= 0,
    PID_CLIENT= sizeof(int),
    CHOIX= PID_CLIENT+sizeof(int),
    MSG= CHOIX+sizeof(int),
    NOM= MSG,
    PRENOM= NOM+sizeof(char)*MAX_SIZE
};


#endif // CONFIG_SHM