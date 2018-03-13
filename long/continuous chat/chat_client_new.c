#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/wait.h>

#define ServPort "12345"
#define CliPort "6969"
#define PeerPort "9696"

pthread_t thread[3];    //ping in thread[0]
                        //send to peer in thread[1]
                        //receive from peer in thread[2]

bool pingCheck, sendCheck, recvCheck;

void *get_in_addr(struct sockaddr *ad){
    return &(((struct sockaddr_in*)ad)->sin_addr);
}

void pchild_handle(int p){
    while(waitpid(-1, NULL, WNOHANG) > 0);
}


bool getOnlineClient(int serv){
    int rv;

    if(send(serv,"LIST",5,0)){
        printf("Error in sending LIST to to server\n");
    }

    char buffer[256];

    memset(buffer,0,256);

    if ((rv = recv(serv,buffer,255,0)) > 0){
        printf("Online clients: %s\n",buffer);

    } else {
        printf("Exit now\n");
        close(serv);
        return false;
    }
    return true;
}




int main(int argc, char* argv[]) {
    int so,pid_write;
    char s[100];
    struct sockaddr_in ad;

    socklen_t ad_length = sizeof(ad);
    struct hostent *hep;

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

    for (int i = 0; i < sizeof((struct in_addr *)->h_addr_list); i++)
    {   
        ad.sin_addr
        printf("%c\n", h );
    }

    // after connected, make client thread to send mess to server
    pid_write = fork();
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

