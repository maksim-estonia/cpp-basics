#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

void report_and_exit(const char* msg) {
    perror(msg);
    exit(-1);   /* EXIT_FAILURE */
}

int main() {
    /* ftok - convert a pathname and a project identifier to a System V IPC key */
    /* 
     * The ftok() function uses the identity of the file named by the
     * given pathname (which must refer to an existing, accessible file)
     * and the least significant 8 bits of proj_id (which must be nonzero)
     * to generate a key_t type System V IPC keym suitable for
     * use with msgget(2), semget(2), or shmget(2).
     * 
     * The resulting value is the same for all pathnames that name the
     * same file, when the same value of proj_id is used. 
     * The value returner should be different when the (simultanously existing) files
     * or the project IDs differ.
     */
    key_t key = ftok(PathName, ProjectId);
    if (key < 0) report_and_exit("couldn't get key...");

    /* msgget - get a System V message queue identifier
     * 
     * The msgget() system call returns the System V message queue
     * identifier associated with the value of the key argument.
     * It may be used either to obtain the identifier of a previously created
     * message queue (when msgflg is zero and key does not have 
     * the value IPC_PRIVATE), or to create a new set.
     */
    int qid = msgget(key, 0666 | IPC_CREAT);
    if (qid < 0) report_and_exit("couldn't get queue id...");

    const char* payloads[] = {"msg1", "msg2", "msg3", "msg4", "msg5", "msg6"};
    int types[] = {1, 1, 2, 2, 3, 3};   /* each must be > 0 */
    int i;
    for (i = 0; i < MsgCount; i++) {
        /* build the message */
        queuedMessage msg;
        msg.type = types[i];
        strcpy(msg.payload, payloads[i]);

        /* send the message */
        msgsnd(qid, &msg, sizeof(msg), IPC_NOWAIT); /* don't block */
        printf("%s sent as type %i\n", msg.payload, (int)msg.type);
    }
    return 0;

}