#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include <stdlib.h>


void handle_close_file (int fd) {
    if (close(fd) < 0) {
        perror("close failed");
        exit(1);
    }
}

void handle_seek_error (off_t pos, int fd) {
    if (pos == (off_t)-1){
        perror("lseek failed");
        handle_close_file(fd);
        exit(1);
    }
}

int main() {
    char filename[] = "file_io_test.txt";
    int fd;
    fd = open(filename, O_RDWR);
    if (fd == -1) {
        printf("File open failed\n");
        exit(1);
    }

    off_t pos = lseek(fd, 20, SEEK_SET);
    handle_seek_error(pos, fd);

    printf("Current position: %ld\n", (long)pos);
    char name[] = "kumar vikas";
    int bytes_written = write(fd, name, sizeof(name)-1);
    if (bytes_written == -1) {
        perror("write failed");
        handle_close_file(fd);
        exit(1);
    }
    printf("Wrote %d bytes\n", bytes_written);
    
    off_t start_pos = lseek(fd, 0, SEEK_SET);
    handle_seek_error(start_pos, fd);
    
    char buffer[100];
    int bytes_read = read(fd, buffer, sizeof(buffer)-1);
    if (bytes_read == -1) {
        perror("read failed");
        handle_close_file(fd);
        exit(1);
    }
    buffer[bytes_read] = '\0';
    
    
    printf("Read %d bytes: %s\n", bytes_read, buffer);

    /**
     * The printf function stops reading the buffer when it encounters a null character.
     * But as we seeked forwarded to 20th bytes when there were only 11 bytes in the file,
     * the prinf stops reading after it encounters the null character.
     * Hence we need to print the buffer byte by byte.
     */
    for (int i = 0; i < bytes_read; i++) {
        // Print the byte as a character if it's printable, otherwise print in hex
        if (buffer[i] != '\0') {
            printf("%c", buffer[i]);  // Print as character
        } else {
            printf("\\0");  // Represent null character as "\0"
        }
    }
    handle_close_file(fd);
    return 0;
}
