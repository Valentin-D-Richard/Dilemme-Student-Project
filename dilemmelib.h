// Bibliothèque des fonctions utiles aux trois parties du projet

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ***** Fonctions utiles pour toutes les parties

#define T 5
#define D 0
#define C 3
#define P 1

// L'action d'une joueur est un entier : 1 signifie coopérer et 0 trahir

void gain(int *p, int* t) {
  if (p[0] && p[1])  { t[0] = C; t[1] = C; } else 
    if (p[0] && !p[1]) { t[0] = D; t[1] = T; } else
      if (!p[1] && p[0]) { t[0] = T; t[1] = D; } else
	{ t[0] = P; t[1] = P; } 
}

// Une stratégie est un fonction du type void strat(int**  g, int i, int r)
// où g est la matrice des affrontements contre un adversaire,
//    i indique le numéro du coup
//    r indique le numéro du joueur
// stat modifie g[i][r]

void gentille (int** g, int i, int r) {
  g[i][r] = 1 ;
}

void mechante (int** g, int i, int r) {
  g[i][r] = 0 ;
}

void donnant_donnant (int** g, int i, int r) {
  (i == 0) ? (g[0][r] = 1) : (g[i][r] = g[i-1][1-r]) ;
}

void mefiante (int** g, int i, int r) {
  (i == 0) ? (g[0][r] = 0) : (g[i][r] = g[i-1][1-r]) ;
}

void periodique_mechante (int** g, int i, int r){
	(i%3==2) ? (g[i][r]=1) : (g[i][r]=0);
}

void periodique_gentille (int** g, int i, int r){
	(i%3==2) ? (g[i][r]=0) : (g[i][r]=1);
}

void majorite_mou (int** g, int i, int r){
	(i==0) ? (g[i][r] = 1):
	int s =maj(g,1-r);
	(s>=1) ? (g[i][r] = 1) : (g[i][r] = 0);
}

void majorite_dur (int** g, int i, int r){
	(i==0) ? (g[i][r] = 0):
	int s =maj(g,1-r);
	(s>1) ? (g[i][r] = 1) : (g[i][r] = 0);
}
// ***** Fonctions générales *****

typedef struct dictionnaire { char* name ; void (*fun)(int**,int,int); } dictionnaire ;

#define N 8 // Nombre de stratégies
dictionnaire dico[N]; // Dictionnaire d'associations

void init_dico() {
  dico[0].name = "gentille" ; dico[0].fun = &gentille ;
  dico[1].name = "mechante" ; dico[1].fun = &mechante ;
  dico[2].name = "donnant_donnant" ; dico[2].fun = &donnant_donnant ;
  dico[3].name = "mefiante" ; dico[3].fun = &mefiante ;
  dico[4].name = "periodique_mechante" ; dico[4].fun = &periodique_mechante ;
dico[5].name = "periodique_gentille" ; dico[5].fun = &periodique_gentille ;
dico[6].name = "majorite_mou" ; dico[6].fun = &majorite_mou ;
dico[7].name = "majorite_dur" ; dico[7].fun = &majorite_dur ;
}

int assoc(char* nom) {
// Associe le numéro i tel que dico[i] représente la fonction nom
// Renvoie -1 si pas d'association
  for (int i = 0; i < N; i++) if (!strcmp(nom,dico[i].name)) return(i) ;
  return(-1);
}

//calcul la majorité dans un tableau jusqu'à l'indice i on suppose i<=taille tableau et r la ligne qu'on regarde
 int maj(int** g, int r){
	 int s
for (int j=0,j<(i-1),j++){
	s=s+g[j][r];
}
(s==(i/2)) ? (return 2) : (s>(i/2)) ? (return 1) : (return 0);
 }
	
