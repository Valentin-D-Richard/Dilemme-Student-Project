#include "dilemmelib.h"

bool** affr(void strat1(bool**,int,int), void strat2(bool**,int,int), int n) {
  bool** g = malloc(n*sizeof(bool*));
  for (int i; i < n; i++) { strat1(g,i,0) ; strat2(g,i,1); }
  return(g);
}

int main (int argc, char** argv) {
  bool** g = affr(gentille,gentille,1) ;
  printf("Gentille vs Gentille : %d à %d",g[0][0],g[0][1]);
  return(0);
  }
