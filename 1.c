#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>

//umaskex.c
int main()
{
    umask(0027);
    char filename[] = "file_io_test.txt";
    int fd = open(filename,O_CREAT|O_EXCL|O_WRONLY,0777);
    if (fd < 0) {
        // Check if the error was because the file already exists
        if (errno == EEXIST) {
            printf("File '%s' already exists, Not overwriting. Exiting.\n", filename);
        } else {
            perror("Error while creating file");
        }
        return 1; // Exit with failure code
    }
    printf("File '%s' created successfully.\n", filename);
    printf("file descriptor: %d\n", fd);

    char student_id[] = "110156183";
    ssize_t bytes_written = write(fd, student_id, sizeof(student_id));
    if (bytes_written < 0) {
        perror("Error while writing to file");
        close(fd);
        return 1; // Exit with failure code
    } else {
        printf("Wrote %ld bytes to file.\n", bytes_written);
    }

    if(close(fd)<0){
        perror("Error while closing file");
        return 1;
    }
    return 0;
}
