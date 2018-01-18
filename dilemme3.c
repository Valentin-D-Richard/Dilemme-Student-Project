/* Fichier qui gère la création du réseau et lance les appels ssh puis les clients et le serveur */

#include <pthread.h>
#include <unistd.h>
#include "dilemmelib.h"

int NBVILLES;

void* routine (void* data) {
  char* s = (char*)data;
  system(s);
  return(NULL);
}

int main (int argc, char** argv) {
  if (argc != 5) {printf("usage: <nb_generations> <port> <ip_address> <nb_villes<=23>\n"); exit(-1);}
  NBVILLES = atoi(argv[4]); if (NBVILLES > 23) {printf("Trop de villes\n"); exit(-1);}
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
  p[7] = " 100 ";        pop[7] = "0 1 1 1 1 1 0 0 0 1 1 ";   coef[7] = "5 0 3 1 ";   // Ville 8
  p[8] = " 200 ";        pop[8] = "1 1 1 1 1 0 1 1 0 1 1 ";   coef[8] = "5 2 6 1 ";   // Ville 9
  p[9] = " 100 ";        pop[9] = "0 1 0 0 1 1 1 0 1 1 1 ";   coef[9] = "5 1 1 10 ";  // Ville 10
  p[10]= " 500 ";        pop[10]= "1 1 1 1 1 0 1 0 0 0 1 ";   coef[10]= "3 0 5 4 ";   // Ville 11
  p[11]= " 100 ";        pop[11]= "1 1 1 1 0 1 1 1 0 1 1 ";   coef[11]= "1 2 3 1 ";   // Ville 12
  p[12]= " 50 ";         pop[12]= "0 0 1 0 1 1 0 0 0 0 0 ";   coef[12]= "6 0 3 1 ";   // Ville 13
  p[13]= " 100 ";        pop[13]= "1 1 1 0 1 0 0 0 1 1 1 ";   coef[13]= "5 0 8 3 ";   // Ville 14
  p[14]= " 100 ";        pop[14]= "1 1 0 1 1 0 0 0 1 1 1 ";   coef[14]= "3 0 3 1 ";   // Ville 15
  p[15]= " 300 ";        pop[15]= "1 1 1 1 1 1 1 1 0 1 1 ";   coef[15]= "5 0 5 1 ";   // Ville 16
  p[16]= " 100 ";        pop[16]= "0 1 0 0 1 0 1 0 0 0 1 ";   coef[16]= "5 1 1 6 ";   // Ville 17
  p[17]= " 500 ";        pop[17]= "1 1 0 1 1 0 1 0 1 0 0 ";   coef[17]= "3 0 4 4 ";   // Ville 18
  p[18]= " 800 ";        pop[18]= "0 1 1 1 1 0 1 1 0 1 1 ";   coef[18]= "1 0 3 8 ";   // Ville 19
  p[19]= " 100 ";        pop[19]= "1 0 1 0 1 1 0 0 0 0 0 ";   coef[19]= "5 9 3 1 ";   // Ville 20
  p[20]= " 100 ";        pop[20]= "1 0 1 0 1 0 1 0 0 1 1 ";   coef[20]= "5 0 5 3 ";   // Ville 21
  p[21]= " 700 ";        pop[21]= "0 1 1 1 1 1 0 1 0 1 1 ";   coef[21]= "5 0 6 1 ";   // Ville 22
  p[22]= " 200 ";        pop[22]= "1 1 0 1 1 0 1 0 0 1 1 ";   coef[22]= "5 0 6 1 ";   // Ville 23

  /* On rappelle l'ordre des stratégies
     0  gentille
     1  mechante
     2  donnant_donnant
     3  mefiante
     4  periodique_gentille
     5  periodique_mechante
     6  majorite_moi
     7  majorite_dur
     8  sondeur
     9  donnant_donnant_dur
     10 rancuniere
  */
  // ***********************************************************************************

  // Préparation des arguments
  char st[NBVILLES][200];
  char* buff; buff = malloc(2*sizeof(char));
  for (j = 0; j < NBVILLES; j++) {
    strcpy(st[j],"ssh "); if (j < 9) strcat(st[j],"0"); sprintf(buff,"%d",j+1); strcat(st[j],buff);
    strcat(st[j],".dptinfo.ens-cachan.fr \"$PWD/client ");
    strcat(st[j],argv[1]); strcat(st[j],p[j]); strcat(st[j],pop[j]); strcat(st[j],coef[j]);
    strcat(st[j],argv[2]); strcat(st[j]," "); strcat(st[j],argv[3]); strcat(st[j]," ");
    strcat(st[j],buff); strcat(st[j],"\"");
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
