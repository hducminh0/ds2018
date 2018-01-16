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
    char s[1000000];
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
                if (strcmp(s, "Ready to receive file") == 0)
                {
                    printf("Ready to receive file\n");
                    read(cli, s, sizeof(s));
                    pfile = fopen(s, "wb");
                    strcpy(s, "Received file name");
                    write(cli, s, strlen(s) + 1);
                }
                else if (strcmp(s, "Received file name") == 0)
                {
                    printf("Received file name\n");
                    read(cli, s, sizeof(s));
                    filelen = atol(s);
                    printf("%lu\n", filelen);
                    strcpy(s, "Received file length");
                    write(cli, s, strlen(s) + 1);
                }
                else if (strcmp(s, "Received file length") == 0)
                {
                    printf("Received file length\n");
                    // create buffer to receive file
                    buffer = (char *)malloc((filelen + 1) * sizeof(char));
                    // printf("buffer: %lu\n", sizeof(buffer));
                    // read(cli, buffer, filelen);
                    long templen = filelen;
                    char* ptemp = buffer;
                    while (templen > 0)
                    {
                        int written = read(cli, ptemp, templen);
                        ptemp = ptemp + written;
                        templen -= written;
                    }
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
            
            //     if (strcmp(s, "Ready to receive file") == 0)
            //     {
            //         read(cli, s, sizeof(s));
            //         pfile = fopen(s, "wb");
            //         s = "Received file name";
            //         write(cli, s, strlen(s) + 1);
            //     }
            //     else
            //     {   
            //         s = "Ready to receive file"
            //         write(cli, s, strlen(s) + 1);
            //     }
            // }



            // while (1) {
            //     // it's client turn to chat, I wait and read message from client
            //    if (state % 2 == 0)
            //     {
            //         read(cli, s, sizeof(s));
            //         // printf("client says: %s\n",s);
            //         filelen = atol(s);
            //         printf("%lu\n", filelen);
            //     }
            //     else
            //     {
            //         // create buffer to receive file
            //         buffer = (char *)malloc((filelen + 1) * sizeof(char));
            //         // printf("buffer: %lu\n", sizeof(buffer));
            //         read(cli, buffer, filelen);
            //         pfile = fopen("received.png", "wb");
            //         while (filelen > 0)
            //         {
            //             // printf("%ld\n", filelen);
            //             int written = fwrite(buffer, 1, filelen, pfile);
            //             if (written < 1)
            //             {
            //                 printf("Can't write to file");
            //                 return -1;
            //             }

            //             buffer += written;
            //             filelen -= written;
            //         }
            //         fclose(pfile);
            //         printf("Done");
            //         getchar();
            //     }
            //     state++;
            // }
            // return 0;
        }
        else {
            // I'm the father, continue the loop to accept more clients
            continue;
        }
    }
    // disconnect
    close(cli);

}
