#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main() {
    int ss, cli, pid, numread, numwrite;
    struct sockaddr_in ad;
    socklen_t ad_length = sizeof(ad);
    char stt[100] = "OK";
    char filename[200];
    FILE *pfile;
    long filelen;
    char filelenstr[200];
    char *buffer;

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
    printf("server init completed\n");

    while (1) {
        // an incoming connection
        cli = accept(ss, (struct sockaddr *)&ad, &ad_length);

        pid = fork();
        if (pid == 0) {
            // I'm the son, I'll serve this client
            printf("client connected\n");
            
            // receive file name
            numread = read(cli, filename, sizeof(filename));
            if (numread <= 0)
            {
                printf("Read file name failed: %d \n", numread);
                close(cli);
                return 0;
            }
            printf("File name received: %s\n",filename);
            write(cli, stt, strlen(stt) + 1);

            // receive file size
            numread = read(cli, filelenstr, sizeof(filelenstr));
            if (numread <= 0)
            {
                printf("Read file size failed: %d \n", numread);
                close(cli);
                return 0;
            }
            filelen = atol(filelenstr);
            printf("File size received: %ld\n",filelen);
            write(cli, stt, strlen(stt) + 1);

            // receive file 
            buffer = (char *)malloc((filelen + 1) * sizeof(char));
            numread = read(cli, buffer, filelen + 1);
            if (numread <= 0)
            {
                printf("Read file failed: %d \n", numread);
                free(buffer);
                close(cli);
                return 0;
            }
            printf("File received: %d \n", numread);
            printf("size buffer: %lu\n", sizeof(buffer));

            pfile = fopen(filename, "wb");
            int written = fwrite(buffer, sizeof(char), filelen, pfile);
            if (written < 1)
            {
                printf("Can't save to file");
                free(buffer);
                fclose(pfile);
                close(cli);
                return 0;
            }

            printf("File saved: %d \n",written);

            write(cli, stt, strlen(stt) + 1);

            free(buffer);
            fclose(pfile);
            close(cli);
            return 0;
        }
        else {
            // I'm the father, continue the loop to accept more clients
            continue;
        }
    }
    // disconnect
    close(cli);
}