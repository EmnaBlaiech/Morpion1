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
  int joue;
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
Grille grille;

// procédure initialise la grille à 0
void initialiser_grille (Grille m){
  int i;
  //Grille m;

  for(i=0;i<=8;i++){
          m.matrice[i] = 0 ;
          //printf("%d ", m.matrice[i]);
    }
  }

char * transforme (int a) {   //transforme les chiffres de la  grille en caractere pour l'affichage de la grille
  if (a == 1) {return "X";}
  else {
    if (a == 4){return "O";}
    else { return "-";}
}
}

char * afficher_grille (Grille m) { //affichage de la grille du morpion
  char* str = malloc(10*256*sizeof(char));
  char* tmp = malloc(256*sizeof(char));

    strcpy(str, "\nVoici la grille\n______________\n");
    sprintf(tmp,"| %s | %s | %s |\n",transforme(m.matrice[0]),transforme(m.matrice[1]),transforme(m.matrice[2]));
    strcat(str,tmp);
    sprintf(tmp,"| %s | %s | %s |\n",transforme(m.matrice[3]),transforme(m.matrice[4]),transforme(m.matrice[5]));
    strcat(str,tmp);
    sprintf(tmp,"| %s | %s | %s |\n",transforme(m.matrice[6]),transforme(m.matrice[7]),transforme(m.matrice[8]));
    strcat(str,tmp);
    sprintf(tmp,"______________\n");
    strcat(str,tmp);

    return str;
}

int verifier_match_null(Grille g) {
  int match_null = 1; //booleen à vrai
  int j;

  //calculer la somme des lignes, colonnes et diagonales
  for (j=0;j<=8;j++){
    if (g.matrice[j] == 0) {
      match_null = 0; //booleen à faux
    }
  }
  return match_null;
}

int verifier_gagnant(Grille m2){
  int gagnant = 0; //booleen à faux
  int scores[7]; // ligne1, ligne2, ligne3, col1, col2, col3, diag1, diag2
  int k;

  scores[0] = m2.matrice[0] + m2.matrice[1] + m2.matrice[2];  //ligne1
  scores[1] = m2.matrice[3] + m2.matrice[4] + m2.matrice[5];  //ligne2
  scores[2] = m2.matrice[6] + m2.matrice[7] + m2.matrice[8];  //ligne3
  scores[3] = m2.matrice[0] + m2.matrice[3] + m2.matrice[6];  //col1
  scores[4] = m2.matrice[1] + m2.matrice[4] + m2.matrice[7];  //col2
  scores[5] = m2.matrice[2] + m2.matrice[5] + m2.matrice[8];  //col3
  scores[6] = m2.matrice[0] + m2.matrice[4] + m2.matrice[8];  //diag1
  scores[7] = m2.matrice[2] + m2.matrice[4] + m2.matrice[6];  //diag2
  k=0;
  do{
    if (scores[k] == 3){
      gagnant = 1 ; //booleen à vrai
      printf("Bravo ! Le gagnant est le joueur 2 ! :)");
    } else {
      if (scores[k] == 12){
        gagnant = 1 ; //booleen à vrai
        printf("Bravo ! Le gagnant est le joueur 1 ! :)");
      }
    }
      k++;
  } while (gagnant == 1 || k<=7); //booleen à vrai
return gagnant;
}

int getCurrentPlayerIndex(int id_user) {
  int i = 0;
  while (JoueursCo[i].id_sock!=id_user)
  {
    i++;
  }
  return i;
}

