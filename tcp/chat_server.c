#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main() {
    int ss, cli, pid;
    struct sockaddr_in ad;
    char s[100];
    socklen_t ad_length = sizeof(ad);

    // create the socket
    ss = socket(AF_INET, SOCK_STREAM, 0);

    // bind the socket to port 12345
    memset(&ad, 0, sizeof(ad));
    ad.sin_family = AF_INET;
    ad.sin_addr.s_addr = INADDR_ANY;
    ad.sin_port = htons(12345);
    bind(ss, (struct sockaddr *)&ad, ad_length);

    // then listen
    listen(ss, 0);
    printf("Server init complete\n");
    while (1) {
        // an incoming connection
        cli = accept(ss, (struct sockaddr *)&ad, &ad_length);
        printf("client %d connected\n",cli);
        // create thread to receive from client
        pid = fork();
        if (pid == 0) {
            // I'm the son, I'll serve receive from this client
            while (1) {
                // I wait and read message from client
                read(cli, s, sizeof(s));
                printf("client %d says: %s\n",cli,s);
            }
            return 0;
        } else {
            // If the father, create thread to send to client
            pid = fork();
            if (pid == 0)
            {   
                // I'm the son, I'll serve send to this client
                while (1) {
                    // now it's my (server) turn
                    // keep read mess and send to client
                    scanf("%s", s);
                    write(cli, s, strlen(s) + 1);
                }
                return 0;
            } else {
                // I'm the father, continue the loop to accept more clients
                continue;
            } 
        }
    }
    // disconnect
    close(cli);

}
