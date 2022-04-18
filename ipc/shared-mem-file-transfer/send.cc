#include <fstream>      /* std::fstream */
#include <iostream>     /* std::cout */
#include <fcntl.h>      /* O_RDWR O_CREAT */
#include <sys/mman.h>   /* shm_open() */
#include <unistd.h>     /* ftruncate */


#define BUFFERSIZE  10

#define BackingFile "/backing-file"
#define SemaphoreName "/semaphore"
#define ByteSize 512
#define AccessPerms 0644
#define MemContents "Hello, world!"

typedef struct shmem_t
{
    volatile unsigned init_flag;    /* has the shared memory and control stuctures been initialized */
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    uint64_t data_version;          /* for tracking updates, 64-bit count won't wrap during lifetime of system */
    char text[BUFFERSIZE];
};


int main() {
    /* variables for reading from readFile */
    char readBuf[BUFFERSIZE];
    const char* readPath = "./input.txt";
    std::fstream readFile;

    /* variables for shared memory */
    int status;
    shmem_t *ptr;
    uint64_t last_version = 0;

    /* open readFile */
    readFile.open(readPath, std::fstream::in);
    if (!readFile.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return EXIT_FAILURE;
    }

    /* shared memory setup */
    int fd = shm_open(BackingFile, O_RDWR | O_CREAT, AccessPerms);  /* read/write, create if needed */
    if (fd < 0) {
        std::cerr << "Error shm_open" << std::endl;
        return EXIT_FAILURE;
    }

    ftruncate(fd, BUFFERSIZE);  /* get the bytes */

    /* semaphore code to lock the shared mem */
    caddr_t memptr = (caddr_t)mmap(NULL,                    /* let system pick where to put segment */
                                   BUFFERSIZE,              /* how many bytes */
                                   PROT_READ | PROT_WRITE,  /* access protections */
                                   MAP_SHARED,              /* mapping visible to other processes */
                                   fd,                      /* file descriptor */
                                   0);                      /* offset: start at 1st byte */
    if (memptr == (caddr_t)-1) {
        std::cerr << "Error mmap" << std::endl;
        return EXIT_FAILURE;
    }

}
