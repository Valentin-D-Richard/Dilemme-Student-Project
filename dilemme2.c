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



// n c'est le nombre de générations et p c'est la population à donner à chaque stratégie
//et nb le nombre de confrontations pour les gains des strats

int** ecosysteme ( int n, int p, int nb){
 init_dico();
 int** g=allocationsquare(11,11); // cela va être la matrice des gains
 int** m=allocationsquare(nb,2);
 int win[2];
 for (int i=0;i<11;i++) {
  for (int j=0;j<=i;j++){
   affr(dico[i].fun, dico[j].fun, m, nb, win);
   g[i][j]=win[0];
   g[j][i]=win[1];
  
  }
 }
 // c c'est le tableaux avec les valeurs des nombres d'individus
 int** c=allocationsquare(11,(n+1));
 for (int i=0;i<11;i++){
  c[i][0]=p;
 }
 for (int i=1;i<n+1; i++){
  long G[11];
  for (int j=0;j<11;j++){
   G[j]=0;
 }
  for (int j=0;j<11;j++){
   for (int k=0;k<11;k++){
    (k==j) ? (G[j]+=(c[j][i-1]-1)*g[j][j]) : (G[j]+=(c[k][i-1])*g[j][k]) ;
   }
  }
  for (int j=0;j<11;j++){
   G[j]*=c[j][i-1];
  }
  double s1=0;
  for (int j=0;j<11;j++){
   s1+=G[j];
  }
  int s2=0;
  //je vais remplir maintenant la i-ème génération avec des parties entières mais pour ne rien perde pour la dernière
  //stratégie en va faire effectif total - les autres effectifs
  for (int j=0;j<10;j++){
   double a=11*p*G[j];
   a=a/s1;
   a=floor ((a)+0.5);
   c[j][i]=a;
   s2+=c[j][i]; 
  }
  c[10][i]=11*p-s2;
 }
 return c;
}
   
// il y a une deuxième façon de voir la question  qui est de faire à chaque fois une seule confrontation    

int** ecosysteme2 ( int n, int p){
 init_dico();
 int*** g=allocationtrid(11,11,n);
  // c c'est le tableaux avec les valeurs des nombres d'individus
 int** c=allocationsquare(11,(n+1));


 for (int i=0;i<11;i++){
  c[i][0]=p;
 }
 for (int i=0;i<n;i++){
  for (int j=0;j<11;j++) {
   for (int k=0;k<=j;k++){
    int** t=allocationsquare(n,2);
    dico[j].fun(t,i,0);
    dico[k].fun(t,i,1);
    g[j][k][i]=t[i][0];
    g[k][j][i]=t[i][1];
   }
  }
  
 long G[11];
 for (int j=0;j<11;j++){
   G[j]=0;
 }
  int** r=allocationsquare(11,11);
  for (int k=0;k<11;k++){
   for (int j=0;j<11;j++){
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
  for (int j=0;j<11;j++){
   for (int k=0;k<11;k++){
    (k==j) ? (G[j]+=(c[j][i]-1)*r[j][j]) : (G[j]+=(c[k][i])*r[j][k]) ;
   
   }
  }
  for (int j=0;j<11;j++){
   G[j]*=c[j][i];
  }
  double s1=0;
  

  for (int j=0;j<11;j++){
   s1+=G[j];
  }
  int s2=0;
  //je vais remplir maintenant la i+1-ème génération avec des parties entières mais pour ne rien perde pour la dernière
  //stratégie en va faire effectif total - les autres effectifs
  for (int j=0;j<10;j++){
   double a=11*p*G[j];
   a=a/s1;
   a = floor ((a)+0.5);
   c[j][i+1]=a;
   s2+=c[j][i+1]; 
  }
  c[10][i+1]=11*p-s2;
 }
 return c;
}
   



   
 int main (int argc, char** argv) {
  if (argc!=3) {printf("Il n'y a pas le bon nombre d'arguments baka"); exit(-1); }

  int** p;
  p=ecosysteme2 ( atoi(argv[1]),atoi(argv[2]));
  int i=affiche_matrice(11,atoi(argv[1])+1,p);
  return i;
 }

  
