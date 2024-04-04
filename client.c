#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char buffer2[1024] = {0};

    // Tạo file descriptor cho máy khách
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Chuyển đổi địa chỉ IPv4 và định dạng dạng chuỗi
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Kết nối tới máy chủ
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    while (1) {
        printf("Enter message to send to server: ");
        fgets(buffer2, 1024-1, stdin);
        if (strcmp(buffer, "goodbye\n") == 0) {
	    printf("Exiting...\n");
	    break;
}
        send(sock , buffer2 , strlen(buffer2) , 0 );

        valread = read( sock , buffer, 1024-1);
        printf("Server: %s\n",buffer );
    }

    return 0;
}

