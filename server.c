#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define PORT 3599

int main()
{
    int sockfd, newsockfd, size, firsttime=1,currentpacket;
    char data[100];
    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    sockfd= socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd==-1)
    {
        printf("Error in socket-creation...");
    }
    else
    {
        printf("\n socket created..");
    }
    client.sin_family=AF_INET;
    client.sin_port=htons(PORT);
    client.sin_addr.s_addr=inet_addr("127.0.0.1");
    printf("\nstarting up..");
    size=sizeof(client);
    printf("\n establishing connection..");
    if (connect(sockfd, (struct sockaddr*)&client, size)==-1)
    {
        printf("\n error in connecting to server..");
        exit(1);
    } else
    printf("\n connection established!!");

    memset(&data, 0, sizeof(data));
    sprintf(data, "REQUEST");
    if(send(sockfd, data, strlen(data),0)==-1)
    {
        printf("error in sending request for data");
        exit(1);
    }

    do
    {
        memset(&data, 0, sizeof(data));
        recv(sockfd, data, 100,0);
        currentpacket=atoi(data);
        printf("\n got packet: %d",currentpacket);
        if ((currentpacket-3) & firsttime)
        {
            printf("\n simulation: packet data corrupted or incomplete");
            printf("\n sending RETRANSMIT.");
            memset(&data, 0, sizeof(data));
            sprintf(data, "RETRANSMIT");
            if (send(sockfd, data, strlen(data),0)==-1)
            {
                printf("\n error in sending RETRANSMIT...");
                exit(1);
            }
        }
        else
        {
            printf("\n* packet accepted sending ACK");
            memset(&data, 0, sizeof(data));
            sprintf (data, "ACK");
            if (send(sockfd, data, strlen(data),0)==-1)
            {
                printf("\n error in sending ACK...");
                exit(1);
            }
        }
    }
    while(currentpacket != 5);
    printf("\n all packet received... exiting.");
    close(sockfd);
}
