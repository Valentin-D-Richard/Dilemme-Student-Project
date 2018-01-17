/* Fichier qui gère la création du réseau et lance les appels ssh puis les clients et le serveur */

#include <pthread.h>
#include <unistd.h>
#include "dilemmelib.h"

int NBVILLES;

int min (int a, int b) {return((a<b)?a:b);}

void* routine (void* data) {
  char* s = (char*)data;
  system(s);
  return(NULL);
}

#define REMPLIES 7

int main (int argc, char** argv) {
  if (argc != 5) {printf("usage: <nb_generations> <port> <ip_address> <nb_villes<=23>\n"); exit(-1);}
  NBVILLES = atoi(argv[4]); if (NBVILLES > min(REMPLIES,23)) {printf("Trop de villes\n"); exit(-1);}
  int j;

  // ****************************** Partie modifiable **********************************
  // Attention aux espaces !!!

  char** p; p = malloc(NBVILLES*sizeof(char*));
  for (j = 0; j < NBVILLES; j++) p[j] = malloc(10*sizeof(char));
  char** pop; pop = malloc(NBVILLES*sizeof(char*));
  for (j = 0; j < NBVILLES; j++) p[j] = malloc(23*sizeof(char));
  char** coef; coef = malloc(NBVILLES*sizeof(char*));
  for (j = 0; j < NBVILLES; j++) p[j] = malloc(30*sizeof(char));

  // p                   // pop                               // coef
  p[0] = " 100 ";        pop[0] = "1 1 1 1 1 0 0 0 0 1 1 ";   coef[0] = "5 0 3 1 ";   // Ville 1
  p[1] = " 200 ";        pop[1] = "1 1 1 1 1 1 1 1 1 1 1 ";   coef[1] = "5 0 10 1 ";  // Ville 2
  p[2] = " 100 ";        pop[2] = "0 1 0 0 1 1 1 0 1 1 1 ";   coef[2] = "5 1 1 1 ";   // Ville 3
  p[3] = " 500 ";        pop[3] = "1 1 1 1 1 0 1 0 1 0 1 ";   coef[3] = "3 0 5 4 ";   // Ville 4
  p[4] = " 1000 ";       pop[4] = "1 1 1 1 1 1 1 1 0 1 1 ";   coef[4] = "1 0 3 1 ";   // Ville 5
  p[5] = " 100 ";        pop[5] = "0 0 0 0 1 1 0 0 0 0 0 ";   coef[5] = "5 0 3 1 ";   // Ville 6
  p[6] = " 100 ";        pop[6] = "1 1 1 0 1 0 1 0 1 1 1 ";   coef[6] = "5 0 1 3 ";   // Ville 7

  // ***********************************************************************************

  // Préparation des arguments
  char st[NBVILLES][200];
  char* buff; buff = malloc(2*sizeof(char));
  for (j = 0; j < NBVILLES; j++) {
    strcpy(st[j],"ssh "); if (j < 9) strcat(st[j],"0"); sprintf(buff,"%d",j+1); strcat(st[j],buff);
    strcat(st[j],".dptinfo.ens-cachan.fr \"$PWD/client ");
    strcat(st[j],argv[1]); strcat(st[j],p[j]); strcat(st[j],pop[j]); strcat(st[j],coef[j]);
    strcat(st[j],argv[2]); strcat(st[j]," "); strcat(st[j],argv[3]); strcat(st[j]," ");
    strcat(st[j],buff); strcat(st[j]," > /dev/null\"");
  }

  char s[200];
  strcpy(s,"./server ");
  strcat(s,argv[1]) ; strcat(s," "); strcat(s,argv[2]); strcat(s," "); strcat(s,argv[4]);


  // Lancements parallèles
  pthread_t* pt; pt = malloc(NBVILLES*sizeof(pthread_t)); pthread_t serv;
  pthread_create(&serv,NULL,routine,(void*)s);
  sleep(1);
  for (j = 0; j < NBVILLES; j++) pthread_create(&pt[j],NULL,routine,(void*)st[j]);

  pthread_join(serv,NULL);
  for (j = 0; j < NBVILLES; j++) pthread_join(pt[j],NULL);

  return(0);
}
