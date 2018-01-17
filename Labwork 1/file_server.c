#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main() {
    int ss, cli, pid, written;
    int state = 0;       
    struct sockaddr_in ad;
    char s[200];
    socklen_t ad_length = sizeof(ad);
    FILE *pfile;
    char *buffer;
    long filelen;

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

    while (1) {
        // an incoming connection
        cli = accept(ss, (struct sockaddr *)&ad, &ad_length);

        pid = fork();
        if (pid == 0) {
            // I'm the son, I'll serve this client
            printf("client connected\n");
            while(1)
            {   
                // check what message has been sent to the client to know what is expected to be read
                if (strcmp(s, "Ready to receive file") == 0)
                {
                    // expect to read file name from client 
                    printf("Ready to receive file\n");
                    // numread = read(cli, s, sizeof(s));
                    read(cli, s, sizeof(s));
                    // printf("numread %d, s = %s\n",numread,s );
                    pfile = fopen(s, "wb");         // create a file with the received name 
                    strcpy(s, "Received file name");        // send confirming message  
                    write(cli, s, strlen(s) + 1);
                }
                else if (strcmp(s, "Received file name") == 0)
                {
                    // expect to read file length 
                    printf("Received file name\n");
                    read(cli, s, sizeof(s));
                    filelen = atol(s);
                    printf("%lu\n", filelen);
                    strcpy(s, "Received file length");
                    write(cli, s, strlen(s) + 1);
                }
                else if (strcmp(s, "Received file length") == 0)
                {
                    // expect to receive the file content
                    printf("Received file length\n");
                    // create buffer to receive file
                    buffer = (char *)malloc((filelen + 1) * sizeof(char));
                    long templen = filelen;
                    // read everything in cli to buffer. This loop is to help reading the whole file
                    while (templen > 0)
                    {
                        int written = read(cli, buffer, templen);
                        buffer += written;
                        templen -= written;

                    }
                    // printf("%lu\n", templen);
                    buffer -= filelen;          // return the buffer pointer to the start of the pointer
                    // rewind(buffer);
                    while (filelen > 0)
                    {
                        int written = fwrite(buffer, 1, filelen, pfile);
                        buffer += written;
                        filelen -= written;
                    }
                    fclose(pfile);
                    strcpy(s, "Done");
                    write(cli, s, strlen(s) + 1);
                }
                else
                {
                    if (strcmp(s, "Done") == 0)
                    {
                        printf("Press enter to continue receive files\n");
                        getchar();
                        // memset(s, '\0', strlen(s));
                    }
                    strcpy(s, "Ready to receive file");
                    write(cli, s, strlen(s) + 1);
                }
            }
        }
        else {
            // I'm the father, continue the loop to accept more clients
            continue;
        }
    }
    // disconnect
    close(cli);

}
