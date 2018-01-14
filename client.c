#include <netdb.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <ulimit.h>

#include "packets.h"
#include "dilemmelib.h"

// port must be the same as in server
#define PORT 2553
#define HOSTNAME "10.dptinfo.ens-cachan.fr"

#define MAXFILES (ulimit(4))


// This connects to the server at the given port and returns a socket
// for communication.
int connect_to_server ()
{
	// Create a TCP socket
	int s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s < 0) fatal_error("Client: No socket available!");

	// Find the IP address of the host machine (where the server is
	// supposed to run).
	struct hostent *host_address = gethostbyname(HOSTNAME);
	if (!host_address) fatal_error("Unknown host");

	// Build the address from hostname and port.
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	memcpy(&server_address.sin_addr,
		host_address->h_addr_list[0],
		host_address->h_length);
	server_address.sin_port = htons(PORT);

	// Connect to the server
	if (connect(s,(struct sockaddr *)&server_address,
                     sizeof server_address) < 0)
		fatal_error("Client: Cannot connect to server");

	return s;
}

void reseau(int i, int** c,int* strat_dispo, int nb_strat, int socket) {
  // Envoie l'état de la population au serveur à la génération i
  // et reçois l'immigration des population (entiers relatif)
  fd_set readfds;
  char* msg;
  int* pop; pop = malloc(N*sizeof(int));
  for (int i1 = 0; i1 < N; i1++) pop[i1] = -1;
  for (int i1 = 0; i1 < nb_strat; i1++) pop[strat_dispo[i1]] = c[i1][i+1];
  int* immigr; immigr = malloc(N*sizeof(int));
  // immigr[i1] contient le nombre d'habitant arrivant dans la ville i de la stratégie i1

  //Envoi de la situation de la ville
  msg = give_pop(pop);
  send_string(socket,msg);
  free(msg);

  //printf("Réception\n");
  // Réception de la population globale et mise à jour
  int b = 1;
  while (b) {
    FD_ZERO(&readfds);
    FD_SET(socket,&readfds);	// .. from server
    if (FD_ISSET(socket,&readfds)) {
      receive_packet(socket,(void**)&msg);
      printf("Message reçu : %s\n",msg);
      b = 0;
    }
    printf("Immigrations\n");
    get_pop(msg,immigr);
    for (int i1 = 0; i1 < nb_strat; i1++) c[i1][i+1] += immigr[strat_dispo[i1]];
    sleep(1); // on attend un peu le serveur
  }
}

