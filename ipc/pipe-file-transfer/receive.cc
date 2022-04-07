/* 
 * A combination of ./ipc/named-pipe and ./read-write-file (write part)
 * Reads the content of a pipe (./fifoChannel) into a buffer (writeBuf)
 * Then writes the content of this buffer into a file (./output.txt).
 * Repeat until the end-of-stream is reached.
 */

#include <fstream>      /* std::fstream */
#include <iostream>     /* std::cout */
#include <unistd.h>     /* read() close() unlink() */
#include <fcntl.h>      /* O_RDONLY */


#define BUFFERSIZE  1024

int main() {
    /* variables for writing to writeFile */
    char writeBuf[BUFFERSIZE];
    const char* writePath = "./output.txt";
    std::fstream writeFile;

    /* variables for pipe */
    const char* pipeName = "./fifoChannel";
    int fd; /* file descriptor */
    ssize_t count;

    /* open writeFile */
    writeFile.open(writePath, std::fstream::out | std::fstream::trunc);
    if (!writeFile.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return EXIT_FAILURE;
    }

    /* open pipe */
    fd = open(pipeName, O_RDONLY);
    if (fd < 0) {
        std::cerr << "Error opening pipe" << std::endl;
        return EXIT_FAILURE;
    }

    while (1) {
        count = read(fd, &writeBuf, BUFFERSIZE);

        /* end-of-stream */
        if (count == 0) 
            break;
        
        else {
            if (count < BUFFERSIZE) {
                writeBuf[count] = '\0';     /* necessary to print out buffer partially (at the end) */
            }
            std::cout << writeBuf << std::endl;
            std::cout << "---------" << std::endl;
            /* write to writeFile */
            writeFile.write(writeBuf, count);
        }
    }

    close(fd);          /* close pipe from read end */
    unlink(pipeName);   /* unlink from the file */

    return EXIT_SUCCESS;
}