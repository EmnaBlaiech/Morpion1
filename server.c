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
int joueurs[3];
int grille[8];


/*------------------------------------------------------*/
void renvoi (int sock) {

    char buffer[256];
    int longueur;

    if ((longueur = read(sock, buffer, sizeof(buffer))) <= 0)
    	return;

    printf("message lu : %s \n", buffer);

    buffer[0] = 'R';
    buffer[1] = 'E';
    buffer[longueur] = '#';
    buffer[longueur+1] ='\0';

    printf("message apres traitement : %s \n", buffer);

    printf("renvoi du message traite.\n");


    /* mise en attente du prgramme pour simuler un delai de transmission */
    sleep(3);

    write(sock,buffer,strlen(buffer)+1);

    printf("message envoye. \n");

    return;

}
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
      println("| "+transforme(grille[0])+" | "+transforme(grille[1])+" | "+transforme(grille[2])+" | ");
      println("_______________");
      println("| "+transforme(grille[3])+" | "+transforme(grille[4])+" | "+transforme(grille[5])+" | ");
      println("_______________");
      println("| "+transforme(grille[6])+" | "+transforme(grille[7])+" | "+transforme(grille[8])+" | ");
      println("_______________");

    }
}

char transforme (int a) {   //transforme les chiffres de la  grille en caractere pour l'affichage de la grille
  if (a == 1) {return "X";}
  else {return "O";}
}

void saisie_tour(int joueur){
  int choix;  //stocke la réponse du joueur

  affiche_grille(grille);   //affciher l'état de la grille actuel
  printf("Au tour du joueur %d de positionner son pion %d ", joueur , joueurs[joueur] );
//  listen(socket_descriptor,5); // attente du message du client
//  choix = message du client // lire le message et stocker la reponse !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  do{
    print("veuillez choisir une case vide entre 0 et 8 ");
//    listen(socket_descriptor,5); // attente du message du client
//    choix = message du client // lire le message et stocker la reponse !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  } while ((choix <=8 || choix >=0) && (grille[choix] = 0) ;
  grille[choix] = joueurs[joueur];  //positionne le pion du joueur courant
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

  scores[0] = grille[0] + grille[1] + grille[2];  //ligne1
  scores[1] = grille[3] + grille[4] + grille[5];  //ligne2
  scores[2] = grille[6] + grille[7] + grille[8];  //ligne3
  scores[3] = grille[0] + grille[3] + grille[6];  //col1
  scores[4] = grille[1] + grille[4] + grille[7];  //col2
  scores[5] = grille[2] + grille[5] + grille[8];  //col3
  scores[6] = grille[0] + grille[4] + grille[8];  //diag1
  scores[7] = grille[2] + grille[4] + grille[6];  //diag2
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

// traitement dans le thread
void *thread_1(void *arg)
{
    printf("Nous sommes dans le thread.\n");

    /* Pour enlever le warning */
    (void) arg;
    pthread_exit(NULL);
}


/*------------------------------------------------------*/

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
    pthread_t thread1;  /* creation de thread  */
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

    /* 2 facons de definir le service que l'on va utiliser a distance */
    /* (commenter l'une ou l'autre des solutions) */

    /*-----------------------------------------------------------*/
    /* SOLUTION 1 : utiliser un service existant, par ex. "irc" */
    /*
    if ((ptr_service = getservbyname("irc","tcp")) == NULL) {
		perror("erreur : impossible de recuperer le numero de port du service desire.");
		exit(1);
    }
    adresse_locale.sin_port = htons(ptr_service->s_port);
    */
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

    /* attente des connexions et traitement des donnees recues */
    for(;;) {
    cpt = 0;  //compter le nombre de connexion/clients
		longueur_adresse_courante = sizeof(adresse_client_courant);

		/* adresse_client_courant sera renseigné par accept via les infos du connect */
		if ((nouv_socket_descriptor =
			accept(socket_descriptor,
			       (sockaddr*)(&adresse_client_courant),
			       &longueur_adresse_courante))
			 < 0)
       {
			perror("erreur : impossible d'accepter la connexion avec le client.");
			exit(1);
		}
    else { // a chaque connexion on attribue un symbole à chaque joueur
      cpt++;
        if (cpt%2 == 0){
        joueurs[cpt] = 1; // eq à X
      } else
        joueurs[cpt] = 4; // eq à O
    }

    // creation de thread
     if (pthread_create(&thread1, NULL, thread_1, NULL)) {
    perror("pthread_create");
    return EXIT_FAILURE;
     }

     // gestion de l'attente de fin de thread
     if (pthread_join(thread1, NULL)) {
    perror("pthread_join");
    return EXIT_FAILURE;
    }

		/* traitement du message */
		printf("reception d'un message.\n");

		renvoi(nouv_socket_descriptor);

		close(nouv_socket_descriptor);

    }
    return 0;
}