int** ville (int n,int p, int* strat_dispo, int nb_strat,int socket){
  // Effectue la simulation d'une ville où les nb_strat stratégies strat_dispo sont autorisées
  // sur n générations avec pour population initiale p par stratégie (autorisée)

  printf("Initialisation\n");
  int i,i1,i2;
  int Poptot = nb_strat * p;
  int*** g; g = malloc(nb_strat*sizeof(int**));
  for (i1 = 0; i1 < nb_strat; i1++) {
    g[i1] = malloc(nb_strat*sizeof(int*));
    for (i2 = 0; i2 < nb_strat; i2++) g[i1][i2] = malloc(n*sizeof(int));
  }
  //g[i1][i2] contient le tableau de réposes de i1 dans sa confrontation avec i2
  printf("la\n");
  // c[i1][i] contient la population de la strat i1 à la génération i
  int** c; c = malloc(nb_strat*sizeof(int*));
  for (i1 = 0; i1 < nb_strat; i1++) c[i1] = malloc((n+1)*sizeof(int));
  for (i1 = 0; i1 < nb_strat; i1++) c[i1][0] = p;

  int** t; t = malloc(n*sizeof(int)); //tableau buffer
  for (i = 0; i < n; i++) t[i] = malloc(2*sizeof(int));

  for (i = 0; i < n; i++) {

    printf("*** Génération %d ***\n",i);
    // tranche des affrontements
    for (i1 = 0; i1 < nb_strat; i1++) {
      for (i2 = 0; i2 <= i1; i2++) {

        dico[strat_dispo[i1]].fun(t,i,0);
        dico[strat_dispo[i2]].fun(t,i,1);
        g[i1][i2][i] = t[i][0];
        g[i2][i1][i] = t[i][1];
      }
    }
    printf("Calcul gains\n");
    // tranche des gains
    long G[nb_strat]; // G[i1] est le gain de l'ens. de la pop de la strat i1
    for (i1 = 0; i1 < nb_strat; i1++) G[i1]=0;

    int** r; r = malloc(nb_strat*sizeof(int*)); // matrice des gains
    for (i1 = 0; i1 < nb_strat; i1++) r[i1] = malloc(nb_strat*sizeof(int));

    for (i1 = 0; i1 < nb_strat; i1++){
      for (i2 = 0; i2 < nb_strat; i2++){
        int p[2]; // buffer
        p[0]=g[i1][i2][i];
        p[1]=g[i2][i1][i];
        int t2[2]; // buffer
        gain(p,t2);
        r[i1][i2]=t2[0];
        r[i2][i1]=t2[1];
      }
    }
    printf("Gain totaux\n");
    // gains totaux
    for (i1 = 0; i1 < nb_strat; i1++) {
      for (i2 = 0; i2 < nb_strat; i2++) {
        (i1 == i2) ? (G[i1] += (c[i1][i]-1)*r[i1][i1]) : (G[i1] += c[i2][i]*r[i1][i2]) ;
      }
    }
    for (i1 = 0; i1 < nb_strat; i1++) G[i1] *= c[i1][i];

    double s1 = 0; //somme de tous les gains
    for (i1 = 0; i1 < nb_strat; i1++) s1 += G[i1];

    printf("Mise à jour populations\n");
    int s2 = 0; // différence à la population totale
    //je vais remplir maintenant la i+1-ème génération avec des parties entières
    //mais pour ne rien perde pour la dernière
    //stratégie on calcule "total - les autres effectifs"
    for (i1 = 0; i1 < (nb_strat-1); i1++) {
      double a = Poptot * G[i1];
      a = a / s1;
      a = floor ((a)+0.5); // valeur entière la plus proche
      c[i1][i+1] = a;
      s2 += c[i1][i+1];
    }
    c[nb_strat-1][i+1] = Poptot - s2;

    printf("Echanges réseaux\n");
    reseau(i,c,strat_dispo,nb_strat,socket);
    Poptot = 0;
    for (i1 = 0; i1 < nb_strat; i1++) Poptot += c[i1][i+1];

    free(r);
  }
  return c;
}

int main(int argc, char** argv) {
  if (argc != 1+2+N+4) {printf("usage: <nb_generations> <initial_pop_per_strat> <tab_authorized_strat[%d]> <tab_coef[4]>\n",N); exit(-1);}
  init_dico();

  printf("Mise en place\n");
  int* strat_dispo; strat_dispo = malloc(N*sizeof(int));
  int nb_strat = 0; // nombre de stratégies autorisées
  int i,j;
  for (i = j = 0; i < 11; i++) {
    nb_strat += atoi(argv[i+3]);
    (atoi(argv[i+3]) != 0) ? (strat_dispo[j] = i, j++) : (i = i) ;
  }
  init_coef(atoi(argv[14]),atoi(argv[15]),atoi(argv[16]),atoi(argv[17]));

  printf("Connexion\n");
  int socket = connect_to_server();
  int b = 1;
  fd_set readfds;
  char* msg;
  // Démarrage
  while (b) {
    FD_ZERO(&readfds);
    FD_SET(socket,&readfds);	// .. from server
    if (FD_ISSET(socket,&readfds)) {
      receive_packet(socket,(void**)&msg);
      //printf("Message reçu : %s\n",msg);
      b = 0;
    }
  }
  sleep(1); // attend un peu le serveur

  printf("Simulation\n");
  ville(atoi(argv[1]),atoi(argv[2]),strat_dispo,nb_strat,socket);
  return(0);
}
