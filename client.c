#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>





void main(int argc, char *argv[])
{
  char buffer[200],Operation[200],Operande[10];
  int port, rc, sock,i,nbOperande,j;
  char c,choix;
  struct sockaddr_in addr;
  struct hostent *entree;
  int SaisieVal = 0 ;

  if (argc !=3)  {
      printf("usage : clientv4 nom_serveur port\n");
	exit(1);    }

  addr.sin_port=htons(atoi(argv[2]));
  addr.sin_family=AF_INET;
  entree=(struct hostent *)gethostbyname(argv[1]);
  bcopy((char *)entree->h_addr,(char *)&addr.sin_addr,entree->h_length);

  sock= socket(AF_INET,SOCK_STREAM,0);

  if (connect(sock, (struct sockaddr *)&addr,sizeof(struct sockaddr_in)) < 0) {
    printf("probleme connexion\n");
    exit(1); }

  printf("connexion passe\n");

  while (1) {
      bzero(Operation,sizeof(Operation));
      bzero(buffer,sizeof(buffer));
      SaisieVal = 0 ;

      while (!SaisieVal){
        printf("Que voulez vous faire : \n");
        printf("Tapez 1 pour ADD : \n");
        printf("Tapez 2 pour MUL : \n");
        printf("Tapez 3 pour QUIT : \n");
        choix=getchar();//on recupere le choix de l'utilisateur (1 caractere)
        SaisieVal = 1 ;
        switch (choix) {
          case '1':
            strcpy(Operation,"ADD ");
            break ;
          case '2':
            strcpy(Operation,"MUL ");
            break ;
          case '3':
            strcpy(Operation,"QUI");
            break ;
          default :
            SaisieVal = 0 ;
        }
      }
      if (strcmp("QUI",Operation) == 0){ // si QUI alors on envoi fin au serveur et on arrete tout
        send(sock,Operation,strlen(Operation)+1,0);
        break;
      }
      i = 4;
      printf("Combien de nombre ? : \n");  // apres choix de l'operation on demande saisie du nb d'opérandes
      while((c=getchar()) != '\n'){
			Operation[i++]=c;
      }
      send(sock,Operation,strlen(Operation)+1,0);
      recv(sock,buffer,sizeof(buffer),0);
      printf("recu %s\n",buffer);
      if (strcmp(buffer,"OK")==0){  // si le serveur est ok on saisit les opérandes
        strtok(Operation," ");
        nbOperande=atoi(strtok(NULL,";"));  // on récupére le nb d'opérandes saisi précedemment
        bzero(Operation,sizeof(Operation));
        bzero(buffer,sizeof(buffer));
        for (i=0 ; i<nbOperande;i++){  // on récupére les opérandes que l'on concatene a la Chaine opération avec des ; pour les separer
          printf("saisissez le %d nombre\n", i+1);
          j=0 ;
          while((c=getchar()) != '\n'){
    			Operande[j++]=c;
          }
          strcat(Operation,Operande);
          strcat(Operation,";");
          bzero(Operande,sizeof(Operande)); //on nettoie Operande pour la prochaine saisie
        }
        send(sock,Operation,strlen(Operation)+1,0);
        recv(sock,buffer,sizeof(buffer),0);
        printf("resultat de l'opération %s\n",buffer); // on affiche le résultat de l'opération
      }
    }

  close(sock); }
