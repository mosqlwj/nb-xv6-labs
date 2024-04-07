#include "kernel/types.h"
#include "user/user.h"

// primes
void handle(int fd) {
    int j, k;
    int pid;
    int fds[2];

    if (read(fd, &j, sizeof(int)) > 0) {
        printf("prime %d\n", j);

        pipe(fds);
        pid = fork();
        if (pid < 0) {
            printf("fork failed\n");
            exit(1);
        } else if (pid == 0) {
            close(fds[1]);
            handle(fds[0]);
        } else {
            close(fds[0]);
            while (read(fd, &k, sizeof(int)) > 0) {
                if (k % j != 0) {
                    write(fds[1], &k, sizeof(int));
                }
            }

            close(fds[1]);
            close(fd);
            wait(0);
        }
        

    } else {
        close(fd);
    }
}
int main() {
    int i;
    int pid;
    int fds[2];

    pipe(fds); // create pipe

    pid = fork();

    if (pid < 0) {
        printf("fork failed\n");
        exit(1);
    } else if (pid == 0) {
        close(fds[1]); 
        handle(fds[0]);
    } else {
        close(fds[0]); // close read end
        for (i = 2; i < 35; i++) { // send numbers
            write(fds[1], &i, sizeof(int));
        };
        close(fds[1]); // close write end
        wait(0); // wait for child
    }


    exit(0);
}