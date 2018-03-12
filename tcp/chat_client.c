#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char* argv[]) {
    int so;
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

    while (1) {
        // after connected, it's client turn to chat
        read(serv, s, sizeof(s));       // read response message from the server
        // Verify the response to know what to do 
        if (strcmp(s, "Please enter username") == 0){
            printf("Server connected");
            printf("Please enter username");
            scanf("%s", s);
            pfile = fopen(s, "rb");         
            write(serv, s, strlen(s) + 1);  // send username to server
        }
        else if (strcmp(s, "Please enter password") == 0){
            printf("Please enter password");
            scanf("%s", s);
            pfile = fopen(s, "rb");         
            write(serv, s, strlen(s) + 1);
        }  // send password to server
        else if (strcmp(s, "Wrong password") == 0){
            printf("Wrong password. Please enter password again");
            scanf("%s", s);
            pfile = fopen(s, "rb");         
            write(serv, s, strlen(s) + 1);
        }else if (strcmp(s, "No username") == 0){
            printf("Username is not registered. Please enter user name again");
            scanf("%s", s);
            pfile = fopen(s, "rb");         
            write(serv, s, strlen(s) + 1);
        }else if (strcmp(s, "Login success") == 0){
            printf("Login success");
            printf("Connecting to peer...");
            // Connect to peer base on list given
        }
    }
        // send some data to server
        //printf("client>");
        scanf("%s", s);
        write(serv, s, strlen(s) + 1);

        // then it's server turn
        read(serv, s, sizeof(s));

        printf("server says: %s\n", s);
    }
}

