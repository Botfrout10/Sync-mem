# Conception:

## Travail à faire:
- Plusieurs clients doivent pouvoir s’échanger des messages par l’intermédiaire d’un
serveur relai. Chaque client est connu côté serveur grâce à son pid, ainsi qu’un pseudo.
Le client peut réaliser les actions suivantes :
Demander au serveur la liste des pseudos connectés au service de chat ;
Envoyer un message à un client particulier par l’intermédiaire du serveur ;
Envoyer un message à l’ensemble des clients ;
Demander une déconnexion du serveur lors de sa fermeture.
Remarque : On programmera ce chat en C++. L’identification des clients au niveau
du serveur sera géré à l’aide d’une map.

## Breif:
- On va creer une application qui va permettre aux **clients** de s'echanger des messages au d'autres clients par l'inetrmediere du **serveur**.
- Le client sera identifier par son pid, qui sera aussi le numero qui identifie ces messages.

## Serveur:
- Le serveur va s'occupper l'abstraction de la communication avec **la file des message**, et il va permettre:
    1. la connection des clients
    2. la deconnection des clients
    3. l'envoie des messages d'un client à un autre client.
    4. L'envoie des messages d'un client à tous les clients.

## Client:
- Le client va pouvoir facilement envoyer des messages à un seul client ou tous les clients vue l'absatraction fait au niveau du serveur.