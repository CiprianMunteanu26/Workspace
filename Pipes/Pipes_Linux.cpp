#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <cstdlib>
#include <iostream>

using namespace std;

int numbers = 10000; //total numbers
int noProcesses = 10;  //number of processes

//checks if the n number is prime or not
bool isPrime(int n) {
    int i;
    for (i = 2; i <= n / 2; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

void childProcess(int readPipe, int writePipe) {
    int j, i = 1;
    int vector[1000] = {};
    //we read the number of process from pipe
    read(readPipe, &i, sizeof(int));
    for (j = i * (numbers / noProcesses) + 1; j <= (i + 1) * (numbers / noProcesses); j++) {
        if (j <= numbers) {
            if (isPrime(j)) {
                vector[0]++; //vector[0] is the size of the prime numbers array
                vector[vector[0]] = j; //we add j to array as it would be a prime number
            }
        }
    }
    write(writePipe, &vector, 4000);
    exit(1);
}

void parentProcess(int readPipe, int writePipe) {
    int i, j;
    int value[1000] = {};

    //for every process we write the number in the pipe
    for (j = 0; j < noProcesses; j++) {
        //we write down in the pipe the number of process
        write(writePipe, &j, sizeof(int));
    }
    for (j = 0; j < noProcesses; j++) {
        printf("Process number %d: ", j);
        //we read from pipe every single prime number of every single process
        read(readPipe, value, 4000);
        for (i = 1; i <= value[0]; i++) {
            printf("%d, ", value[i]);
        }
        printf("\n");
    }
}

int main() {
    int i, j;
    int firstPipe[2], secondPipe[2];
    int processID = 1;

    if (pipe(firstPipe)) {
        printf("There is an error creating the pipe.\n");
        exit(-1);
    }

    if (pipe(secondPipe)) {
        printf("There is an error creating the pipe.\n");
        exit(-1);
    }
    for (j = 0; j < noProcesses; j++) {
        processID = fork();
        if (processID) {
            if (processID == -1) {
                printf("There is an error creating the child process.\n");
                exit(-1);
            }
        } else {
            childProcess(firstPipe[0], secondPipe[1]);
        }
    }
    parentProcess(secondPipe[0], firstPipe[1]);
}