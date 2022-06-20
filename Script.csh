#!/bin/csh

#Création des variables

# nbrRepartition : la méthode de répartion avec laquelle on va exécuter notre programme
# nbrRepartition = 1 : Répartition cyclique des éléments
# nbrRepartition = 2 : Répartition cyclique des blocs de éléments
# nbrRepartition = 3 : Répartition des blocs d'éléments à la demande (farming)
set nbrRepartition=0


# nbrThreads : Nombre de threads d'addition de 1 à 1024
set nbrThreads=0

#megration : l'autorisation, ou non, de migration entre coeurs : 1 = OUI , 0 = NON  
set megration=0



set i=0

#début de la boucle d'exécution
 while( $i < 30 )
	
	# A chaque itération le programme teste un méthode de répartion , la valeur nbrRepartition =0 est traité dans le main.c 
	@ nbrRepartition = $i % 3
	
	@ nbrThreads = $i + 1
	
	@ megration = $i % 2
		
	#exécution du programme avec les valeurs obtenus ci-dessus
	./main $nbrRepartition $nbrThreads $megration 
	
	@ i++
end


