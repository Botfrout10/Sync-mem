# include <iostream>
# include <map>
# include <sys/types.h>
#include <sys/msg.h>

# include "ServerConfig.hpp"

using namespace std;


map<pid_client,string> _listClient;

/**
 * @return le message id
*/
int createMassagesQueue()
{
    return msgget(QUEUE_KEY,0); // Question?? Comment creer la file des messages une seule fois meme si on lance le programme * fois?
}


// @returns false if the client is already connected
bool connect(pid_client pidClient,const string& pseudoClient)
{
    const auto [it,inserted]=_listClient.emplace(pidClient,pseudoClient);
    return inserted;
}

bool disconnect(pid_client pidClient)
{
    return static_cast<bool>(_listClient.erase(pidClient));
}

bool isConnected(pid_client client)
{
    return _listClient.find(client) != _listClient.end() ;
}



/**
 * @brief Le serveur prend le message(juste le text) utiliser par le client (sender) et ecrit le directement dans la file des message avec le pid du client (receiver)
 * @pre les doivent client doivent être connectés.
*/
bool sendMessage(int msgid, pid_client sender,pid_client receiver, const string& mtext)
{
    if( !isConnected(sender) )
        return false;

    if(receiver != TYPE_MSG_BROADCAST && !isConnected(receiver))
        return false;


    Message msg={receiver,static_cast<char>(Messages_type::Msg)+mtext};

    try
    {
        msgsnd(msgid,&msg,sizeof(msg),0);
    }
    catch(const exception& e)
    {
        cerr << e.what() << endl;
        return false;
    }
    

    return true;
}

void listClients(int msgid, pid_client client)
{
    for(const auto& [pidC,pseudo]: _listClient)
        cout<<pidC<<":"<<pseudo<<endl;
}









