
///* A simple server in the internet domain using TCP
//   The port number is passed as an argument
//   This version runs forever, forking off a separate
//   process for each connection
//   gcc server2.c -lsocket
//*/
//#include <stdio.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h>
//#include <iostream>
//#include <string.h>
//#include <unistd.h>

//void dostuff(int); /* function prototype */
//void error(const char *msg)
//{
//    perror(msg);
//    exit(1);
//}

//int main(int argc, char *argv[])
//{
//     int sockfd, newsockfd, portno, pid;
//     struct sockaddr_in serv_addr, cli_addr;
//     socklen_t clilen;

//     if (argc < 2) {
//         fprintf(stderr,"ERROR, no port provided\n");
//         exit(1);
//     }
//     sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (sockfd < 0)
//        error("ERROR opening socket");
//     bzero((char *) &serv_addr, sizeof(serv_addr));
//     portno = atoi(argv[1]);
//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_addr.s_addr = INADDR_ANY;
//     serv_addr.sin_port = htons(portno);
//     if (bind(sockfd, (struct sockaddr *) &serv_addr,
//              sizeof(serv_addr)) < 0)
//              error("ERROR on binding");
//     listen(sockfd,5);
//     clilen = sizeof(cli_addr);
//     while (1) {
//         newsockfd = accept(sockfd,
//               (struct sockaddr *) &cli_addr, &clilen);
//         if (newsockfd < 0)
//             error("ERROR on accept");
//         pid = fork();
//         if (pid < 0)
//             error("ERROR on fork");
//         if (pid == 0)  {
//             close(sockfd);
//             dostuff(newsockfd);
//             exit(0);
//         }
//         else close(newsockfd);
//     } /* end of while */
//     return 0; /* we never get here */
//}

///******** DOSTUFF() *********************
// There is a separate instance of this function
// for each connection.  It handles all communication
// once a connnection has been established.
// *****************************************/
//void dostuff (int sock)
//{
//   int n;
//   char buffer[256];

//   bzero(buffer,256);
//   n = read(sock,buffer,255);
//   if (n < 0) error("ERROR reading from socket");
//   printf("Here is the message: %s\n",buffer);
//   n = write(sock,"I got your message",18);
//   if (n < 0) error("ERROR writing to socket");
//}


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <iostream>

void error_server(const char *msg)
{
perror(msg);
exit(1);
}
void dostuff (int sock)
{
    int n;
    char buffer[256];
    std::cout<<"i am in dostuff\n";
    bzero(buffer,256);
    char sendBuff[1025];
    memset(sendBuff, '0', sizeof(sendBuff));
    strcpy(sendBuff, "Message from server");
    write(sock, sendBuff, strlen(sendBuff));
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        n = read(sock,buffer,255);
        if (n < 0) error_server("ERROR reading from socket");
        std::cout<<"do print::::\n";
        printf("Here is the message: %s\n",buffer);
        n = write(sock,"I got your message",18);
        if (n < 0) error_server("ERROR writing to socket");
    }

}

int main(void)
{
    int listenfd = 0,connfd = 0, pid;

    struct sockaddr_in serv_addr;

    char sendBuff[1025];
    int numrv;

    listenfd = socket(AF_INET, SOCK_STREAM, 0); //didn't close it!!!
    printf("socket retrieve success\n");

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(4000);

    bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

    if(listen(listenfd, 10) == -1){
        printf("Failed to listen\n");
        return -1;
    }

    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL); // accept awaiting request
        if (connfd < 0)
            error_server("ERROR on accept");
        pid = fork();
        std::cout<<"forked"<<std::endl;
        if (pid < 0)
            error_server("ERROR on fork");
        if (pid == 0)
        {
            std::cout<<"pid==0"<<std::endl;
            /*close(listenfd);*/
            dostuff(connfd);
            exit(0);
        }
        else close(connfd);

        strcpy(sendBuff, "Message from server");
        write(connfd, sendBuff, strlen(sendBuff));

        //close(connfd);
        //sleep(1);
    }

    return 0;//we never reach here
}
