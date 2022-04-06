#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include <fstream>
#include <iostream>

#define MaxLoops         12000   /* outer loop */
#define ChunkSize           16   /* how many written at a time */
#define IntsPerChunk         4   /* four 4-byte ints per chunk */
#define MaxZs              250   /* max microseconds to sleep */

#define BUFFERSIZE  10

int main() {
  const char* pipeName = "./fifoChannel";
  const char *path = "./input.txt";
  int status;

  // open file to send
  std::fstream readFile;
  readFile.open(path, std::fstream::in);
  if (!readFile.is_open()) {
    std::cerr << "Read file couldn't be opened" << std::endl;
  }
  std::cout << "reading" << std::endl;

  // open fifo
  status = mkfifo(pipeName, 0666);                      /* read/write for user/group/others */
  std::cout << "reading" << std::endl;
  if (status == -1) {
    if (errno == EEXIST) {
      remove(pipeName);
      status = mkfifo(pipeName, S_IRUSR | S_IWUSR);
      if (status == -1) {
        std::cerr << "Couldn't open fifo" << std::endl;
      }
    }
  }
  std::cout << "reading" << std::endl;
  int fd = open(pipeName, O_WRONLY); /* open as write-only */
  std::cout << "opened" << std::endl;
  if (fd < 0) return -1;                       /* can't go on */

  std::cout << "reading" << std::endl;

  char sendBuf[BUFFERSIZE];

  while (readFile.eof() == false) {
    readFile.read(sendBuf, BUFFERSIZE);
    std::cout << sendBuf << std::endl;
    std::cout << "reading" << std::endl;
  }

  // int i;
  // for (i = 0; i < MaxLoops; i++) {          /* write MaxWrites times */
  //   int j;
  //   for (j = 0; j < ChunkSize; j++) {       /* each time, write ChunkSize bytes */
  //     int k;
  //     int chunk[IntsPerChunk];
  //     for (k = 0; k < IntsPerChunk; k++)
  //       chunk[k] = rand();
  //     write(fd, chunk, sizeof(chunk));
  //   }
  //   usleep((rand() % MaxZs) + 1);           /* pause a bit for realism */
  // }

  // close(fd);           /* close pipe: generates an end-of-stream marker */
  // unlink(pipeName);    /* unlink from the implementing file */
  // printf("%i ints sent to the pipe.\n", MaxLoops * ChunkSize * IntsPerChunk);

  return 0;
}