/*----------------------------------------------
Serveur à lancer avant le client
------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h> 	/* pour les sockets */
#include <sys/socket.h>
#include <netdb.h> 		/* pour hostent, servent */
#include <string.h> 		/* pour bcopy, ... */
#include <unistd.h> //Header file for sleep()
#define TAILLE_MAX_NOM 256
#include <pthread.h>

// A normal C function that is executed as a thread
// when its name is specified in pthread_create()
typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

/*******************Type structuré de joueurs **************/
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
  int matrice[8];
  int identifiant;
};
#pragma pack(0)
typedef struct Grille Grille;

Joueur JoueursCo[3]; //tableau des joueurs connectés maximum 4 joueurs, 2 par partie
int nombre_joueurs=0;

// procédure initialise la grille à 0
void initialiser_grille (){
  int i;

  for(i=0;i<=8;i++){
          grille[i] = 0 ;
          printf("%d ", grille[i]);
    }
  }

void afficher_grille{ //affichage de la grille du morpion
    int x;

    for (x=0;x<=8;x++){
      println("_______________");
      println("| "+transforme(grille.matrice[0])+" | "+transforme(grille.matrice[1])+" | "+transforme(grille.matrice[2])+" | ");
      println("_______________");
      println("| "+transforme(grille.matrice[3])+" | "+transforme(grille.matrice[4])+" | "+transforme(grille.matrice[5])+" | ");
      println("_______________");
      println("| "+transforme(grille.matrice[6])+" | "+transforme(grille.matrice[7])+" | "+transforme(grille.matrice[8])+" | ");
      println("_______________");

    }
}

char transforme (int a) {   //transforme les chiffres de la  grille en caractere pour l'affichage de la grille
  if (a == 1) {return "X";}
  else {return "O";}
}

boolean verifier_match_null(){
  boolean match_null = true;
  int j;
  //calculer la somme des lignes, colonnes et diagonales
  for (j=0;j<=8;j++){
    if (grille[j] == 0){
      match_null = false;
    }
  }
  return match_null;
}

bool verifier_gagnant(){
  bool gagnant = false;
  int scores[7]; // ligne1, ligne2, ligne3, col1, col2, col3, diag1, diag2
  int k;

  scores[0] = grille.matrice[0] + grille.matrice[1] + grille.matrice[2];  //ligne1
  scores[1] = grille.matrice[3] + grille.matrice[4] + grille.matrice[5];  //ligne2
  scores[2] = grille.matrice[6] + grille.matrice[7] + grille.matrice[8];  //ligne3
  scores[3] = grille.matrice[0] + grille.matrice[3] + grille.matrice[6];  //col1
  scores[4] = grille.matrice[1] + grille.matrice[4] + grille.matrice[7];  //col2
  scores[5] = grille.matrice[2] + grille.matrice[5] + grille.matrice[8];  //col3
  scores[6] = grille.matrice[0] + grille.matrice[4] + grille.matrice[8];  //diag1
  scores[7] = grille.matrice[2] + grille.matrice[4] + grille.matrice[6];  //diag2
  k=0;
  do{
    if (scores[k] == 3){
      gagnant = true ;
      printf("Bravo ! Le gagnant est le joueur 2 ! :)")
    } else {
      if (scores[k] == 12){
        gagnant = true ;
        printf("Bravo ! Le gagnant est le joueur 1 ! :)")
      }
    }
      k++;
  } while (gagnant == true || k<=7);
return gagnant;
}

