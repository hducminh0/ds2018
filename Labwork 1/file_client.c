#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char* argv[]) {
    int so;
    char s[200];
    struct sockaddr_in ad;
    FILE *pfile;
    char *buffer;
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

    while (1) 
    {
        // after connected, it's client turn to chat
        read(serv, s, sizeof(s));       // read response message from the server
        // Verify the response to know what to do 
        if (strcmp(s, "Ready to receive file") == 0)
        {
            // start to send file name to the server
            printf("Ready to receive file\n");
            printf("client> Please enter file name: "); 
            scanf("%s", s);
            pfile = fopen(s, "rb");         
            write(serv, s, strlen(s) + 1);          // send file name to the server
        }
        else if (strcmp(s, "Received file name") == 0)
        {
            // start to send file length to the server
            printf("Received file name\n");
            fseek(pfile, 0, SEEK_END);      // go to the end of the file
            filelen = ftell(pfile);         // length of the file
            rewind(pfile);          // set the pointer back to the start of the file 
            sprintf(s, "%ld", filelen);      // turn filelen into string to put into buffer
            // printf("%ld\n", filelen);
            write(serv, s, strlen(s) + 1);          // send file length
            printf("%lu\n", filelen);
        }
        else if (strcmp(s, "Received file length") == 0)
        {
            //  start to send file content to the server 
            printf("Received file length\n");
            // create buffer to send file 
            buffer = (char *)malloc((filelen + 1) * sizeof(char));       // create a buffer to 
            // printf("buffer: %lu\n", sizeof(buffer));
            fread(buffer, filelen, 1, pfile);
            fclose (pfile);
            // send file to the server 
            while(filelen > 0)
            {
                int written = write(serv, buffer, filelen);
                buffer += written;
                filelen -= written;
                // printf("%lu\n", filelen);
            }
        }
        else
        {
            // when there is not a verified message -> wait
            printf("Wait\n");
        }
    }
}

