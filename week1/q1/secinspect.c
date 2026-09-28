#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);

    if (fd == -1) {
        perror("open");
        return 1;
    }

    printf("Opened file: %s\n", argv[1]);
    printf("File descriptor: %d\n", fd);
    struct stat file_info;

if (fstat(fd, &file_info) == -1) {
    perror("fstat");
    close(fd);
    return 1;
}

printf("File size: %ld bytes\n", (long) file_info.st_size);
printf("File permissions: %o\n", file_info.st_mode & 0777);
printf("Owner UID: %d\n", file_info.st_uid);
printf("Group GID: %d\n", file_info.st_gid);

    char buffer[128];

    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);

    if (bytes_read == -1) {
        perror("read");
        close(fd);
        return 1;
    }

    buffer[bytes_read] = '\0';

    printf("Bytes read: %zd\n", bytes_read);
    printf("File contents:\n%s", buffer);

    off_t offset = lseek(fd, 0, SEEK_SET);

    if (offset == -1) {
        perror("lseek");
        close(fd);
        return 1;
    }

    printf("\nFile offset reset to: %ld\n", (long) offset);

    if (close(fd) == -1) {
        perror("close");
        return 1;
    }

    printf("\nFile descriptor closed successfully.\n");
int out_fd = open("secinspect_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

if (out_fd == -1) {
    perror("open output");
    return 1;
}

const char *message = "secinspect write() test successful.\n";

ssize_t bytes_written = write(out_fd, message, 36);

if (bytes_written == -1) {
    perror("write");
    close(out_fd);
    return 1;
}

printf("Bytes written to secinspect_output.txt: %zd\n", bytes_written);

if (close(out_fd) == -1) {
    perror("close output");
    return 1;
}
    return 0;
}
