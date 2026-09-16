// C Program to demonstrate how to Implement a queue
#include <stdbool.h>
#include <stdio.h>
#define MAX_SIZE 100  

// Defining the Queue structure
typedef struct {
    int eax;       // Registrador de uso geral
    int ebx;       // Registrador de uso geral
    int ecx;       // Registrador de uso geral
    int edx;       // Registrador de uso geral
    
    int esp;       // Stack Pointer (Aponta para o topo da pilha)
    int ebp;       // Base Pointer (Aponta para a base da pilha)
    
    int esi;       // Source Index (Usado para strings/arrays)
    int edi;       // Destination Index
    
    int eip;       // Instruction Pointer (Guarda a próxima linha de código a executar)
    int eflags;    // Registrador de status (flags de sistema)
} CPU_Registers;