/*------------------------------------------------------*/
void *GererJoueur(void *socket_descriptor) {
    Joueur joueur;

    //Get the socket descriptor
    int sock = *(int*)socket_descriptor;
    int read_size=0, current_index;
    char *message , client_message[256];
    int choix;

    //Recevoir le message du client
    if((read_size = recv(sock , &joueur , sizeof(Joueur) , 0) > 0 ))
    {
       //enregistrer le client dans le tableau des clients
       current_index = getCurrentPlayerIndex(sock);
       joueur.id_sock=sock;
       JoueursCo[current_index] = joueur;

       if (current_index == 0){
       		JoueursCo[current_index].symbole = 1;
            printf("current index 0. \n");
       }
       else {
         if (current_index == 1)
       		{
            JoueursCo[current_index].symbole = 4;
              printf("current index 1. \n");
          }
       }
       //printf("%s ",message);
     //write(sock,message,strlen(message)+1);
    }
    else
    {
      perror("Erreur : impossible d'enregistrer le client sur le server.");
      exit(1);
    }


int init = 0;

while (1){

    if (init == 0) {
      initialiser_grille(grille);
      message =  strcat(strcat(strcat(JoueursCo[current_index].pseudo," vous avez le symbole "),transforme(JoueursCo[current_index].symbole)),afficher_grille(grille));
      if (write(sock,message,strlen(message))>0){
          printf("grille envoyée. \n");
      } else {
          printf("grille non envoyee. \n");
      }
      init = 1;
    }
    do{
        printf("debut communication c/s \n");
    	  read_size = recv(sock, client_message, sizeof(client_message),0);  // lire le message client
        if (sock == JoueursCo[current_index].id_sock){  //gerer le jeu à tour de role
          JoueursCo[current_index].joue =1; // notifier qu'il joue
          for(int j=0;j<2;j++){ // changer la valeur de jour de l'autre joueur
            if(JoueursCo[j].id_sock != sock){JoueursCo[j].joue =0;}
          }
    	  client_message[read_size]='\0';  // garder juste ce qui est utile dans le message client
        if (strcmp(client_message,"")!= 0){
          choix = atoi(client_message); 	// convertir le message en entier
          printf("choix saisi %d \n", choix);
          if (grille.matrice[choix] == 0){
            init = 2;
          }
        } else
        {
          printf ("je suis dans else \n");
        }
      }
      } while (init !=2) ;

      printf ("Fin do while \n");
      printf("current index %d \n", current_index);
      printf("%d \n",(int) JoueursCo[current_index].symbole);
      for (int i=0;i<9;i++){
        printf("\n matrice %d vaut %d",i,grille.matrice[i]);
      }
      grille.matrice[choix] = JoueursCo[current_index].symbole;  //positionne le pion du joueur courant
      printf("\n gm %d \n ", (int) grille.matrice[choix]);
      printf("\nchoix %d\n",choix);
      //envoyer la grille à jour
      message = afficher_grille(grille);

      if(write(sock,message,strlen(message)+1)<0){

              printf ("Impossible d'énvoyer la nouvelle grille \n");
      } else {
          if (verifier_gagnant(grille)!=1){
            if (verifier_match_null(grille)==1){
              write(sock,"\nla partie est finie , match null! \n",strlen("\nla partie est finie , match null! \n")+1);
              // deconnecter les deux clients
            }
          } else {
            sprintf (message,"\nPartie terminée! Joueur %s a gagné, bravo!\n",JoueursCo[current_index].pseudo);
            write(sock,message,strlen(message)+1);
            // deconnecter les deux clients
          }
      }

    }

    return 0;
}


/*------------------------------------------------------*/
int main(int argc, char **argv) {

    int 		socket_descriptor, 		/* descripteur de socket */
    *new_sock,
			nouv_socket_descriptor; 	/* [nouveau] descripteur de socket */
	socklen_t  longueur_adresse_courante; 	/* longueur d'adresse courante d'un client */
    sockaddr_in 	adresse_locale, 		/* structure d'adresse locale*/
			adresse_client_courant; 	/* adresse client courant */
    hostent*		ptr_hote; 			/* les infos recuperees sur la machine hote */
    //servent*		ptr_service; 			/* les infos recuperees sur le service de la machine */
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
initialiser_grille(grille);

/* attente des connexions et traitement des donnees recues */
longueur_adresse_courante = sizeof(adresse_client_courant);
while((nouv_socket_descriptor = accept(socket_descriptor,(sockaddr*)(&adresse_client_courant),&longueur_adresse_courante))) {
/* traitement du message */
printf("Connexion acceptée.\n");
//affectation de la valeur du socket au user
	JoueursCo[nombre_joueurs].id_sock = nouv_socket_descriptor;

    // creation de thread
     new_sock = malloc(1);
	*new_sock = nouv_socket_descriptor; //identifiant de la socket
     if (pthread_create(&thread, NULL, GererJoueur, (void*) new_sock)<0) {
	    perror("Erreur concernant la création du thread");
	    return 1;
     }

// a chaque connexion on attribue un symbole à chaque joueur
        nombre_joueurs++;
    	printf("Thread accordé au client !\n");	//changer message
}

    if (nouv_socket_descriptor < 0)
    {
       perror("erreur : impossible d'accepter la connexion avec le client.");
       exit(1);
    }
    return 0;
}
