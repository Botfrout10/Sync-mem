# Travail à faire:
- Plusieurs clients doivent pouvoir s’échanger des messages par l’intermédiaire d’un
serveur relai. 
- Chaque client est connu côté serveur grâce à son `pid`, ainsi qu’un `pseudo`.
- Le client peut réaliser les actions suivantes :
    1. Demander au serveur la liste des pseudos connectés au service de chat ;
    2. Envoyer un message à un client particulier par l’intermédiaire du serveur ;
    3. Envoyer un message à l’ensemble des clients ;
    4. Demander une déconnexion du serveur lors de sa fermeture.
- Remarque : On programmera ce chat en C++. L’identification des clients au niveau
du serveur sera géré à l’aide d’une map.
