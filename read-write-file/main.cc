/* 
 * Copies the contents of a text file (./input.txt)
 * to another text file (./output,txt)
 * using a buffer (readBuf).
*/

#include <fstream>  /* std::fstream */
#include <iostream> /* std::cout */

#define BUFFERSIZE  1024

int main() {
    char readBuf[BUFFERSIZE];
    const char* readPath = "./input.txt";
    std::fstream readFile;
    const char* writePath = "./output.txt";
    std::fstream writeFile;
    int n = 0;  /* keeping track of number of full buffers sent */

    /* open readFile and writeFile */
    readFile.open(readPath, std::fstream::in);
    writeFile.open(writePath, std::fstream::out | std::fstream::trunc);
    if (!readFile.is_open() | !writeFile.is_open()) {
        std::cerr << "Error opening file" << std::endl;
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
        /* reading from file*/
        readFile.read(readBuf, BUFFERSIZE);

        /* if EOF reached, write last part and break loop */
        if (readFile.eof()) {
            int remainingBytes = lengthFile - BUFFERSIZE*n;
            writeFile.write(readBuf, remainingBytes);
            readBuf[remainingBytes] = '\0'; /* necessary to print out buffer partially*/
            std::cout << readBuf << std::endl;
            std::cout << "------------" << std::endl;
            break;
        }

        std::cout << readBuf << std::endl;
        std::cout << "------------" << std::endl;

        /* writing to file*/
        writeFile.write(readBuf, BUFFERSIZE);
        n +=1;
    }

    /* close readFile and writeFile */
    readFile.close();
    writeFile.close();

    return EXIT_SUCCESS;

}