#define BUFFERSIZE 10

#define ACCESSPERMS 0644
#define BACKINGFILE "/backing-file"
#define SEMAPHORENAME "semaphore"

typedef struct {
    bool finalPacket;         
    int sizeMessage;
    char payload[BUFFERSIZE];   /* bytes in the message */
} shmMessage;
