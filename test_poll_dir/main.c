#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define FIFO_PATH "/tmp/myfifo"

int main() {
    // 創建 named pipe
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        perror("mkfifo failed");
        exit(EXIT_FAILURE);
    }

    printf("Named pipe created: %s\n", FIFO_PATH);

    // 打開 named pipe 寫入端
    int fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    printf("Writer: FIFO opened for writing\n");

    // 模擬寫入數據
    const char *messages[] = {
        "Hello, process B!\n",
        "This is a second message.\n",
        "Goodbye, process B!\n"
    };

    for (int i = 0; i < 3; i++) {
        printf("Writer: Writing message %d\n", i + 1);
        write(fd, messages[i], strlen(messages[i]));
        sleep(2); // 模擬一些延遲
    }

    close(fd);
    printf("Writer: Finished writing\n");

    return 0;
}
