#include "kernel/types.h"
#include "user/user.h"

// pingpong: communication between two processes

int main() {
    int pid;
    int fds_p[2], fds_c[2];
    char buf = 'A';
    
    // create a pipe, with two FDs in fds[0], fds[1].
    pipe(fds_p);
    pipe(fds_c);

    pid = fork();
    if (pid == 0) {
        pid = getpid(); // child process id
        close(fds_p[1]); // child process close fds_p[1]
        close(fds_c[0]); // child process close fds_c[0]

        read(fds_p[0], &buf, 1); // child process read from fds_p[0]
        printf("%d: received ping\n", pid); // child process print

        write(fds_c[1], &buf, 1); // child process write to fds_c[1]
        exit(0);
    } else {
        pid = getpid(); // parent process id
        close(fds_p[0]); // parent process close fds_p[0]
        close(fds_c[1]); // parent process close fds_c[1]
        write(fds_p[1], &buf, 1); // parent process write to fds_p[1]

        read(fds_c[0], &buf, 1); // parent process read from fds_c[0]
        printf("%d: received pong\n", pid);
        exit(0);
    }

    exit(0);
}