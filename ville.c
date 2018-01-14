#include "dilemmelib.h"

int** allocationsquare (int n, int m){
  int** g = malloc(n * sizeof(int *));
  for(size_t i = 0; i < n; i++){
	g[i] = malloc(m * sizeof(int));
  }
  return g;
}

int*** allocationtrid (int n, int m, int k){
  
  int*** g = malloc(n * sizeof(int **));
  for(size_t i = 0; i < n; i++){
	g[i] = malloc(m * sizeof(int *));
	for(size_t j =0; j<m;j++){
	  g[i][j] = malloc(k * sizeof(int));
	}
  }
  return g;
}

int affiche_matrice (int n, int m, int** M) {
  for (int i=0; i<n;i++){
    for (int j=0;j<m;j++){
   printf("%d\t",M[i][j]);
  }
  printf("\n");
 }
 return 0;
}

// c'est la population initiale de chaque stratégies
// ns c'est la nmbr de stratégies autorisées
// n c'est le nombre de génération
int** ville (int n,int p, int* strat_dispo, int ns){
 init_dico();
 int Poptot=ns*p;
 int*** g=allocationtrid(ns,ns,n);
  // c c'est le tableaux avec les valeurs des nombres d'individus
 int** c=allocationsquare(ns,(n+1));
 for (int i=0;i<ns;i++){
  c[i][0]=p;
 }
 for (int i=0;i<n;i++){
  for (int j=0;j<ns;j++) {
   for (int k=0;k<=j;k++){
    int** t=allocationsquare(n,2);
    dico[strat_dispo[j]].fun(t,i,0);
    dico[strat_dispo[k]].fun(t,i,1);
    g[j][k][i]=t[i][0];
    g[k][j][i]=t[i][1];
   }
  }
  
 long G[ns];
 for (int j=0;j<ns;j++){
   G[j]=0;
 }
  int** r=allocationsquare(ns,ns);
  for (int k=0;k<ns;k++){
   for (int j=0;j<ns;j++){
     int p[2];
     p[0]=g[k][j][i];
     p[1]=g[j][k][i];
    int t[2];
    gain(p,t);
    r[k][j]=t[0];
    r[j][k]=t[1];
   }
  }
  // la matrice des gains r est bien remplie
  for (int j=0;j<ns;j++){
   for (int k=0;k<ns;k++){
    (k==j) ? (G[j]+=(c[j][i]-1)*r[j][j]) : (G[j]+=(c[k][i])*r[j][k]) ;
   
   }
  }
  for (int j=0;j<ns;j++){
   G[j]*=c[j][i];
  }
  double s1=0;
  

  for (int j=0;j<ns;j++){
   s1+=G[j];
  }
  int s2=0;
  //je vais remplir maintenant la i+1-ème génération avec des parties entières mais pour ne rien perde pour la dernière
  //stratégie en va faire effectif total - les autres effectifs
  for (int j=0;j<(ns-1);j++){
   double a=Poptot*G[j];
   a=a/s1;
   a = floor ((a)+0.5);
   c[j][i+1]=a;
   s2+=c[j][i+1]; 
  }
  c[ns-1][i+1]=Poptot-s2;
  //il va falloir attendre les informations des autres villes pour savoir où et si on envoie des gens 
   // et donc modifié Poptot et les population de C avant de recommencer une nouvelle boucle
 }
 return c;
}


int main (int argc, char** argv) {
  if (argc!=18) {printf("Il n'y a pas le bon nombre d'arguments "); exit(-1); }
  init_coef( atoi(argv[14]),atoi(argv[15]),atoi(argv[16]),atoi(argv[17]));
  int** p;
  // p=ville ( atoi(argv[1]),atoi(argv[2]),argv[3],atoi(argv[4]));
  int n=0;
  for (int i=0;i<11;i++){ n+=atoi(argv[i+3]);}
  int tab[n];
  int j=0;
  for (int i=0;i<11;i++){(atoi(argv[i+3])!=0) ? (tab[j]=i; j++;) : () ; }	
  p=ville ( atoi(argv[1]),atoi(argv[2]),tab,n); 
  int i=affiche_matrice(n,atoi(argv[1])+1,p);
  return i;
}

// si on arrive pas à passer de tableau un argument faire 11 int de 0 ou 1 et je recréer le tableau dans le main

