#include "shm.h"

#include <fstream>      /* std::fstream */
#include <fcntl.h>      /* O_RDWR O_CREAT */
#include <iostream>     /* std::cout */
#include <sys/mman.h>   /* shm_open() */
#include <unistd.h>     /* ftruncate() */

int main() {
    /* variables for reading from readFile */
    char readBuf[BUFFERSIZE];
    const char* readPath = "./input.txt";
    std::fstream readFile;
    int readBytes = 0;

    /* open readFile */
    readFile.open(readPath, std::fstream::in);
    if (!readFile.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return EXIT_FAILURE;
    }

    /* setup shared memory */
    int fd = shm_open(BACKINGFILE, O_RDWR | O_CREAT, ACCESSPERMS);
    if (fd < 0) {
        std::cerr << "Error shm_open" << std::endl;
        return EXIT_FAILURE;
    }

    ftruncate(fd, sizeof(shmMessage));  

    caddr_t memptr = (caddr_t)mmap(NULL,                /* let system pick where to put segment */
                                sizeof(shmMessage),     /* how many bytes */
                                PROT_READ | PROT_WRITE, /* access protections */
                                MAP_SHARED,             /* mapping visible to other processes */
                                fd,                     /* file descriptor */
                                0);                     /* offset: start at 1st byte */
    if (memptr == (caddr_t)-1) {
        std::cerr << "Error mmap" << std::endl;
        return EXIT_FAILURE;
    }

    fprintf(stderr, "shared mem address: %p [0..%d]\n", memptr, sizeof(shmMessage)-1);

    
}