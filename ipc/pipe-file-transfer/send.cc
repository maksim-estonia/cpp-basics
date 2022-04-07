/* 
 * A combination of ./ipc/named-pipe and ./read-write-file (read part)
 * Reads the content of a text file (./input.txt) into a buffer (readBuf)
 * Then writes the content of this buffer into a pipe (./fifoChannel).
 * Repeat until the full text file has been sent.
 */

#include <fstream>      /* std::fstream */
#include <iostream>     /* std::cout */
#include <sys/stat.h>   /* mkfifo() */
#include <unistd.h>     /* write() */
#include <fcntl.h>      /* O_CREAT, O_WRONLY */

#define BUFFERSIZE  10

int main() {
    /* variables for reading from readFile */
    char readBuf[BUFFERSIZE];
    const char* readPath = "./input.txt";
    std::fstream readFile;
    int n = 0;  /* keeping track of number of full buffer sent */

    /* variables for pipe */
    const char* pipeName = "./fifoChannel";
    int fd;
    
    /* open readFile */
    readFile.open(readPath, std::fstream::in);
    if (!readFile.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return EXIT_FAILURE;
    }

    /* pipe setup */
    mkfifo(pipeName, 0666); /* read/write for user/group/others */
    fd = open(pipeName, O_CREAT | O_WRONLY);
    if (fd < 0) {
        std::cerr << "Error opening pipe" << std::endl;
        return EXIT_FAILURE;
    }

    /* determine length of file */
    readFile.seekg(0, std::ios::beg);   /* set position to begin of file */
    auto begin = readFile.tellg();
    readFile.seekg(0, std::ios::end);   /* set position to end of file */
    auto end = readFile.tellg();
    int lengthFile = (int)(end-begin);  /* store length of file */
    readFile.seekg(0, std::ios::beg);   /* set position back to begin */

    std::cout << "TOTAL LENGTH: " << lengthFile << std::endl;

    while (1) {
        /* reading from readFile */
        readFile.read(readBuf, BUFFERSIZE);

        /* if EOF reached, write last part and break loop */
        if (readFile.eof()) {
            int remainingBytes = lengthFile - BUFFERSIZE*n;
            /* write to pipe (partial buffer)*/
            write(fd, readBuf, remainingBytes);
            readBuf[remainingBytes] = '\0'; /* necessary to print out buffer partially */
            std::cout << readBuf << std::endl;
            std::cout << "---------" << std::endl;
            break;
        }

        std::cout << readBuf << std::endl;
        std::cout << "---------" << std::endl;

        /* write to pipe (full buffer)*/
        write(fd, readBuf, BUFFERSIZE);

        n +=1;
    }

    /* close readFile */
    readFile.close();
    
    /* close pipe: generates an end-of-stream marker */
    close(fd);
    /* unlink from the file */
    unlink(pipeName);

    return EXIT_SUCCESS;

} 