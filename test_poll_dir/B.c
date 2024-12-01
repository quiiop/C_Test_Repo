#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_PATH "/tmp/myfifo"
#define TIMEOUT 10000 // 超时时间 (毫秒)


int main(void)
{
    // Open FIFO file
    int fd = open(FIFO_PATH, O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    printf("Reader: FIFO opened for reading\n");

    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;

    char buff[1024];
    while(1){
        printf("Reader: Waiting for data...\n");

        int poll_ret = poll(&pfd, 1, TIMEOUT);

        if (poll_ret < 0){
            perror("poll failed");
            break;
        }else if (poll_ret == 0){
            printf("Reader: Timeout, no data available.\n");
            sleep(2);
            continue;
        }

        if (pfd.revents == POLLIN){
            memset(buff, 0, sizeof(buff));
            int reading_cnt = read(pfd.fd, buff, sizeof(buff)-1);
            buff[sizeof(buff)-1] = '\0';

            if (reading_cnt > 0) {
                printf("Reader: Received: %s", buff);
            } else if (reading_cnt == 0) {
                printf("Reader: Writer closed the pipe.\n");
                break;
            } else {
                perror("read failed");
            }
        }
    }

    close(pfd.fd);
    printf("Reader: Exiting\n");
    return 0;
}