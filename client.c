#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 3599

void itoa(int number, char numberstring[]) {
    numberstring[0] = (char)(number + 48);
    numberstring[1] = '\0';
}

int main() {
    int sockfd, newsockfd, size, currentpacket = 1;
    char buffer[100];
    socklen_t len;
    struct sockaddr_in server, client;

    memset(&server, 0, sizeof(server));
    memset(&client, 0, sizeof(client));

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        printf("\n ERROR IN SOCKET CREATION...");
    else
        printf("\n socket created successfully....");

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    printf("\n starting up..");
    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        printf("\nbinding error..");
        exit(1);
    } else {
        printf("\n binding completed successfully waiting for connection...");
    }

    len = sizeof(client);
    if (listen(sockfd, 20) != -1) {
        if ((newsockfd = accept(sockfd, (struct sockaddr *)&client, &len)) == -1)
            printf("error in accepting connection...");
        else
            printf("accepted connection...\n");

        memset(&buffer, 0, sizeof(buffer));
        if (recv(newsockfd, buffer, 100, 0) == -1)
            printf("\n receive error! exiting...");
        else
            printf("/nrecieved a request from client.sending packets one by one..");

        do {
            memset(&buffer, 0, sizeof(buffer));
            itoa(currentpacket, buffer);
            send(newsockfd, buffer, strlen(buffer), 0);
            printf("\n packet send %d", currentpacket);
            memset(&buffer, 0, sizeof(buffer));
            recv(newsockfd, buffer, 100, 0);
            currentpacket++;
            if (strcmp(buffer, "RETRANSMIT") == 0) {
                currentpacket--;
                printf("\n** received a RETRANSMIT packet Resending last packet...");
            }
            sleep(1);
        } while (currentpacket != 6);
    } else {
        printf("\n error in listening...");
        exit(1);
    }

    close(sockfd);
    close(newsockfd);
    printf("\nsending complete. sockets closed.exiting...");
}