#define ProjectId 123
#define PathName "queue.h"  /* any existing, accessible file would do */

#define MsgLen      10
#define MsgCount    6

#define BUFFERSIZE 1024

typedef struct {
    long index;                  /* must be of type long */
    long endIndex;          
    int sizeMessage;
    char payload[BUFFERSIZE];   /* bytes in the message */
} queuedMessage;

