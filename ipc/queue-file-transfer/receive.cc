#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include "queue.h"

#include <fstream>      /* std::fstream */
#include <iostream>     /* std::cout */
#include <string.h>     /* strcpy */

void report_and_exit(const char* msg) {
    perror(msg);
    exit(-1);   /* EXIT_FAILURE */
}

int main() {
    /* variables for writing to writeFile */
    char writeBuf[BUFFERSIZE];
    const char* writePath = "./output.txt";
    std::fstream writeFile;

    /* variables for message queue */
    int qid;
    int n = 1;
    key_t key;

    /* open writeFile */
    writeFile.open(writePath, std::fstream::out | std::fstream::trunc);
    if (!writeFile.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return EXIT_FAILURE;
    }

    /* setup message queue */
    key = ftok(PathName, ProjectId);    /* key to identify the queue */
    if (qid<0) {
        std::cerr << "error ftok" << std::endl;
        return EXIT_FAILURE;
    }

    qid = msgget(key, 0666 | IPC_CREAT); /* access if created already */
    if (qid < 0) {
        std::cerr << "Error msgget" << std::endl;
        return EXIT_FAILURE;
    }

    while (1) {
        queuedMessage msg;  
        if (msgrcv(qid, &msg, sizeof(msg), n, MSG_NOERROR | IPC_NOWAIT) < 0) {
            std::cerr << "Error msgrcv" << std::endl;
            return EXIT_FAILURE;
        }

        strcpy(writeBuf, msg.payload);
        std::cout << "---------" << std::endl;
        std::cout << "index: " << (int) msg.index << std::endl;
        std::cout << "sizeMessage: " << msg.sizeMessage << std::endl;
        std::cout << writeBuf << std::endl;
        std::cout << "endIndex: " << (int) msg.endIndex << std::endl;
        std::cout << "---------" << std::endl;
        writeFile.write(writeBuf, msg.sizeMessage);

        /* break loop if file fully received */
        if (msg.index == msg.endIndex) {
            std::cout << "File received fully" << std::endl;
            break;
        }

        n +=1;

    }

    /* remove the queue */
    if (msgctl(qid, IPC_RMID, NULL) < 0) {
        std::cerr << "Error msgctl" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

}