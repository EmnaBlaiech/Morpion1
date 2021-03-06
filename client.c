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
#include <pthread.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>

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
typedef struct Joueur Joueur;

/*********************Type structuré de la grille ***************************************/
#pragma pack(1)
struct Grille
{
  int grille[8];
  int identifiant;
};
#pragma pack(0)
typedef struct Grille Grille;

//Thread qui écoute si des données doivent être affichées
static void * ecoute(void * socket_desc)
{
  char buffer[3000];
  int * socket = (int *) socket_desc;
  int longueur;
    while (1)
    {
      if((longueur = read(*socket, buffer, (int)sizeof(buffer)))<=0)  // verifier que la chaine n'est pas vide
        exit(1);
      buffer[longueur]='\0';  // limiter la taille du buffer par un caractere de fin de chaine pour n'afficher que la partie remplie
      printf("%s \n", buffer);
    }
}

//Création instance joueur
 Joueur ajouterJoueur () {
   Joueur j;

   j.id_sock = 0; //0 par defaut
   printf("\n\nBienvenue au jeu du Morpion !\n\n");
   printf("Donnez votre pseudo : \n");
   scanf("%s", (j.pseudo));
   printf("\nVous êtes prêt à jouer! \n\n");
   return j;
 }

int main(int argc, char **argv) {

    int 	socket_descriptor; 	/* descripteur de socket */
//		longueur; 		/* longueur d'un buffer utilisé */
    sockaddr_in adresse_locale; 	/* adresse de socket local */
    hostent *	ptr_host; 		/* info sur une machine hote */
    //servent *	ptr_service; 		/* info sur service */
  //  char 	buffer[256];
    char *	prog; 			/* nom du programme */
    char *	host; 			/* nom de la machine distante */
    char	msg[256]; 			/* message envoyé */
    pthread_t 	ecoute_thread;


    if (argc != 2) {
  	perror("usage : client <adresse-serveur>");
  	exit(1);
    }

    prog = argv[0];
    host = argv[1];

    printf("nom de l'executable : %s \n", prog);
    printf("adresse du serveur  : %s \n", host);

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
    if(send(socket_descriptor , &joueur, sizeof(Joueur),0) < 0)
    {
        puts("Transmission joueur impossible. \n");
        return 1;
    }
    //Création thread client
    pthread_create(&ecoute_thread, NULL, ecoute, &socket_descriptor);

    printf("\nA vous de jouer ! Positionnez votre pion (de 0 à 8)");
    fgets(msg,sizeof(msg),stdin);
    msg[strcspn(msg,"\n")]='\0';

    // verifier la saisie de l'utilisateur
  //  while (strcmp(msg,"0")==0 || strcmp(msg,"1")==0 || strcmp(msg,"2")==0 || strcmp(msg,"3")==0 || strcmp(msg,"4")==0 || strcmp(msg,"5")==0 || strcmp(msg,"6")==0 || strcmp(msg,"7")==0 || strcmp(msg,"8")==0)
    do {
         if ((atoi(msg)<0) && (atoi(msg)>8)) {
          printf("Veuillez ressaisir un numéro valide !");

        } else {
          printf("Choix enregistré !");
        }
        printf("\n");
        fgets(msg,sizeof(msg),stdin);
        msg[strcspn(msg,"\n")]='\0';

       //echec envoi serveur
        if(send(socket_descriptor , msg, sizeof(msg),0) < 0)
        {
            puts("Impossible d'enregistrer votre tour\n");
            exit(1);
        }

    } while (strcmp(msg,"x")!=0);

    if (strcmp(msg,"x")==0){

    close(socket_descriptor);

    printf("connexion avec le serveur fermee, fin du programme.\n");

    exit(0);
  }

}
