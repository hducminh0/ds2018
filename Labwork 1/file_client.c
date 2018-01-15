#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char* argv[]) {
    int so, state = 0;
    char s[100];
    struct sockaddr_in ad;
    FILE *pfile;
    char *buffer, str_filelen[1000000];
    long filelen;

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
        if (state % 2 == 0)
        {
            printf("client>");
            scanf("%s", s);
            pfile = fopen(s, "rb");         // open file 
            fseek(pfile, 0, SEEK_END);      // go to the end of the file
            filelen = ftell(pfile);         // length of the file
            rewind(pfile);          // set the pointer back to the start of the file 
            sprintf(str_filelen, "%ld", filelen);      // turn filelen into string to put into buffer
            // printf("%ld\n", filelen);
            write(serv, str_filelen, strlen(str_filelen) + 1); 
        }
        else
        {
            // create buffer to send file 
            buffer = (char *)malloc((filelen + 1) * sizeof(char));       // create a buffer to 
            // printf("buffer: %lu\n", sizeof(buffer));
            fread(buffer, filelen, 1, pfile);
            fclose (pfile);
            write(serv, buffer, filelen + 1);
        }
        state++;
    }
}

