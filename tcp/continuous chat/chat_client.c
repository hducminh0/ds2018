#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>

void *get_in_addr(struct sockaddr *ad)
{
    return &(((struct sockaddr_in*)ad)->sin_addr);
}

int main(int argc, char* argv[]) {
    int so,pid_write;
    char s[100];
    struct sockaddr_in ad;

    socklen_t ad_length = sizeof(ad);
    struct hostent *hep, *hepcli;

    // create socket
    int serv = socket(AF_INET, SOCK_STREAM, 0);

    // init address
    hep = gethostbyname(argv[1]);
    memset(&ad, 0, sizeof(ad));
    ad.sin_family = AF_INET;
    ad.sin_addr = *(struct in_addr *)hep->h_addr_list[0];
    ad.sin_port = htons(12345);

    // connect to server
    connect(serv, (struct sockaddr *)&ad, ad_length);
    // after connected, make client thread to send mess to server
    


    int clisock,cas,c2c;
    struct sockaddr_in cliad;
    char s1[100], s2[100];
    int opt;

    socklen_t cliad_length = sizeof(cliad);

    printf("Enter your option:\n");
    printf("1.Set this client as a server\n");
    printf("2.Set this client connecting to a client-as-server.\n");

    scanf("%d\n",opt);
    
    pid_write = fork();
    
    

    if (opt == 1){

    	// create the socket
    	cas = socket(AF_INET, SOCK_STREAM, 0);

    	// bind the socket to port 12345
    	memset(&cliad, 0, sizeof(cliad));
    	cliad.sin_family = AF_INET;
    	cliad.sin_addr.s_addr = INADDR_ANY;
    	cliad.sin_port = htons(5555);
    	bind(cas, (struct sockaddr *)&cliad, cliad_length);


    	// then listen
    	listen(cas, 5);

    	clisock = accept(cas, (struct sockaddr *)&cliad, &cliad_length);
    	printf("client %d connected\n",clisock);

    	// inet_ntop(AF_INET, get_in_addr((struct sockaddr *)&cliad), s1, sizeof(s1));
    	// printf("client as server: got connection from %s\n", so);

		} else if (opt == 2){
    
    	// create socket
    	int c2c = socket(AF_INET, SOCK_STREAM, 0);

    	// init address
    	hepcli = gethostbyname(argv[1]);
    	memset(&ad, 0, sizeof(ad));
    	cliad.sin_family = AF_INET;
    	cliad.sin_addr = *(struct in_addr *)hepcli->h_addr_list[0];
    	cliad.sin_port = htons(8888);

    	// connect to server
    	connect(c2c, (struct sockaddr *)&cliad, ad_length);
    	// after connected, make client thread to send mess to server
    	pid_write = fork();
    }


    if (pid_write == 0)
    {   
        // I'm the son, I'm keep reading and sending mess
        while (1) {
            // read line and send data to server
            fgets(s, 95, stdin);
            int numwrite = write(serv, s, strlen(s) + 1);
        }
        
    }
    // I'm the father, I handle reading
    while(1){
        // keep reading
        int numread = read(serv, s, sizeof(s));
        if (numread != 0)
        {
            // if numread > 0, read success, print mess
            printf("server says: %s\n", s);    
        } else {
            // read fail, disconnect
            printf("server disconnected\n");
            // kill the son writing thread
            kill(pid_write,SIGKILL);
            // break to end
            break;
        }
        
    }
    close(serv);
    return 0;
}