/*------------------------------------------------------*/
void *GererJoueur(void *socket_descriptor) {
    Joueur joueur;
    //Get the socket descriptor
    int sock = *(int*)socket_descriptor;
    int read_size=0, current_index;
    char *message , client_message[256];

    //Recevoir le message du client
    if(read_size = recv(sock , &joueur , sizeof(Joueur) , 0) > 0 )
    {
       //enregistrer le client dans le tableau des clients
       current_index = getCurrentUserIndex(sock);
       joueur.id_sock=sock;
       JoueursCo[current_index] = joueur;
    }
    else
    {
      perror("Erreur : impossible d'enregistrer le client sur le server.");
      exit(1);
    }
while (1)
{
  
  affiche_grille(grille);   //affciher l'état de la grille actuel

do{	  
	  read_size = recv(sock, client_message, sizeof(client_message),0);
	  client_message[read_size]='\0';
	  choix = atoi(client_message); 	// convertir le message en entier
	  
  } while ((choix <=8 || choix >=0) && (grille.matrice[choix] = 0) ;
  grille.matrice[choix] = joueur.symbole;  //positionne le pion du joueur courant
  message = grille !!!! envoyer grille a jour!
 write(sock,message,strlen(message)+1);
}
}
	
}


/*------------------------------------------------------*/
main(int argc, char **argv) {

    int 		socket_descriptor, 		/* descripteur de socket */
			nouv_socket_descriptor, 	/* [nouveau] descripteur de socket */
			longueur_adresse_courante; 	/* longueur d'adresse courante d'un client */
    sockaddr_in 	adresse_locale, 		/* structure d'adresse locale*/
			adresse_client_courant; 	/* adresse client courant */
    hostent*		ptr_hote; 			/* les infos recuperees sur la machine hote */
    servent*		ptr_service; 			/* les infos recuperees sur le service de la machine */
    char 		machine[TAILLE_MAX_NOM+1]; 	/* nom de la machine locale */
    pthread_t 		thread ;  			/* creation de thread  */
    gethostname(machine,TAILLE_MAX_NOM);		/* recuperation du nom de la machine */

    /* recuperation de la structure d'adresse en utilisant le nom */
    if ((ptr_hote = gethostbyname(machine)) == NULL) {
		perror("erreur : impossible de trouver le serveur a partir de son nom.");
		exit(1);
    }

    /* initialisation de la structure adresse_locale avec les infos recuperees */

    /* copie de ptr_hote vers adresse_locale */
    bcopy((char*)ptr_hote->h_addr, (char*)&adresse_locale.sin_addr, ptr_hote->h_length);
    adresse_locale.sin_family		= ptr_hote->h_addrtype; 	/* ou AF_INET */
    adresse_locale.sin_addr.s_addr	= INADDR_ANY; 			/* ou AF_INET */

    /*-----------------------------------------------------------*/
    /* SOLUTION 2 : utiliser un nouveau numero de port */
    adresse_locale.sin_port = htons(5000);
    /*-----------------------------------------------------------*/

    printf("numero de port pour la connexion au serveur : %d \n",
		   ntohs(adresse_locale.sin_port) /*ntohs(ptr_service->s_port)*/);

    /* creation de la socket */
    if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("erreur : impossible de creer la socket de connexion avec le client.");
		exit(1);
    }

    /* association du socket socket_descriptor à la structure d'adresse adresse_locale */
    if ((bind(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0) {
		perror("erreur : impossible de lier la socket a l'adresse de connexion.");
		exit(1);
    }

/* initialisation de la file d'ecoute */
listen(socket_descriptor,5);
initialiser_grille();

    /* attente des connexions et traitement des donnees recues */
longueur_adresse_courante = sizeof(adresse_client_courant);
while(nouv_socket_descriptor = accept(socket_descriptor,(sockaddr*)(&adresse_client_courant),&longueur_adresse_courante)) {
/* traitement du message */
printf("Connexion acceptée.\n");
//affectation de la valeur du socket au user
JoueursCo[nombre_joueurs].id_sock = nouv_socket_descriptor;

	    
    // creation de thread
     if (pthread_create(&thread1, NULL, thread_1, NULL)) {
    perror("pthread_create");
    return 1;
     }
// a chaque connexion on attribue un symbole à chaque joueur
      nombre_joueurs++;
        if (nombre_joueurs%2 == 0){
        joueur.symbole = 1; // eq à X
      } else
        joueur.symbole = 4; // eq à O
    }


    }
	   
    if (nouv_socket_descriptor < 0)
    {
       perror("erreur : impossible d'accepter la connexion avec le client.");
        exit(1);
    }
    return 0;
}
