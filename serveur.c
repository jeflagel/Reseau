#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>

char * ADD ( char** tab , int nb){ //fonction d'addition d'opérandes présentes sous formes de chaines de caractere dans tab
	int somme = 0;
	int i ;
	char * c1;
	for ( i= 0; i<nb ; i++){
		somme+= atoi(tab[i]);
	}
	sprintf(c1,"%d",somme);
	return c1;                       //retourne le résultat sous forme de chaine
}

char * MUL ( char** tab , int nb){ //fonction de multiplication d'opérandes présentes sous formes de chaines de caractere dans tab
	int mul = 0;
	int i ;
	char * c1;
	for ( i= 0; i<nb ; i++){
		mul*= atoi(tab[i]);
	}
	sprintf(c1,"%d",mul);
	return c1;                 //retourne le résultat sous forme de chaine
}





int main(int argc, char *argv[])   {
	if (argc==2){
		int s_ecoute,scom, lg_app,i,j;
		struct sockaddr_in adr;
		struct sockaddr_storage recep;
		char buf[1500], renvoi[1500], host[1024],service[20];
		char Operation[3];
		int nbOperateur ;
		char ** Operande ;

		s_ecoute=socket(AF_INET,SOCK_STREAM,0);
		printf("creation socket\n");

		adr.sin_family=AF_INET;
		adr.sin_port=htons(atoi(argv[1]));
		adr.sin_addr.s_addr=INADDR_ANY;

		if (bind(s_ecoute,(struct sockaddr *)&adr,sizeof(struct sockaddr_in)) !=0) {
		 printf("probleme de bind sur v4\n");
		 exit(1); }

		if (listen(s_ecoute,5) != 0)      {
		   printf("pb ecoute\n"); exit(1);}

		printf("en attente de connexion\n");
		while (1)  {
		  scom=accept(s_ecoute,(struct sockaddr *)&recep, (unsigned long *)&lg_app);
		  getnameinfo((struct sockaddr *)&recep,sizeof (recep), host, sizeof(host),service, sizeof(service),0);
		   printf("recu de %s venant du port %s\n",host, service);

		   while (1)     {
			   recv(scom,buf,sizeof(buf),0);
				 if (strcmp(buf,"QUI")==0) {
				 	break ;
				 }
			   printf("buf recu %s\n",buf);
			   bzero(renvoi,sizeof(renvoi));
			   strcpy(Operation,strtok(buf," ");
				 nbOperateur = atoi(strtok(" "));
			   if (nbOperateur>1 && (strcmp(Operation,"ADD")==0 || strcmp(Operation,"MUL")==0) ) {
				   strcpy(renvoi,"OK");
			   }
			   else strcpy(renvoi,"TU ES MAUVAIS JACK");
			   send(scom,renvoi,strlen(renvoi),0);
			   bzero(buf,sizeof(buf));
			   // on a renvoyer ok maintenant on attend les opérateurs a traiter et traitement
			   recv(scom,buf,sizeof(buf),0);
			   printf("buf recu %s\n",buf);
			   bzero(renvoi,sizeof(renvoi));
				 Operande = malloc(sizeof(char *) *nbOperateur );
				 for(i=0; i<nbOperateur; i++){
				    Operande[i] = malloc(10 * sizeof(char));
						if (i==0){
							strcpy(Operande[i],strtok(buf,";");
						}
						else strcpy(Operande[i],strtok(";");
				 }
				 switch (Operation) {
				 	case "ADD":
						strcpy(renvoi,ADD( Operande , nbOperateur));
						break;
					case "MUL":
						strcpy(renvoi,MUL( Operande , nbOperateur));
						break;
				 }
			   send(scom,renvoi,strlen(renvoi),0);
			   bzero(buf,sizeof(buf));
			 }
		   close(scom);
		 }
		close(s_ecoute);
	}
}
