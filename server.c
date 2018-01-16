#include <errno.h>
#include <time.h>
#include <ulimit.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <time.h>

#include "packets.h"
#include "dilemmelib.h"

// maximum number of files that a process can open
#define MAXFILES (ulimit(4))

// maximal size of a file that we'll allow
#define MAXSIZE 65535

// name of history file
#define HISTORY "history.txt"

int all_true(int* tab, int n) {
  for (int i = 0; i < n; i++) {
    if (!tab[i]) return(0);
  }
  return(1);
}

void init_0(int* tab, int n) { for (int i = 0; i < n; i++) tab[i] = 0; }

int get_server_socket (int port)
{
	struct sockaddr_in server_address;

	// create a TCP socket
	int s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s < 0) fatal_error("No socket available");

	// connect the socket to the chosen port
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(port);
	if (bind(s,(struct sockaddr *)&server_address,
				sizeof server_address))
		fatal_error("Can't bind port");

	// make the socket listen for connections
	if (listen(s,5)) fatal_error("listen failed");

	return s;
}


int wait_for_client (int socket)
{
	while (1)
	{
		// let a client connect to the socket
		int client_socket = accept(socket,NULL,NULL);

		if (client_socket < 0)
		{
			if (errno == EINTR || errno == EWOULDBLOCK)
				continue;
			else
				fatal_error("Can't accept connection");
		}

		return client_socket;
	}
}

#define MAXFILES (ulimit(4))
int NBVILLES;

int binom(int n) {
  // loi binomiale de pamètres n, 1/3
  int s = 0;
  for (int i = 0; i < n; i++) s += (rand() % 3 == 0) ? 1 : 0;
  return(s);
}

int** migration(int** g) {
  int p[NBVILLES]; // p[i] contient la population totale de la ville i
  init_0(p,NBVILLES);
  int i,i1;
  for (i = 0; i < NBVILLES; i++) {
    for (i1 = 0; i1 < N ; i1++) {
      (g[i][i1] !=-1) ? (p[i] += g[i][i1]) : (i = i) ;
    }
  }
  int** immigr;
  immigr = malloc(NBVILLES*sizeof(int));
  for (i = 0; i < NBVILLES; i++) {
    immigr[i] = malloc(N*sizeof(int));
    for (i1 = 0; i1 < N; i1++) immigr[i][i1] = 0;
  }

  int c[N]; //c[i1] contient le numero de la ville qui possède la plus grande proportion de strat i1
  for (i = 0; i < N; i++) c[i]=0;
  // Cacul de c
  for (i1 = 0; i1 < N; i1++) {
    for (i = 0; i < NBVILLES; i++) {
	(g[i][i1]/p[i] > g[c[i1]][i1]/p[c[i1]]) ? (c[i1] = i) : (i = i) ;
    }
  }

  // Calcul de immigr
  for (i = 0; i < NBVILLES; i++) {
    for (i1 = 0; i1 < N; i1++) {
      float ratio = (float)g[i][i1] / (float)p[i]; //printf("%f; ",ratio);
      if (g[i][i1] != -1 && g[c[i1]][i1] != -1 && ratio <= 0.04) {
        int migr = binom(g[i][i1]);
        immigr[c[i1]][i1] += migr;
        immigr[i][i1] -= migr;
      };
    }
  }
  return immigr;
}

int main (int argc, char **argv)
{ srand(time(NULL));

  if (argc != 4) {printf("usage: <nb_parties> <port> <nb_villes>\n"); exit(-1);}
  int i,j;
  NBVILLES = atoi(argv[3]);

  int n = atoi(argv[1]);
  int* resp; resp = malloc(NBVILLES*sizeof(int));
  int** glb_pop; glb_pop = malloc(NBVILLES*sizeof(int*));
  for (j = 0; j < NBVILLES; j++) {
    resp[j] = 0; // resp[j] = 1 ssi la ville j a répondu
    glb_pop[j] = malloc(N*sizeof(int));
  }
  int** immigr;

  char *msg;
  fd_set readfds;

  // Connexions
  printf("Connexion\n");
  int socket = get_server_socket(atoi(argv[2]));
  int* csock; csock = malloc(NBVILLES*sizeof(int));
  for (j = 0; j < NBVILLES; j++) csock[j] = wait_for_client(socket);


  // Démarrage
  for (j = 0; j < NBVILLES; j++) send_string(csock[j],"start");

  printf("Simulation\n");
  int b;
  for (i = 0; i < n; i++) {
    b = 1;
    printf("*** Génération %d ***\n",i);
    while (b) {

      // wait for input...
      FD_ZERO(&readfds);
      for (j = 0; j < NBVILLES; j++) FD_SET(csock[j],&readfds); // from client
      FD_SET(0,&readfds);		// .. from console
      //FD_SET(socket,&readfds);	// .. another connection
      if (select(MAXFILES, &readfds, NULL, NULL, NULL) <0) continue;

      // check where input arrived
      for (j = 0; j < NBVILLES; j++) {
        if (FD_ISSET(csock[j],&readfds)) {
          receive_packet(csock[j],(void**)&msg);
          printf("Message reçu par %d : %s\n",j,msg);
          resp[j] = 1;
          get_pop(msg,glb_pop[j]);
          free(msg);
        };
      }
      /*if (FD_ISSET(0,&readfds))
        {
        scanf("%ms",&msg);
        printf("server said: %s\n",msg);
        send_string(csock1,msg);
        send_string(csock2,msg);
        free(msg);
        }*/
      if (all_true(resp,NBVILLES)) {
        printf("Migrations\n");
        immigr = migration(glb_pop);

        for (j = 0; j < NBVILLES; j++) {
          //printf("Envoi à %d\n",j);
          msg = give_pop(immigr[j]);
          send_string(csock[j],msg);
          init_0(resp,NBVILLES);
          //free(msg);
        }
        b = 0;
      }
    }
  }
}
