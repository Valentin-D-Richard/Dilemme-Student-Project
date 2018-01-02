
#include "dilemmelib.h"
#include <stdio.h>
#include <math.h>
 
// n c'est le nombre de générations et p c'est la population à donner à chaque stratégie
//et nb le nombre de confrontations pour les gains des strats

int** ecosysteme ( int n, int p, int nb){
 init_dico();
 int** g[11][11];
 for (int i=0,i<11,i++) {
  for (int j=0,j<=i,j++){
  //il faut faire une fonction qui sort le gain totale d'une confrontation pour remplir g
  //avec g[i][j] est le gain de la strat i contre la strat j
  }
 }
 // c c'est le tableaux avec les valeurs des nombres d'individus
 int** c[11][n];
 for (int i=0,i<11,i++){
  c[i][0]=p;
 }
 for (int i=1,i<n, i++){
  int* P[11];
  for (int j=0,j<11,j++){
   for (int k=0,k<11,k++){
    (k==j) ? (P[j]+=(c[j][i-1]-1)*g[j][j]) : (P[j]+=(c[j][i-1])*g[j][k]) ;
   }
  }
  for (int j=0,j<11,j++){
   P[j]*=c[j][i-1];
  }
  int s1=0;
  for (int j=0,j<11,j++){
   s1+=P[j];
  }
  int s2=0;
  //je vais remplir maintenant la i-ème génération avec des parties entières mais pour ne rien perde pour la dernière
  //stratégie en va faire effectif total - les autres effectifs
  for (int j=0,j<10,j++){
   c[j][i]=floor(11*p*(P[j]/s1));
   s2+=c[j][i]; 
  }
  c[10][i]=11*p-s2;
 }
 return c;
}
   
    
   
   
   
