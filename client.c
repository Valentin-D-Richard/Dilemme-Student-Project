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
#define PORT 2552
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

void ville(int socket, int n) {
  fd_set readfds;
  char* msg;
  int* pop; pop = malloc(N*sizeof(int));
  int* immigr; immigr = malloc(N*sizeof(int));
  // immigr[i1] contient le nombre d'habitant arrivant dans la ville i de la stratégie i1

  int b = 1;
  // Démarrage
  while (b) {
    FD_ZERO(&readfds);
    FD_SET(socket,&readfds);	// .. from server
    if (FD_ISSET(socket,&readfds)) {
      receive_packet(socket,(void**)&msg);
      printf("Message reçu : %s\n",msg);
      b = 0;
    }
  }
  sleep(2); // attend un peu le serveur

  for (int i = 0; i < n; i++) {
    printf("Partie %d\n",i);
    //Envoi de la situation de la ville
    msg = give_pop(pop);
    send_string(socket,msg);
    free(msg);

    printf("Réception\n");
    // Réception de la population globale et mise à jour
    b = 1;
    while (b) {
      FD_ZERO(&readfds);
      FD_SET(socket,&readfds);	// .. from server
      if (FD_ISSET(socket,&readfds)) {
        receive_packet(socket,(void**)&msg);
        printf("Message reçu : %s\n",msg);
        b = 0;
     }
    printf("Mise à jour\n");
    get_pop(msg,immigr);
    for (int i1 = 0; i1 < N; i1++) pop[i1] += immigr[i1];
    sleep(2); // on attend un peu le serveur
    }
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {printf("usage: <nb_villes>\n"); exit(-1);}
  int n = atoi(argv[1]);

  /*for (int i1 = 0; i1 < N; i1++) pop[i1] = i1-1;
  char* msg;
  msg = give_pop(pop);
  get_pop(msg,immigr);
  printf("Test strings : %d, %d, %d\n",immigr[0],immigr[1],immigr[2]);*/

  printf("Connexion\n");
  int socket = connect_to_server();
  printf("Simulation\n");
  ville(socket,n);
  return(0);
}
