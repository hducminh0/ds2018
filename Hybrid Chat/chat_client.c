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

void *get_in_addr(struct sockaddr *sv_address)
{
    return &(((struct sockaddr_in*)sv_address)->sin_addr);
}


int svsaid;


int main(int argc, char* argv[]) {
    svsaid = (char*) mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    char clsaid[100];

//----------------------------------INIT CLIENT Connection to SERVER----------------------------------
    
    char mess[100];
    int serv;
    struct sockaddr_in sv_address;

    socklen_t ad_length = sizeof(sv_address);
    struct hostent *hostend_ip, *peerend_ip;

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
    }

    // after connected to server, client doing initial stuff with server
    while (1) {
        // after connected, read instruction from server 
        read(serv, mess, sizeof(mess));       // read response message from the server
        // Verify the response to know what to do 
        if (strcmp(mess, "Please enter username") == 0){
            printf("Please enter username: ");
            scanf("%s", mess);
            write(serv, mess, strlen(mess) + 1);  // send username to server
        }
        else if (strcmp(mess, "Please enter password") == 0){
            printf("Please enter password: ");
            scanf("%s", mess);
            // should encrypt password but nah, just do it later
            write(serv, mess, strlen(mess) + 1);
        }  // send password to server
        else if (strcmp(mess, "Wrong password") == 0){
            printf("Wrong password. Please enter password again. \n");
            scanf("%s", mess);
            // encrypt pass can wait to tomorrow
            write(serv, mess, strlen(mess) + 1);
        }else if (strcmp(mess, "No username") == 0){
            printf("Username is not registered. Please enter user name again. \n");
            scanf("%s", mess);       
            write(serv, mess, strlen(mess) + 1);
        }else if (strcmp(mess, "Login success") == 0){
            printf("Login success !\n");
            //printf("Connecting to peer...");
            // Connect to peer base on list given
            break;
        }else {
            // received some wrong mess from server, give 0 f*ck and continue
            continue;
        }
    }


//----------------------------------INIT CLIENT wait for incomming peer----------------------------------    

    int mysocket,peer,pid_write,pid_read, pid_in, pid_in_write, pid_in_read;
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
    printf("Client as server initilize successfully\n");
    

    while (1) {
        peer = accept(mysocket, (struct sockaddr *)&incomming_addr, &inaddr_length);
        printf("peer %d connected\n",peer);

        inet_ntop(AF_INET, &(incomming_addr.sin_addr), peer_ip, sizeof(peer_ip));
        printf("Me: got connection from %s\n", peer_ip);

        // create thread to send from client
        pid_in_write = fork();
        

        if (pid_in_write == 0) {
            // I'm the son, I'll serve send to this client
            while (1) {
                // now it's my (server) turn
                // keep read mess and send to client
                fgets(svsaid, 95, stdin);
                int numwrite = write(peer, svsaid, 100);
            }

            
        } else {
            // If the father, create thread to receive from client
            pid_in_read = fork();
            if (pid_read == 0)
            {   
                // I'm the son, I'll serve receive from this client
                // but I have the writing thread PID from father
                while (1) {
                    // I wait and read message from client
                    int numread = read(peer, clsaid, sizeof(clsaid));
                    if (numread != 0)
                    {
                        // read success, print mess
                        printf("client %d says: %s\n",peer,clsaid);    
                    } else {
                        // read fail, disconnected, kill the writing thread.
                        printf("client %d disconnected\n", peer);
                        kill(pid_write,SIGKILL);
                        // break out to close socket and end reading thread.
                        break;
                    }
                    
                }
                close(peer);
                return 0;
            } else {
                // I'm the father, continue the loop to accept more clients
                continue;    
            }
        }   
    
    }        

    


//------------------------------------Outgoing connection-------------------------------------------------------------------

    int clisock,cli_as_server,cli_as_cli, pid_out;
    struct sockaddr_in cliad;
    socklen_t cliad_length = sizeof(cliad);
    char arrIP[100];
    // create socket
    cli_as_cli = socket(AF_INET, SOCK_STREAM, 0);

    // init address
    peerend_ip = gethostbyname(argv[1]);
    memset(&cliad,0,sizeof(cliad));
    cliad.sin_family = AF_INET;
    cliad.sin_addr = *(struct in_addr *)peerend_ip->h_addr_list[0];
    cliad.sin_port = htons(12355);

    // connect to server
    
    char temp[15];

    for (int i = 0; i < strlen(arrIP) ; i += 15)
    {

        // cliad.sin_addr.s_addr = fgets(arrIP + i, 15, stdin);

        for (int j = 0; j < 14; j++)
        {
            temp[j] = arrIP[j + i];
        }

        inet_aton(temp, &cliad.sin_addr);

        pid_out = fork();

        if (pid_out == 0){

            connect(cli_as_cli, (struct sockaddr *)&cliad, cliad_length);
            printf("Connected to peer %d\n",cli_as_cli);
            // after connected, make client thread to send mess to server

            pid_write = fork();

            if (pid_write == 0)
            {   
                // I'm the son, I'm keep reading and sending mess
                while (1) {
                    // read line and send data to server
                    fgets(svsaid, 95, stdin);
                    int numwrite = write(cli_as_cli, svsaid, 100);
                }
                
            }
            // I'm the father, I handle reading
            while(1){
                // keep reading
                int numread = read(cli_as_cli, clsaid, sizeof(clsaid));
                if (numread != 0)
                {
                    // if numread > 0, read success, print mess
                    printf("peer %d says: %s\n", cli_as_cli, clsaid);    
                } else {
                    // read fail, disconnect
                    printf("server disconnected\n");
                    // kill the son writing thread
                    kill(pid_write,SIGKILL);
                    // break to end
                    break;
                }
            }

        }

        // after connected, make client thread to send mess to server
    }

    // close;
    
    return 0;
}
