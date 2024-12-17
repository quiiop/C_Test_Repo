#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define SOCKET_PATH "/tmp/mysocket"

int main(void){
    printf("HI This is server.c\n");

    int server_fd, client_fd;
    struct sockaddr_un address; //un是指這個socket用於Unix Domain
    char buffer[100];

    //Step 1. create socket, return socket file description
    /*
        1. AF_UNIX 表示這個socket是Unix Domain Socket, Unix Domain Socket專門用於Process之間通信
        2. Unix Domain Socket不依赖于IP地址和端口，而是通过文件路径来标识, SOCKET_PATH "/tmp/mysocket"
    */
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        return -1;
    }

    //Step 2. 設定address
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, SOCKET_PATH);

    //Step 3. 把socket和address綁定 
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("Bind failed");
        return -1;
    }

    //Step 4. 設定這個socket的監控requester數量, 5就是最多5個client, 這裡我猜是因為socket背後有wait queue, 所以需要先指定requester cnt才可以去設定wait queue大小
    /*
        1. 之後client要會用socket connect API連過來
    */
    if (listen(server_fd, 5) == -1) {
        perror("Listen failed");
        return -1;
    }

    printf("Server waiting for connection...\n");

    //Step 5. 這裡我沒有trce code還不確定, 但我猜accept實際上是去看socket的wait queue有沒有requester, 平常沒有requester的話這邊會被Block住
    /*
        1. int accept (int __fd, __SOCKADDR_ARG __addr, socklen_t *__restrict __addr_len);
        __addr和__addr_len是用來紀錄現在是哪個client連進server
    */
    struct sockaddr client_addr;
    socklen_t client_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("Accept failed");
        return -1;
    }

    //Step 6. read client送來的message
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer));
    if (bytes_read == -1) {
        perror("Read failed");
    } else if (bytes_read == 0) {
        printf("No data received.\n");
    } else {
        buffer[bytes_read] = '\0';  // Ensure string is null-terminated
        printf("Received message: %s\n", buffer);
    }

    // Close sockets
    close(server_fd);
    close(client_fd);
    //Step 7. 記得要移除Unix Domain File
    unlink(SOCKET_PATH);

    return 0;
}
