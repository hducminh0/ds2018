#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char* argv[]) {
    int so,pid;
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
    // after connected, make client thread to receive mess from server
    pid = fork();
    if (pid == 0)
    {   
        while(1){
            read(serv, s, sizeof(s));
            printf("server says: %s\n", s);
        }
        return 0;
    }
    while (1) {
        // send some data to server
        scanf("%s", s);
        write(serv, s, strlen(s) + 1);
    }
    return 0;
}

