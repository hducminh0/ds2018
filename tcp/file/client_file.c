#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char* argv[]) {
    int so;
    struct sockaddr_in ad;
    char stt[100];
    char filename[200];
    strcpy(filename,argv[2]);
    char namesend[200];
    strcpy(namesend,argv[3]);
    FILE *pfile;
    long filelen,numread,numwrite;
    char filelenstr[200];
    char *buffer;

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
    printf("connect server success\n");


    //send file name
    printf("send file name to server: %s\n",namesend);
    numwrite = write(serv, namesend, strlen(namesend) + 1);
    if (numwrite <= 0)
    {
        printf("Filename not sended: %ld\n", numwrite);

        fclose(pfile);
        close(serv);
        return 0;
    }

    read(serv, stt, sizeof(stt));

    //open file
    pfile = fopen(filename, "rb");
    //go to end
    fseek(pfile, 0, SEEK_END);
    //check the number of bytes = position of pointer
    filelen = ftell(pfile);
    //go back to start
    rewind(pfile);
    //long to string
    sprintf(filelenstr,"%ld",filelen);

    //send file size
    printf("send file size to server: %s\n",filelenstr);
    numwrite = write(serv, filelenstr, strlen(filelenstr) + 1);
    if (numwrite <= 0)
    {
        printf("File size not sended: %ld\n",numwrite);
        fclose(pfile);
        close(serv);
        return 0;
    }

    read(serv, stt, sizeof(stt));

    //create free memory for file
    buffer = (char *)malloc((filelen + 1) * sizeof(char));
    //read file into memory buffer
    fread(buffer, filelen, 1, pfile);

    //send file to server
    printf("send file to server\n");
    numwrite = write(serv, buffer, filelen);
    if (numwrite <= 0)
    {
        printf("File size not sended: %ld\n",numwrite);
        free(buffer);
        fclose(pfile);
        close(serv);
        return 0;
    }
    printf("send complete: %ld \n",numwrite);

    read(serv, stt, sizeof(stt));

    free(buffer);
    fclose(pfile);
    close(serv);
    return 0;
}