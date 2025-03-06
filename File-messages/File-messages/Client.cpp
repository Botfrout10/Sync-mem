#include <iostream>
#include <string>
#include <sys/msg.h>
#include <unistd.h>

#include "ServerConfig.hpp"
#include "Server.cpp"

using namespace std;


/**
 * @brief chaque client reçoit les message de son pid
*/
string handleMessage(int msgid, pid_client client)
{
    Message msg{};
    msgrcv(msgid, &msg,sizeof(Message), client,0);
    return msg.mtext;
}

void printMenu()
{
    cout<<"Menu"<<endl;
    cout<<"1) Connect"<<endl;
    cout<<"2) Disconnect "<<endl;
    cout<<"3) List client"<<endl;
    cout<<"4) send message"<<endl;
    cout<<"5) Quitter"<<endl;
}


int main()
{
    bool run= true;
    int choix= 5;
    string pseudo;
    while(run)
    {
        printMenu();
        cin>>choix;
        switch (choix)
        {
        case 1:

            cout<<"Entrez votre pseudo:"<<endl;
            cin>>pseudo;
            connect(getpid(),pseudo)? cout<<"Vous êtes connecté"<<endl:cout<<"Vous êtes déja connecté"<<endl;
            break;
        case 2:
            /* code */
            break;
        case 3:
            /* code */
            break;
        case 4:
            /* code */
            break;
        case 5:
            run= false;
            break;
        
        default:
            run= false;
            break;
        }
    }
    return 0;
}