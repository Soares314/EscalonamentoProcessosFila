// C Program to demonstrate how to Implement a queue
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "Registradores.h"
#define MAX_SIZE 100
#define MAX_INPUT 10
#define NUMBER_PRIORITIES 4

typedef enum { NEW, READY, RUNNING, BLOCKED, FINISHED } ProcessState;

// Defining the Queue structure
typedef struct
{
    int PID;
    ProcessState state;
    char name[MAX_INPUT * 2];
    CPU_Registers registers;
    uint32_t allocatedMemory;
    uint32_t relapsedTime;
    int priority;
    int timeProcess[MAX_INPUT];
    char inputProcess[MAX_INPUT];
    int inputCurrent;

} Process;

int generatePID(){
    return rand();
}

Process* initializeProcess(char* n, uint64_t* numberIn, char* charIn, int count)
{
    Process* new_p = malloc(sizeof(Process));

    new_p->PID = generatePID();
    strcpy(new_p->name, n);
    new_p->state = NEW;

    // memset(&new_p->registers, 0, sizeof(process->registers));
    // new_p->allocatedMemory = allocatedMemory;
    new_p->relapsedTime = 0;
    new_p->priority = 1;

    for (int i = 0; i < count; i++) {
        new_p->timeProcess[i] = numberIn[i];
        new_p->inputProcess[i] = charIn[i];
        printf("Tempo %d: %d, Input %d: %c \n", i, new_p->timeProcess[i], i, new_p->inputProcess[i]);
    }

    new_p->inputCurrent = 0;

    return new_p;
}

void changeThread(Process* p){
    p->inputCurrent++;
}

