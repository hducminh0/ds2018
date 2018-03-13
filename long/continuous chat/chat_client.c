#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>
#include <netinet/in.h>

void *get_in_addr(struct sockaddr *ad)
{
    return &(((struct sockaddr_in*)ad)->sin_addr);
}

int main(int argc, char* argv[]) {
    int so,pid_write,pid_read;
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
    



    int clisock,cli_as_server,cli_as_cli;
    struct sockaddr_in cliad;
    char s1[100];

    
    socklen_t cliad_length = sizeof(cliad);
    memset(&cliad, 0, sizeof(cliad));
    

        // create the socket
        cli_as_server = socket(AF_INET, SOCK_STREAM, 0);

        // bind the socket to port 12345
        cliad.sin_family = AF_INET;    
        cliad.sin_addr.s_addr = INADDR_ANY;
        cliad.sin_port = htons(5555);
        bind(cli_as_server, (struct sockaddr *)&cliad, cliad_length);


        // then listen
        listen(cli_as_server, 5);

        clisock = accept(cli_as_server, (struct sockaddr *)&cliad, &cliad_length);
        printf("client %d connected\n",clisock);

        // pid_write = fork();

        // inet_ntop(AF_INET, get_in_addr((struct sockaddr *)&cliad), s1, sizeof(s1));
        // printf("client as server: got connection from %s\n", so);

        // if (pid_write == 0) {
        //     // I'm the son, I'll serve send to this client
        //     while (1) {
        //         // now it's my (server) turn
        //         // keep read mess and send to client
        //         fgets(s, 95, stdin);
        //         int numwrite = write(clisock, s, strlen(s) + 1);
        //     }

            
        // } else {
        //     // If the father, create thread to receive from client
        //     pid_read = fork();
        //     if (pid_read == 0)
        //     {   
        //         // I'm the son, I'll serve receive from this client
        //         // but I have the writing thread PID from father
        //         while (1) {
        //             // I wait and read message from client
        //             int numread = read(clisock, s, sizeof(s));
        //             if (numread != 0)
        //             {
        //                 // read success, print mess
        //                 printf("client %d says: %s\n",clisock,s);    
        //             } else {
        //                 // read fail, disconnected, kill the writing thread.
        //                 printf("client %d disconnected\n", clisock);
        //                 kill(pid_write,SIGKILL);
        //                 // break out to close socket and end reading thread.
        //                 break;
        //             }
                    
        //         }
        //         close(clisock);
        //         return 0;
        //     } else {
        //         // I'm the father, continue the loop to accept more clients
        //         continue;    
        //     }
        // } 
        // }  


      
        // create socket
        int cli_as_cli = socket(AF_INET, SOCK_STREAM, 0);

        // init address
        hepcli = gethostbyname(argv[1]);
        cliad.sin_addr = *(struct in_addr *)hepcli->h_addr_list[0];
        cliad.sin_port = htons(8888);

        // connect to server
        connect(cli_as_cli, (struct sockaddr *)&cliad, ad_length);
        // after connected, make client thread to send mess to server

        


        // pid_write = fork();
    
        // if (pid_write == 0)
        // {   
        // // I'm the son, I'm keep reading and sending mess
        //     while (1) {
        //         // read line and send data to server
        //         fgets(s, 95, stdin);
        //         int numwrite = write(cli_as_cli, s, strlen(s) + 1);
        //     }
        
        // }
        // // I'm the father, I handle reading
        // while(1){
        //     // keep reading
        //     int numread = read(cli_as_cli, s, sizeof(s));
        //     if (numread != 0)
        //     {
        //         // if numread > 0, read success, print mess
        //         printf("server says: %s\n", s);    
        //     } else {
        //         // read fail, disconnect
        //         printf("server disconnected\n");
        //         // kill the son writing thread
        //         kill(pid_write,SIGKILL);
        //         // break to end
        //         break;
        //     }
        
        // }
        

    // close(cli_as_cli);
    // return 0;
}