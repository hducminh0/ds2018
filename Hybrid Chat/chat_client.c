#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <sys/wait.h>

//shared data between process
static char* mesaid;

int main(int argc, char* argv[]) {
//----------------------------------INIT Input Share Memory----------------------------------

    //shared data between process
    mesaid = (char*) mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // create thread to read from keyboard
    int pid_scan = fork();
    if (pid_scan == 0) {
        // I'm the son, I'll serve read from keyboard
        while (1) {
            // keep read mess and save to shared memory
            fgets(mesaid, 95, stdin);
        }
    }

    printf("Shared Memory Init done.\n");

    char peersaid[100];
    
//----------------------------------INIT CLIENT Connection to SERVER----------------------------------
    
    // char svmess[100];
    int serv;
    struct sockaddr_in sv_address;

    socklen_t ad_length = sizeof(sv_address);
    struct hostent *hostend_ip;

    // create socket
    serv = socket(AF_INET, SOCK_STREAM, 0);

    // init address
    hostend_ip = gethostbyname(argv[1]);
    memset(&sv_address, 0, sizeof(sv_address));
    sv_address.sin_family = AF_INET;
    sv_address.sin_addr = *(struct in_addr *)hostend_ip->h_addr_list[0];
    sv_address.sin_port = htons(12345);

    // connect to server
    int connect_status = connect(serv, (struct sockaddr *)&sv_address, ad_length);
    if (connect_status == 0) {
        printf("Server connected.\n");
    }
    else {
        printf("Can not connect to Server");
        close(serv);
        return 0;
    }


//----------------------------------INIT CLIENT wait for incomming peer----------------------------------    

    int pid_passive = fork();

    if (pid_passive == 0) {
        // I'm the son, I'll serve receive connection
        int mysocket,peer,pid_write,pid_read;
        struct sockaddr_in incomming_addr;  
        char peer_ip[100];
        
        socklen_t inaddr_length = sizeof(incomming_addr);
        
        // create the socket
        mysocket = socket(AF_INET, SOCK_STREAM, 0);

        // bind the socket to port 12355 (10 port from 12345)
        memset(&incomming_addr, 0, sizeof(incomming_addr));
        incomming_addr.sin_family = AF_INET;    
        incomming_addr.sin_addr.s_addr = INADDR_ANY;
        incomming_addr.sin_port = htons(12355);
        bind(mysocket, (struct sockaddr *)&incomming_addr, inaddr_length);

        // then listen
        listen(mysocket, 0);
        printf("Client as server initilize successfully.\n");
        printf("Ready to receive incomming connection.\n");

        while (1) {
            peer = accept(mysocket, (struct sockaddr *)&incomming_addr, &inaddr_length);
            printf("peer %d connected\n",peer);

            inet_ntop(AF_INET, &(incomming_addr.sin_addr), peer_ip, sizeof(peer_ip));
            printf("Me: got connection from %s\n", peer_ip);

            // create thread to send from me
            pid_write = fork();
            if (pid_write == 0) {
                // I'm the son, I'll serverve send to this peer
                char meold[100];
                while (1) {
                    // keep read mess and send to peer
                    if ((strcmp(mesaid,meold) != 0) && (mesaid[0] != '/'))
                    {
                        int numwrite = write(peer, mesaid, 100);
                        strcpy(meold,mesaid);
                    }
                }
            } 

            // If the father, create thread to receive from peer
            pid_read = fork();
            if (pid_read == 0) {   
                // I'm the son, I'll serve receive from this peer
                // but I have the sending process PID from father
                while (1) {
                    // I wait and read message from peer
                    int numread = read(peer, peersaid, sizeof(peersaid));
                    if (numread != 0) {
                        // read success, print peer mess
                        printf("client %d <%s> says: %s\n",peer,peer_ip,peersaid);    
                    } else {
                        // read fail, disconnected, kill the writing thread.
                        printf("client %d <%s> disconnected\n", peer, peer_ip);
                        kill(pid_write,SIGKILL);
                        // close socket and end reading thread.
                        close(peer);
                        return 0;
                    }
                }
            }
                
            // I'm the father, continue the loop to accept more clients
        }
    } 


//----------------------------------INIT CLIENT setup for outgoing peer----------------------------------    

    int pid_active = fork();

    if (pid_active == 0)
    {
        printf("Client waiting for outgoing connection.\n");    
        char ipold[100];
        int outsock, pid_outwrite, pid_outread;
        struct sockaddr_in outgoing_addr;
        socklen_t outad_length = sizeof(outgoing_addr);

        // create socket
        outsock = socket(AF_INET, SOCK_STREAM, 0);

        while (1) {
            // I'm the son, I'll check if i need to make connection
            if ((strcmp(mesaid+1,ipold) != 0) && (mesaid[0] == '/')) {
                strcpy(ipold,mesaid+1);
                printf("Prepare to connect to %s\n", ipold);

                // init address
                memset(&outgoing_addr,0,sizeof(outgoing_addr));
                outgoing_addr.sin_family = AF_INET;
                // outgoing_addr.sin_addr = *(struct in_addr *)peerend_ip->h_addr_list[0];
                inet_pton(AF_INET,ipold, &outgoing_addr.sin_addr);
                outgoing_addr.sin_port = htons(12355);
                // init address

                // connect to server
                int connect_status = connect(outsock, (struct sockaddr *)&outgoing_addr, outad_length);
                if (connect_status == 0) {
                    printf("Connected to peer %d <%s>\n",outsock,ipold);
                } else {
                    printf("Can't connect to %s\n",ipold);    
                }

                // create thread to send from me
                pid_outwrite = fork();
                if (pid_outwrite == 0) {
                    // I'm the son, I'll serve send to this peer
                    char meoutold[100];
                    while (1) {
                        // keep read mess and send to peer
                        if ((strcmp(mesaid,meoutold) != 0) && (mesaid[0] != '/')) {
                            int numwrite = write(outsock, mesaid, 100);
                            strcpy(meoutold,mesaid);
                        }
                    }
                }

                // If the father, create thread to receive from peer
                pid_outread = fork();
                if (pid_outread == 0) {   
                    // I'm the son, I'll serve receive from this peer
                    // but I have the sending process PID from father
                    while (1) {
                        // I wait and read message from peer
                        int numread = read(outsock, peersaid, sizeof(peersaid));
                        if (numread != 0) {
                            // read success, print peer mess
                            printf("client %d <%s> says: %s\n",outsock,ipold,peersaid);    
                        } else {
                            // read fail, disconnected, kill the writing thread.
                            printf("client %d <%s> disconnected\n", outsock, ipold);
                            kill(pid_outwrite,SIGKILL);
                            // close socket and end reading thread.
                            close(outsock);
                            return 0;
                        }
                    }
                }
                // I'm the father, continue the loop to connect more clients
            }
        }
    }

    while (1) {
        // if exit then kill all chat_client
        if (strcmp(mesaid,"-exit") == 0)
        {
            system("pkill chat_client");
            close(serv);
            return 0;
            break;
        }
    }
    // close;    
    return 0;
}
