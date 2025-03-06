# ifndef SERVER_CONFIG
# define SERVER_CONFIG


# define TYPE_MSG_SERVER 1
# define TYPE_MSG_BROADCAST 2
# define QUEUE_KEY 10

#include <string>

using pid_client= long;

struct Message
{
    pid_client mtype; // Le type de message sera l'identificateur du client, c'est le PID
    std::string mtext;
};

struct Client
{
    pid_client pid;
    std::string pseudo;
};


enum class Messages_type
{
    // List,
    Msg,
};


# endif