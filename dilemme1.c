#include "dilemmelib.h"

void affr(void strat1(int**,int,int), void strat2(int**,int,int), int** g, int n) {
  // Génère n affrontement de strat1 contre strat2
  for (int i; i < n; i++) { strat1(g,i,0) ; strat2(g,i,1); }
}

void confr(int argc, char** argv) {
  // Affiche une confrontation de argv[2] contre argv[3]
  if (argc != 4) { printf("usage : confr <strat1> <strat2>\n"); exit(-1); }
  else {
    int i1,i2;

    i1 = assoc(argv[2]); i2 = assoc(argv[3]);
    if (i1 == -1 || i2 == -1) (printf("Wrong name of startegy\n"), exit(-1)) ;

    int* g[1];
    g[0] = malloc(2*sizeof(int)) ;
    affr(*dico[i1].fun,*dico[i2].fun,g,1) ;
    print_affr(g,1,i1,i2);

    int win[2];
    gain(g[0],win);
    printf("%s vs %s : gain %d à %d\n",dico[i1].name,dico[i2].name,win[0],win[1]);
  }
}

void res_confrs(int argc, char** argv) {
  // Affiche le tableau des listes des actions pour chaque couple de stratégies, sur argv[2] parties
  int n = atoi(argv[2]);
  int**** tab; // t = tab[i1][i2] est la tableau de la confrotation de i1 contre i2
               // t[i][r] est l'action de i(r+1) au coup i 
  tab = malloc(N*sizeof(int****));

  printf("\t");
  for (int j = 0; j < N; j++) {
    printf("%d",j);
    repeat(" ",(n < 10) ? n : (n-1));
  }
  printf("\n");
    
  for (int i1 = 0; i1 < N; i1++) {
    tab[i1] = malloc(N*sizeof(int***)) ;
    printf("%d\t",i1);

    for (int i2 = 0; i2 < N; i2++) {

      tab[i1][i2] = malloc(n*sizeof(int*));
      for (int i = 0; i < n; i++) tab[i1][i2][i] = malloc(2*sizeof(int)) ;

      affr(*dico[i1].fun,*dico[i2].fun,tab[i1][i2],n);
      print_cmpct(tab[i1][i2],n,0);
    }
    printf("%s\n",dico[i1].name);
  }
}
  
void tab_cumul(int argc, char** argv) {}
 

int main (int argc, char** argv) {
  init_dico();
  if (argc <= 2) {printf("usage : confr/res/tab <arg1> [<arg2>]\n"); exit(-1);}
  else {
    switch(argv[1][0]) {
    case 'c' : confr(argc,argv); break;
    case 'r' : res_confrs(argc,argv); break;
    case 't' : tab_cumul(argc,argv); break;
    }
    return(0);
  }
}
