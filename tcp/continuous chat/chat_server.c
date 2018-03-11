#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/wait.h>

//shared data between process
static char *svsaid; 

int main() {
    //shared data between process
    svsaid = (char*) mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    char clsaid[100];

    int ss, cli, pid_write, pid_read;
    struct sockaddr_in ad;
    
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

    // create thread to read from screen
    pid_write = fork();
    if (pid_write == 0) {
        // I'm the son, I'll serve read from screen
        while (1) {
            // keep read mess and send to client
            fgets(svsaid, 95, stdin);
        }
    }

    printf("Server init complete\n");
    while (1) {
        // an incoming connection
        cli = accept(ss, (struct sockaddr *)&ad, &ad_length);
        printf("client %d connected\n",cli);

        // create thread to send from client
        pid_write = fork();
        if (pid_write == 0) {
            // I'm the son, I'll serve send to this client
            char svold[100];
            while (1) {
                // now it's my (server) turn
                // keep read mess and send to client
                if (strcmp(svsaid,&svold[0]) != 0)
                {
                    int numwrite = write(cli, svsaid, 100);
                    memcpy(svold,svsaid,100);
                }
            }
        } else {
            // If the father, create thread to receive from client
            pid_read = fork();
            if (pid_read == 0)
            {   
                // I'm the son, I'll serve receive from this client
                // but I have the writing thread PID from father
                while (1) {
                    // I wait and read message from client
                    int numread = read(cli, clsaid, sizeof(clsaid));
                    if (numread != 0)
                    {
                        // read success, print mess
                        printf("client %d says: %s\n",cli,clsaid);    
                    } else {
                        // read fail, disconnected, kill the writing thread.
                        printf("client %d disconnected\n", cli);
                        kill(pid_write,SIGKILL);
                        // break out to close socket and end reading thread.
                        break;
                    }
                }
                close(cli);
                return 0;
            } else {
                // I'm the father, continue the loop to accept more clients
                continue;    
            }
        }   
    }
    // disconnect
    close(cli);
    return 0;
}
