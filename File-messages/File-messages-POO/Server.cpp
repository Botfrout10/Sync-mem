# include <iostream>
# include <map>


# include "ServerConfig.hpp"
# include "Client.cpp"

using namespace std;
class Server
{

private:
    
    

    void createQueue()
    {
        
    }
public:
    Server()
    {
        createQueue();
    }
    ~Server()= default;


    void connect(const Client& client);
    // void connect(pid_t pid);
    void disconnect(const Client& client);
    void envoyerMessage(const Client& se)
};

