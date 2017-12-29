// Bibliothèque des fonctions utiles aux trois parties du projet

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define T 5
#define D 0
#define C 3
#define P 1

// L'action d'une joueur est un booléen : true signifie coopérer et false trahir

int* gain(bool a, bool b) {
  int t[2];
  (a && b) ? (t[0] = C, t[1] = C) : () ;
  (a && !b) ? (t[0] = D, t[1] = T) : () ;
  (!a && b) ? (t[0] = T, t[1] = D) : () ;
  (!a && !b) ? (t[0] = P, t[1] = P) : () ;
  return(t);
}

// Une stratégie est un fonction du type void strat(bool**  g, int i, int r)
// où g est la matrice des affrontements contre un adversaire,
//    i indique le numéro du coup
//    r indique le numéro du joueur
// stat modifie g[i][r]

void gentille (bool** g, int i, int r) {
  g[i][r] = true ;
}

void mechante (bool** g, int i, int r) {
  g[i][r] = false ;
}

void donnant_donnant (bool** g, int i, int r) {
  (i == 0) ? (g[0][r] = true) : (g[i][r] = g[i-1][1-r]) ;
}

void mefiante (bool** g, int i, int r) {
  (i == 0) ? (g[0][r] = false) : (g[i][r] = g[i-1][1-r]) ;
}

