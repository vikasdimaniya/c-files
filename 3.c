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

int main(int argc, char *argv[]) {
    ssize_t bytes_read, bytes_written;
    int buffer_size = 1024;
    char buffer[buffer_size];
    if (argc != 3) {
        printf("program expects 2 arguments, but %d were given\n", argc-1);
        return 1;
    }
    //check if file argv[1] exists
    int read_file_descriptor = open(argv[1], O_RDWR);
    if (read_file_descriptor == -1) {
        printf("Failed to open file %s\n", argv[1]);
        return 1;
    }
    int write_file_descriptor = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (write_file_descriptor == -1) {
        perror("Failed to open file");
        printf("Failed to open file %s\n", argv[2]);
        return 1;
    }
    printf("Copying file %s to %s\n", argv[1], argv[2]);
    long total_bytes = 0;
    bytes_read = read(read_file_descriptor, buffer, buffer_size);
    while (bytes_read > 0) {
        total_bytes += bytes_read;
        bytes_written = write(write_file_descriptor, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            printf("Write failed\n");
            handle_close_file(read_file_descriptor);
            handle_close_file(write_file_descriptor);
            return 1;
        }
        bytes_read = read(read_file_descriptor, buffer, buffer_size);
    }
    if (bytes_read == -1) {
        printf("Read failed\n");
        handle_close_file(read_file_descriptor);
        handle_close_file(write_file_descriptor);
        return 1;
    }
    handle_close_file(read_file_descriptor);
    handle_close_file(write_file_descriptor);
    
    printf("Total bytes copied: %ld\n", total_bytes);
    printf("File copied successfully\n");
    return 0;
}

