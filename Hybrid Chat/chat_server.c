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
#include <my_global.h>
#include <mysql.h>

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);        
}

int finduser(char username[100]){
    char query[100]; 
    sprintf(query,"SELECT Name FROM Users WHERE Name ='%s'",username);

    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
    }  
    // connect to database
    if (mysql_real_connect(con, "localhost", "hungson", "Son_123*", 
          "userdata", 0, NULL, 0) == NULL) 
    {
      finish_with_error(con);
    }    
    // query
    if (mysql_query(con, query)) 
    {
      finish_with_error(con);
    }
    // store the result after query
    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) 
    {
      finish_with_error(con);
    }
    // get the numb of columns in the table
    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

    //fetch the rows
    if ((row = mysql_fetch_row(result))) 
    { 
        return 1;
    } else {
        return 0;
    }

    mysql_free_result(result);
    mysql_close(con);
}

void save_user_ip(char *username, char *ip) {
    char query[100]; 
    sprintf(query,"UPDATE Users SET IP ='%s', Status = '1' WHERE Name ='%s'",ip,username);

    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
    }  
    // connect to database
    if (mysql_real_connect(con, "localhost", "hungson", "Son_123*", 
          "userdata", 0, NULL, 0) == NULL) 
    {
      finish_with_error(con);
    }    
    // query
    if (mysql_query(con, query)) 
    {
      finish_with_error(con);
    }
    
    mysql_close(con);
}

int getroom(char *usertofind) {
    char query[100]; 
    sprintf(query,"SELECT Room FROM Users WHERE Name ='%s' AND Status = '1'",usertofind);
    char iptosend[16];
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
    }  
    // connect to database
    if (mysql_real_connect(con, "localhost", "hungson", "Son_123*", 
          "userdata", 0, NULL, 0) == NULL) 
    {
      finish_with_error(con);
    }    
    // query
    if (mysql_query(con, query)) 
    {
      finish_with_error(con);
    }
    // store the result after query
    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) 
    {
      finish_with_error(con);
    }
    // get the numb of columns in the table
    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

    //fetch the rows
    if ((row = mysql_fetch_row(result))) 
    { 
        return atoi(row[0]);
    } else {
        return -1;
    }

    mysql_free_result(result);
    mysql_close(con);
}

void setroom(char *usertofind) {
    char query[100]; 
    sprintf(query,"UPDATE Users SET Room = Id WHERE Name ='%s'",username);

    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
    }  
    // connect to database
    if (mysql_real_connect(con, "localhost", "hungson", "Son_123*", 
          "userdata", 0, NULL, 0) == NULL) 
    {
      finish_with_error(con);
    }    
    // query
    if (mysql_query(con, query)) 
    {
      finish_with_error(con);
    }
    
    mysql_close(con);
}

void send_ip_in_room(int room,int cli) {
    char query[100]; 
    sprintf(query,"SELECT IP FROM Users WHERE Room ='%d'",room);

    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
    }  
    // connect to database
    if (mysql_real_connect(con, "localhost", "hungson", "Son_123*", 
          "userdata", 0, NULL, 0) == NULL) 
    {
      finish_with_error(con);
    }    
    // query
    if (mysql_query(con, query)) 
    {
      finish_with_error(con);
    }
    // store the result after query
    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) 
    {
      finish_with_error(con);
    }
    // get the numb of columns in the table
    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

    char mess[100];
    //fetch the rows
    while ((row = mysql_fetch_row(result))) 
    { 
        strcpy(mess, row[0]);
        write(cli, mess, strlen(mess) + 1);

        while (strcmp(mess,'OK')){
            read(cli, mess, sizeof(mess));       // read response message from the server    
        }
        
    }

    mysql_free_result(result);
    mysql_close(con);
}

