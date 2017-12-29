#include "dilemmelib.h"

void affr(void strat1(int**,int,int), void strat2(int**,int,int), int** g, int n) {
  for (int i; i < n; i++) { strat1(g,i,0) ; strat2(g,i,1); }
}

int main (int argc, char** argv) {
  init_dico();
  if (argc != 4) { printf("usage : <strat1> <strat2> <nb iterations>\n"); return(-1); }
  else {
    int i1,i2,n;

    i1 = assoc(argv[1]); i2 = assoc(argv[2]);
    if (i1 == -1 || i2 == -1) (printf("Wrong name of startegy\n"), exit(-1)) ;
    n = atoi(argv[3]);

    int** g; g = malloc(n*sizeof(int*));
    for (int i = 0; i < n; i++) g[i] = malloc(2*sizeof(int)) ;
    affr(*dico[i1].fun,*dico[i2].fun,g,n) ;
    printf("%s vs %s : %d à %d\n",dico[i1].name,dico[i2].name,g[n-1][0],g[n-1][1]);

    int win[2];
    gain(g[n-1],win);
    printf("%s vs %s : gain %d à %d\n",dico[i1].name,dico[i2].name,win[0],win[1]);
    return(0);
  }
}
