#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "Fila.h"
#define STANDARD_QUANTUM 100
# define MAX_LINE 1000

uint64_t quantum = STANDARD_QUANTUM;
int flag = -1;
QueueProcess* priorityQueues[NUMBER_PRIORITIES];
QueueProcess* blockedQueue;
Process* runningProcess;

static FILE *arquivo = NULL;
static char linhaLida[MAX_LINE];


char* initialize();
void manageProcesses();
char* getInput();
char* execProcess(Process* processExec, uint64_t time, char inputToContinue);

char* initialize(){
    printf("[INIT] Inicializando escalonador...\n");
    initializeReadyQueues(priorityQueues, NUMBER_PRIORITIES);
    printf("[INIT] Filas de prioridade inicializadas.\n");
    blockedQueue = initializeQueue();
    printf("[INIT] Fila de bloqueados inicializada.\n");
    flag = 0;
    
    // Abre o arquivo só na primeira chamada
    if (arquivo == NULL) {
        printf("[INIT] Abrindo arquivo input.txt...\n");
        arquivo = fopen("input.txt", "r");
        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo\n");
            return NULL;
        }
        printf("[INIT] Arquivo aberto com sucesso.\n");
    }

    while(flag != -1){

        printf("[INIT] Lendo proxima entrada...\n");
        getInput();
        manageProcesses();

    }

    for(int i = 0; i < NUMBER_PRIORITIES; i++){
        printQueue(priorityQueues[i]);
    }

    printQueue(blockedQueue);

    printf("[INIT] Escalonador finalizado.\n");
    return "OS powered off";
}

void manageProcesses(){
    printf("[GERENCIA] Procurando processo pronto para executar...\n");

    int i = 0;
    for(i = 0; i < NUMBER_PRIORITIES; i++){
        if(!isEmpty(priorityQueues[i])){
            printf("[GERENCIA] Processo encontrado na prioridade %d.\n", i);
            
            Process* processToBe = getElement(priorityQueues[i]);
            printf("[GERENCIA] Iniciando execucao do processo.\n");
            execProcess(processToBe, processToBe->timeProcess[processToBe->inputCurrent], processToBe->inputProcess[processToBe->inputCurrent]);
            break;
        }
    }

    if(flag == 1 && i == NUMBER_PRIORITIES){
        flag = -1;
    } 

}

// Pega o input, já cria o processo com base nele e coloca na fila
char* getInput(){
    uint64_t numberInput[MAX_INPUT];
    char charInput[MAX_INPUT];
    int count = 0;

    // Lê a próxima linha do Input
    if (fgets(linhaLida, MAX_LINE, arquivo) == NULL) {
        // Chegou no fim do arquivo
        printf("[INPUT] Fim do arquivo alcançado.\n");
        fclose(arquivo);
        arquivo = NULL;
        flag = 1;
        return "Input is empty";
    }
    printf("Pegando input denovo...");

    // Remove o '\n' do final, se tiver
    linhaLida[strcspn(linhaLida, "\n")] = '\0';
    printf("[INPUT] Entrada lida: %s\n", linhaLida);

    int len = strlen(linhaLida);

    if (len == 1 && !isdigit(linhaLida[0])) {
        printf("[INPUT] Entrada representa desbloqueio de processo.\n");
        
        if(!isEmpty(blockedQueue)){
            Process* blockedProcess = getElement(blockedQueue);
            if(linhaLida[0] == blockedProcess->inputProcess[blockedProcess->inputCurrent - 1]){
                printf("[INPUT] Entrada coincide; desbloqueando processo.\n");

                removeElement(blockedQueue);
                addElement(priorityQueues[blockedProcess->priority], blockedProcess);
                return "Free block process";
            }else{
                printf("[INPUT] Entrada não corresponde ao processo bloqueado.\n");
                return "Not valid input";
            }
        }else{
            printf("[INPUT] Não há processos bloqueados.\n");
            return "Not valid input";
        }
    }
    // Caso 2: só dígitos -> "%d" puro
    int souNumero = 1;
    for (int i = 0; i < len; i++) {
        if (!isdigit(linhaLida[i])) { souNumero = 0; break; }
    }
    if (souNumero) {
        printf("[INPUT] Criando processo sem entrada adicional.\n");
        int valor;
        sscanf(linhaLida, "%llu", &valor);
        numberInput[count] = valor;
        charInput[count] = '\0'; // sem letra associada
        (count)++;
        Process* newProcess = initializeProcess(linhaLida, numberInput, charInput, count);
        addElement(priorityQueues[newProcess->priority], newProcess);
        printf("[INPUT] Processo adicionado à fila de prioridade %d.\n", newProcess->priority);
        return "Add process without input";
    }

    // Caso 3: pares número+letra repetindo -> "%d%c%d%c..."
    int pos = 0, offset;
    while (sscanf(linhaLida + pos, "%llu%c%n", &numberInput[count], &charInput[count], &offset) == 2) {
        pos += offset;
        (count)++;
    }

    Process* newProcess = initializeProcess(linhaLida, numberInput, charInput, count);
    addElement(priorityQueues[newProcess->priority], newProcess);
    printf("[INPUT] Processo com entrada adicionado à fila de prioridade %d.\n", newProcess->priority);
    return "Add process with input";
}

char *execProcess(Process* processExec, uint64_t time, char inputToContinue){
    clock_t inicioProcess, fimProcess;
    clock_t inicioVeri, fimVeri;

    printf("[EXEC] Processo iniciado: prioridade=%d, tempo=%llu.\n", processExec->priority, time);
    removeElement(priorityQueues[processExec->priority]);
    // printf("Process removed from Ready");

    runningProcess = processExec;
    processExec->state = RUNNING;

    // Processo sendo executado
    inicioProcess = clock();
    while(1){

        uint64_t relapsedTimeProcess = (uint64_t)( (clock() - inicioProcess) * 1000)/CLOCKS_PER_SEC;
        // printf("Tempo passado no processo: %llu", relapsedTimeProcess);
        // Verificação se...

        // Para caso não respeita a janela de quantum e..
        if(relapsedTimeProcess >= quantum){

            printf("[EXEC] Quantum excedido; processo será interrompido.\n");
            processExec->relapsedTime = relapsedTimeProcess;

            // Muda a prioriedade ou
            if(processExec->priority < 3){

                int newPriority = processExec->priority + 1;
                printf("[EXEC] Rebaixando processo para a prioridade %d.\n", newPriority);
                addElement(priorityQueues[newPriority], processExec);
                processExec->priority = newPriority;
            }
            return "Process was interrupted";
        // Bloqueia caso precise de uma entrada ou
        }else if(relapsedTimeProcess >= time && inputToContinue != 0){

            printf("[EXEC] Processo bloqueado aguardando entrada '%c'.\n", inputToContinue);
            changeThread(processExec);
            addElement(blockedQueue, processExec);
            processExec->state = BLOCKED;
            return "Process nedded of input to continue";
        // Completa caso finalize no tempo
        } else if(relapsedTimeProcess >= time){

            printf("[EXEC] Processo concluído com sucesso.\n");
            return "Successfully completed";
        // Continua caso não tenha terminado
        } else {
            continue;
        }
    }

}

int main() {
    initialize(STANDARD_QUANTUM);
    return 0;
}
