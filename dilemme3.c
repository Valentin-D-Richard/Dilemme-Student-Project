/* Fichier qui gère la création du réseau et lance les appels ssh puis les clients et le serveur */

#include <pthread.h>
#include "dilemmelib.h"

#define NBVILLES 2

void* routine (void* data) {
  char* s = (char*)data;
  system(s);
  return(NULL);
}

int main (int argc, char** argv) {
  if (argc != 2) {printf("usage: <nb_generations>\n"); exit(-1);}

  char* p = " 100 ";
  char* pop1 = "1 1 1 1 1 0 0 0 0 1 1 ";
  char* pop2 = "1 1 1 1 1 1 1 1 1 1 1 ";

  char* coef1 = "5 0 3 1\"";
  char* coef2 = "5 0 10 1\"";

  char s1[200];
  strcpy(s1,"ssh richard@11.dptinfo.ens-cachan.fr \"$PWD/client ");
  strcat(s1,argv[1]); strcat(s1,p); strcat(s1,pop1); strcat(s1,coef1);

  char s2[200];
  strcpy(s2,"ssh richard@12.dptinfo.ens-cachan.fr \"$PWD/client \"");
  strcat(s2,argv[1]); strcat(s2,p); strcat(s2,pop2); strcat(s2,coef2);

  char s[200];
  strcpy(s,"ssh richard@10.dptinfo.ens-cachan.fr \"$PWD/server ");
  strcat(s,argv[1]) ; strcat(s,"\"");

  pthread_t* pt; pt = malloc((NBVILLES+1)*sizeof(pthread_t));
  pthread_create(&pt[0],NULL,routine,(void*)s);
  pthread_create(&pt[1],NULL,routine,(void*)s1);
  pthread_create(&pt[2],NULL,routine,(void*)s2);

  pthread_join(pt[0],NULL);
  pthread_join(pt[1],NULL);
  pthread_join(pt[2],NULL);

  return(0);
}
