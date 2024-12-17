#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/mysocket"

int main(void) {
    int sock_fd;
    struct sockaddr_un addr;
    char *message = "Hello from client";

    // Create socket
    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("Socket creation failed");
        return -1;
    }

    // 這裡SOCKET_PATH "/tmp/mysocket"在設置client和server通訊的文件路徑
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    // Connect to server
    if (connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("Connect failed");
        close(sock_fd);
        return -1;
    }

    // Write data to server
    ssize_t bytes_written = write(sock_fd, message, strlen(message));
    if (bytes_written == -1) {
        perror("Write failed");
    } else {
        printf("Sent message: %s\n", message);
    }

    close(sock_fd);
    return 0;
}
