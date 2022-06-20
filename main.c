#include<math.h>
#include<unistd.h>
//#include<file.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<sys/types.h>
#include <pthread.h>

/*Taille des tableaux : 100 000 000*/
#define MAX_SIZE 100000000


/*Déclaration globale des variables */ 

// Premier tableau à addidtionner
double *Tableau1;

// Deuxième tableau à addidtionner
double *Tableau2;

// Tableau de résultat
double *TableauDeResultat;

// La somme totale de l'addition entre les deux tableaux
double SommeTotale;


pthread_t * threads;
int CompteurCourant;

//Nombre de threads d'addition : 1 à 1024
int NbrThreads_D_addition;

FILE *f;  
char filename[255]; 



/* La fonction de l'addition */
void* Fun_Addition(void* arg)
{

  	
  int ThreadCourant = CompteurCourant++;
  
  /* Répartition des éléments par thread*/
  int i = ThreadCourant * (MAX_SIZE / NbrThreads_D_addition);
  
  /* Addition des élements par les threads */
   for (i ; i < (ThreadCourant + 1) * (MAX_SIZE / NbrThreads_D_addition); i++)
        TableauDeResultat[ThreadCourant] += Tableau1[i] + Tableau2[i];
    
    pthread_exit("NULL");
}



/* Répartition cyclique des éléments ; */
void RepartitionCycliqueElements()
{
	int i=0;

	//Création des Threads
	for ( i= 0; i < NbrThreads_D_addition; i++)
        	pthread_create(&threads[i], NULL, Fun_Addition, (void*)NULL);

	for ( i= 0; i < NbrThreads_D_addition; i++)
		pthread_join(threads[i], NULL);

	for ( i= 0; i < NbrThreads_D_addition; i++)
        	SommeTotale += TableauDeResultat[i];


	printf("La somme des deux tableaux est :%f \n", SommeTotale);

}




/* Répartition des blocs d'éléments à la demande ( farming ). */
void RepartitionParFarming()
{
	int pid =0;
	pid = fork();

	if(pid ==0)
	{
		int i=0;
		for ( i= 0; i < NbrThreads_D_addition; i++)
        		pthread_create(&threads[i], NULL, Fun_Addition, (void*)NULL);

		for ( i= 0; i < NbrThreads_D_addition; i++)
			pthread_join(threads[i], NULL);

		for ( i= 0; i < NbrThreads_D_addition; i++)
        		SommeTotale += TableauDeResultat[i];


		printf("La somme des deux tableaux est :%f \n", SommeTotale);

	}

}



/* Faire l'addition entre les tableaux avec non authorisation du migration entre les coeurs */
void MigrationNoAutorise()
{
	int i=0;
	double TableauDeResultat=0.0;
	for(i; i<MAX_SIZE; i++)
	{
		SommeTotale +=Tableau1[i] + Tableau2[i];
	}


}


/***************************************************************************************** Fonction principal le Main()*******************************************************************/
int main(int argc, char* argv[])
{

/* Declaration des variables */
CompteurCourant =0;
int MethodDeRepartition =0;
int AutorisationMigration;

/* Mesure le temps d'exécution */
struct timespec start, finish;
double elapsed;


/* Création du fichier CSV */
sprintf((char *) &filename,"ficher.csv");  

/* Ouverture du ficher en mode écriture et insértion à la fin */
f = fopen(filename, "a");


/* Le script C-shell génere 2 valeurs (0,1) pour le deuxième paramètre (index 0) (Méthode de répartition) */
MethodDeRepartition = atoi(argv[1]) + 1;

/**
* nbrRepartition = 1 : Répartition cyclique des éléments
* nbrRepartition = 3 : Répartition des blocs d'éléments à la demande (farming)
* nbrRepartition = 3 : Répartition des blocs d'éléments à la demande (farming)
*/
	if(MethodDeRepartition == 2 || MethodDeRepartition == 3)  
		MethodDeRepartition=3;

/*le troixème paramétre (index 2 ) represente le nombre de threads d'additions */
NbrThreads_D_addition = atoi(argv[2]); 


/*le quatrième paramétre (index 3 ) represente la migration */
AutorisationMigration = atoi(argv[3]);

/* Début du calcul de temps d'exécution */
clock_gettime(CLOCK_MONOTONIC, &start);

/* au début l'utulisateur doit saisir 4 paramétre ./FileExecutable LaMéthodeDeRépartistion LeNombreDeThreadAddition Migration */
if(argc == 4)
{

	/* Allocations dynamique de la mémoire */
	Tableau1 = (double *) malloc(MAX_SIZE * sizeof(double));
	Tableau2 = (double *) malloc(MAX_SIZE * sizeof(double));
	TableauDeResultat = (double *) malloc(MAX_SIZE * sizeof(double));
	threads = (pthread_t *) malloc(NbrThreads_D_addition * sizeof(pthread_t)); 

	/* Remplir des deux tableaux par des valeurs aléatoires */
	int i=0; 
	for( i = 0 ; i < MAX_SIZE ; i++ ) {
      		Tableau1[i]=   (rand() / 100);
      		Tableau2[i]=   (rand() / 50);
      	}

 
	 /* Début du calcul selon les paramétres insérés */
	if(AutorisationMigration == 0)
	{
		MigrationNoAutorise();
	}

	else if(AutorisationMigration == 1){
		switch (MethodDeRepartition){
	
			case 1 : RepartitionCycliqueElements();
			case 3 : RepartitionParFarming();	
		}
	}

	/* Fin du calcul */
	clock_gettime(CLOCK_MONOTONIC, &finish);

	/* calcul le la différence entre le temps du début et le temps de la fin */
	elapsed = (finish.tv_sec - start.tv_sec);

	/*Affichage du temps d'exécution */
	printf("le temps d'exécution est : %.2f secondes \n",elapsed);
}


if (f) { 
	    /* écricre les resultats obtenus (paramétres , temps d'exéxution) dans le fichier .CSV */
            fprintf(f,"%f.2, %d, %d, %d\n", elapsed,MethodDeRepartition, NbrThreads_D_addition ,AutorisationMigration);  
            fclose(f);  
        } 
/* Lébération de la mémoire */
free(Tableau1);
free(Tableau2);
free(TableauDeResultat);

}

