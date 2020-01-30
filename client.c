/*-----------------------------------------------------------
Client a lancer apres le serveur avec la commande :
client <adresse-serveur> <message-a-transmettre>
------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

typedef struct sockaddr 	sockaddr;
typedef struct sockaddr_in 	sockaddr_in;
typedef struct hostent 		hostent;
typedef struct servent 		servent;

/*******************Type structuré de joueur **************/
#pragma pack(1)
struct Joueur
{
  int id_sock; //id de la socket du client, par defaut elle vaut 0
  char pseudo[20];
  int symbole;  // 1 ou 2
};
#pragma pack(0)
/*********************Type structuré de la grille ***************************************/
#pragma pack(1)
struct Grille
{
  int grille[8];
  int identifiant;
};
#pragma pack(0)
typedef struct Grille Grille;

/********************** Thread qui écoute si des données doivent être affichées */
static void * sniffer(void * socket_desc)
{
  char buffer[3000];
  int * socket = (int *) socket_desc;
  int longueur;
    while (1)
    {  
      if((longueur = read(*socket, buffer, (int)sizeof(buffer)))<=0)
        exit(1);
      buffer[longueur]='\0';
      printf("%s \n", buffer); 
    }
}

//**********************************Création instance joueur
 Joueur ajouterJoueur () {
   Joueur j;
	 
   j.id_sock = 0; //0 par defaut
   printf("\n\n Bienvenue au jeu du Morpion\n\n");
   printf("Donnez votre pseudo : \n");
   scanf("%s", &(j.pseudo));
    printf("\nVous êtes prêt à jouer! \n");
    return j;
 }

int main(int argc, char **argv) {

    int 	socket_descriptor, 	/* descripteur de socket */
		longueur; 		/* longueur d'un buffer utilisé */
    sockaddr_in adresse_locale; 	/* adresse de socket local */
    hostent *	ptr_host; 		/* info sur une machine hote */
    servent *	ptr_service; 		/* info sur service */
    char 	buffer[256];
    char *	prog; 			/* nom du programme */
    char *	host; 			/* nom de la machine distante */
    char *	mesg; 			/* message envoyé */
    pthread_t 	sniffer_thread;


    if (argc != 3) {
	perror("usage : client <adresse-serveur> <message-a-transmettre>");
	exit(1);
    }

    prog = argv[0];
    host = argv[1];
    mesg = argv[2];

    printf("nom de l'executable : %s \n", prog);
    printf("adresse du serveur  : %s \n", host);
    printf("message envoye      : %s \n", mesg);

    if ((ptr_host = gethostbyname(host)) == NULL) {
	perror("erreur : impossible de trouver le serveur a partir de son adresse.");
	exit(1);
    }

    /* copie caractere par caractere des infos de ptr_host vers adresse_locale */
    bcopy((char*)ptr_host->h_addr, (char*)&adresse_locale.sin_addr, ptr_host->h_length);
    adresse_locale.sin_family = AF_INET; /* ou ptr_host->h_addrtype; */
    /*-----------------------------------------------------------*/
    /* SOLUTION 2 : utiliser un nouveau numero de port */
    adresse_locale.sin_port = htons(5000);
    /*-----------------------------------------------------------*/

    printf("numero de port pour la connexion au serveur : %d \n", ntohs(adresse_locale.sin_port));

    /* creation de la socket */
    if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	perror("erreur : impossible de creer la socket de connexion avec le serveur.");
	exit(1);
    }

    /* tentative de connexion au serveur dont les infos sont dans adresse_locale */
    if ((connect(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0) {
	perror("erreur : impossible de se connecter au serveur.");
	exit(1);
    }

    printf("connexion etablie avec le serveur. \n");

    printf("envoi d'un message au serveur. \n");

    Joueur joueur ; 
    joueur = ajouterJoueur();
    joueur.id_sock = socket_descriptor;	
    
    //Transmission du joueur au serveur echoué
    if( send(socket_descriptor , &joueur, sizeof(Joueur),0) < 0)
    {
        puts("Transmission joueur impossible. \n");
        return 1;
    }  
    //Création thread client
    pthread_create(&sniffer_thread, NULL, sniffer, &socket_descriptor);
	
	
    /* lecture de la reponse en provenance du serveur */
    while((longueur = read(socket_descriptor, buffer, sizeof(buffer))) > 0) {
	printf("reponse du serveur : \n");
	write(1,buffer,longueur);
    }
      
      printf("A vous de jouer! Positionnez votre pion (de 0 à 8)");	
    // verifier la saisie de l'utilisateur
      while (strcmp(msg,"0")==0 or strcmp(msg,"1")==0 or strcmp(msg,"2")==0 and strcmp(msg,"3")==0 or strcmp(msg,"4")==0 or strcmp(msg,"5")==0 or strcmp(msg,"6")==0 or strcmp(msg,"7")==0 or strcmp(msg,"8")==0)
     {
      fgets(msg,sizeof(msg),stdin);
      msg[strcspn(msg,"\n")]='\0';
    //echec envoi serveur
        if( send(socket_descriptor , msg, sizeof(msg),0) < 0)
        {
            puts("Impossible d'enregistrer votre tour\n");
            exit(1);
        }
      }

    close(socket_descriptor);

    printf("connexion avec le serveur fermee, fin du programme.\n");

    exit(0);

}
