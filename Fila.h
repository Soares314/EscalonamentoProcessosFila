// C Program to demonstrate how to Implement a queue
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>
#include "PCB.h"
#define MAX_SIZE 100
#define NUMBER_PRIORITIES 4

// Defining the Queue structure
typedef struct{
    Process* processes[MAX_SIZE];
    int front;
    int rear;
} QueueProcess;

// Function to initialize the queue
QueueProcess* initializeQueue(){
    QueueProcess* q = malloc(sizeof(QueueProcess));
    q->front = -1;
    q->rear = -1;

    return q;
}

void initializeReadyQueues(QueueProcess* priorityQueues[], int numberQueues){
    for (int i = 0; i < numberQueues; i++) {
        priorityQueues[i] = initializeQueue();
    }
}

// Function to check if the queue is empty
int isEmpty(QueueProcess *q) {
    return q->front == -1;
}

// Function to get the number of elements in the queue
int getQueueSize(QueueProcess *q){
    if (isEmpty(q)) {
        return 0;
    }

    if (q->rear >= q->front) {
        return q->rear - q->front + 1;
    }

    return MAX_SIZE - q->front + q->rear + 1;
}

// Function to check if the queue is full
bool isFull(QueueProcess *q){
    return (q->rear + 1) % MAX_SIZE == q->front;
}

// Function to add an element to the queue (Enqueue
// operation)
void addElement(QueueProcess *q, Process* novoProcess){
    if (isFull(q)) {
        printf("Queue Overflow: Cannot insert %d\n", novoProcess);
        return;
    }
    if (q->front == -1) q->front = 0; // First element
    
    q->rear = (q->rear + 1) % MAX_SIZE;
    q->processes[q->rear] = novoProcess;
}

// Function to remove an element from the queue (Dequeue
// operation)
void removeElement(QueueProcess *q){
    if (isEmpty(q)) {
        printf("Queue Underflow: Nothing to delete\n");
        return ;
    }

    if (q->front == q->rear) {
        q->front = q->rear = -1; // Reset queue
    } else {
        q->front = (q->front + 1) % MAX_SIZE;
    }

}

// Function to get the element at the front of the queue
// (Peek operation)
Process* getElement(QueueProcess *q){
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return NULL;
    }
    return q->processes[q->front];
}

// Function to print the current queue
void printQueue(QueueProcess *q){
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return;
    }

    int i = q->front;
    while (i != q->rear) {
        Process *procesoPrint = q->processes[i];
        printf("%s ", procesoPrint->name);
        i = (i + 1) % MAX_SIZE;
    }
    printf("%d\n", q->processes[q->rear]);
}

