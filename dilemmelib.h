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

// ***** Fonctions générales *****

typedef struct dictionnaire { char* name ; void (*fun)(int**,int,int); } dictionnaire ;

#define N 4 // Nombre de stratégies
dictionnaire dico[N]; // Dictionnaire d'associations

void init_dico() {
  dico[0].name = "gentille" ; dico[0].fun = &gentille ;
  dico[1].name = "mechante" ; dico[1].fun = &mechante ;
  dico[2].name = "donnant_donnant" ; dico[2].fun = &donnant_donnant ;
  dico[3].name = "mefiante" ; dico[3].fun = &mefiante ;
}

int assoc(char* nom) {
// Associe le numéro i tel que dico[i] représente la fonction nom
// Renvoie -1 si pas d'association
  for (int i = 0; i < N; i++) if (!strcmp(nom,dico[i].name)) return(i) ;
  return(-1);
}