int main() {
//----------------------------------INIT SERVER----------------------------------
    char mess[100];

    int mysocket, cli, pid_client, numread;
    struct sockaddr_in socket_address;
    char cli_ip[100];
    socklen_t skaddr_length = sizeof(socket_address);

    // create the socket
    mysocket = socket(AF_INET, SOCK_STREAM, 0);

    // bind the socket to port 12345
    memset(&socket_address, 0, sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = INADDR_ANY;
    socket_address.sin_port = htons(12345);
    bind(mysocket, (struct sockaddr *)&socket_address, skaddr_length);

    // then listen
    listen(mysocket, 0);
    printf("Server init complete\n");

    

//----------------------------------STANDBY FOR CONNECTION----------------------------------
    while (1) {
        // an incoming connection
        cli = accept(mysocket, (struct sockaddr *)&socket_address, &skaddr_length);
        printf("client %d connected\n",cli);

        inet_ntop(AF_INET, &(socket_address.sin_addr), cli_ip, sizeof(cli_ip));
        printf("server: got connection from %s\n", cli_ip);

        // create process to serve this client
        pid_client = fork();

        if (pid_client == 0)
        {
            char username[100];
            strcpy(mess, "Please enter username");
            write(cli, mess, strlen(mess) + 1);
            // I'm the son, I'll serve this client
            while (1) {
                // do login checking, searching, trading info
                numread = read(cli, mess, sizeof(mess));
                if (numread == 0){
                    // read fail, client disconnected.
                    printf("client %d disconnected\n", cli);
                    close(cli);
                    return 0;
                }
                if (finduser(mess) == 1){ //Compare to user name in database
                    strcpy(username, mess);
                    save_user_ip(username, cli_ip);
                    strcpy(mess, "Login success"); 
                    write(cli, mess, strlen(mess) + 1);
                    break;
                }
                else{
                    strcpy(mess, "No username"); 
                    write(cli, mess, strlen(mess) + 1);
                }
            }

            //find user to check ip
            // I wait and read message from client
            strcpy(mess, "Input username");
            write(cli, mess, strlen(mess) + 1);
            int room;
            while (1) {
                numread = read(cli, mess, sizeof(mess));
                if (numread != 0)
                {
                    char usertofind[100];
                    strcpy(usertofind, mess);
                    //get room , if no room then make room
                    room = getroom(usertofind);
                    if (room == -1) {
                        strcpy(mess, "User is not online or not exist"); 
                        write(cli, mess, strlen(mess) + 1);
                    } else {
                        sprintf(mess,"%d",room);
                        write(cli, mess, strlen(mess) + 1);
                    }
                } else {
                    // read fail, client disconnected.
                    printf("client %d disconnected\n", cli);
                    printf("username %s has disconnected.\n", username);
                    close(cli);
                    return 0;
                }
            }

            // after doing stuff, just put this checking client status here
            while (1) {
                // I wait and read message from client
                numread = read(cli, mess, sizeof(mess));
                if (numread != 0)
                {
                    // read success, print mess
                    printf("client %d says: %s\n",cli,mess);    
                } else {
                    // read fail, client disconnected.
                    printf("client %d disconnected\n", cli);
                    printf("username %s has disconnected.\n", username);
                    close(cli);
                    return 0;
                }
            }


        } else {
            // I'm the father, I continue to accept more client
            continue;
        }

        /*
        // // create process to send from server to client
        // pid_write = fork();


        // if (pid_write == 0) {
        //     // I'm the son, I'll serve send to this client
        //     while (1) {
        //         // now it'mess my (server) turn
        //         // keep read mess and send to client
        //         fgets(mess, 95, stdin);
        //         int numwrite = write(cli, mess, strlen(mess) + 1);
        //     }
        // } else {
        //     // If the father, create process to receive from client
        //     pid_read = fork();
        //     if (pid_read == 0)
        //     {   
        //         // I'm the son, I'll serve receive from this client
        //         // but I have the writing process PID from father
        //         while (1) {
        //             // I wait and read message from client
        //             int numread = read(cli, mess, sizeof(mess));
        //             if (numread != 0)
        //             {
        //                 // read success, print mess
        //                 printf("client %d says: %mess\n",cli,mess);    
        //             } else {
        //                 // read fail, disconnected, kill the writing thread.
        //                 printf("client %d disconnected\n", cli);
        //                 kill(pid_write,SIGKILL);
        //                 // break out to close socket and end reading thread.
        //                 break;
        //             }
        //         }
        //         close(cli);
        //         return 0;
        //     } else {
        //         // I'm the father, continue the loop to accept more clients
        //         continue;    
        //     }
        // }   
        */
    }
    return 0;
}




