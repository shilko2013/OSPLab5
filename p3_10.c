#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

#define BUF_LEN 4096

int main(int argc, char **argv) {
    int fd[2];
    if (argc != 2) {
        char *error_str = "FileName expected in arg\n";
        write(STDERR_FILENO, error_str, strlen(error_str));
        return 1;
    }
    if (pipe(fd) == -1) {
        perror("error in pipe");
        return 1;
    }
    int file;
    if ((file = open(argv[1], O_RDONLY)) < 0) {
        perror(argv[1]);
        return 1;
    }
    switch (fork()) {
        case -1:
            perror("error in fork");
            return 1;
        case 0:
            close(fd[0]);
            if (    dup2(fd[1], STDIN_FILENO) == -1) {
                perror("error in dup2");
            }
            if (execl("/usr/bin/wc", "wc", (char *) 0) == -1) {
                perror("error in execl");
            }
        default:
            close(fd[1]);
            char buf[BUF_LEN];
            char out[BUF_LEN];
            int r_buf = 0;
            int j;
            int i, flag = 0;
            while ((r_buf = read(file, buf, BUF_LEN)) > 0) {
                for (i = flag, j = 0; i < r_buf; i += 2, j++) {
                    out[j] = buf[i];
                }
                write(fd[0], out, j);
                flag = r_buf % 2;
            }
            if (r_buf == -1) {
                perror("error in read");
                return 1;
            }
            close(file);
            close(fd[0]);
            wait(NULL);
    }
    return 0;
}