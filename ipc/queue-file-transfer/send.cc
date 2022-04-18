#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#include <fstream>      /* std::fstream */
#include <iostream>     /* std::cout */
#include <sys/stat.h>   /* S_IRWXU S_IRWXG */

int main() {
    /* variables for reading from readFile */
    char readBuf[BUFFERSIZE];
    const char* readPath = "./input.txt";
    std::fstream readFile;
    long n = 1;  /* keeping track of number of full buffer sent */
    int readBytes = 0;

    /* open readFile */
    readFile.open(readPath, std::fstream::in);
    if (!readFile.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return EXIT_FAILURE;
    }

    /* determine length of file */
    // readFile.seekg(0, std::ios::beg);   /* set position to begin of file */
    // auto begin = readFile.tellg();
    // readFile.seekg(0, std::ios::end);   /* set position to end of file */
    // auto end = readFile.tellg();
    // int lengthFile = (int)(end-begin);  /* store length of file */
    // readFile.seekg(0, std::ios::beg);   /* set position back to begin */

    /* ftok - convert a pathname and a project identifier to a System V IPC key */
    key_t key = ftok(PathName, ProjectId);
    if (key < 0) {
        std::cerr << "Error ftok" << std::endl;
        return EXIT_FAILURE;
    }

    /* msgget - get a System V message queue identifier */
    int qid = msgget(key, S_IRWXU | S_IRWXG | IPC_CREAT);
    if (qid < 0) {
        std::cerr << "Error msgget" << std::endl;
        return EXIT_FAILURE;
    }

    while (1) {
        /* reading from readFile */
        readBytes = readFile.read(readBuf, BUFFERSIZE).gcount();

        /* if EOF reached, send last part and break loop */
        if (readFile.eof()) {
            // int remainingBytes = lengthFile - BUFFERSIZE*(n-1); 
            /* build the message */
            readBuf[readBytes] = '\0'; /* necessary to copy buffer partially */
            queuedMessage msg;
            msg.index = n;
            msg.endIndex = n;
            msg.sizeMessage = readBytes;
            strcpy(msg.payload, readBuf);
            /* send the message */
            msgsnd(qid, &msg, sizeof(msg), IPC_NOWAIT); /* don't block */
            std::cout << "---------" << std::endl;
            std::cout << "index: " << msg.index << std::endl;
            std::cout << "sizeMessage: " << msg.sizeMessage << std::endl;
            std::cout << msg.payload << std::endl;
            std::cout << "---------" << std::endl;
            break;
        }

        /* build the message */
        queuedMessage msg;
        msg.index = n;
        msg.endIndex = (n+1);
        msg.sizeMessage = BUFFERSIZE;
        strcpy(msg.payload, readBuf);
        /* send the message */
        msgsnd(qid, &msg, sizeof(msg), IPC_NOWAIT); /* don't block */
        std::cout << "---------" << std::endl;
        std::cout << "index: " << (int) msg.index << std::endl;
        std::cout << "sizeMessage: " << msg.sizeMessage << std::endl;
        std::cout << msg.payload << std::endl;
        std::cout << "---------" << std::endl;

        n +=1;
    }

    /* close readFile */
    readFile.close();

    return EXIT_SUCCESS;

}