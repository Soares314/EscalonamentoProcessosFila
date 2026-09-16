# Simulador de Escalonamento de Processos

Simulador de um **escalonador de processos** desenvolvido em C, utilizando filas de prioridade para representar o gerenciamento de processos de um sistema operacional. O projeto tem como objetivo simular, de maneira simplificada, mecanismos envolvidos no gerenciamento de processos de um sistema operacional.

A aplicação lê processos a partir de um arquivo `input.txt`, coloca-os nas filas de prioridade correspondentes e simula sua execução considerando **quantum de processamento**, **bloqueio por entrada**, **desbloqueio** e **mudança de prioridade**.

## Funcionalidades

O simulador possui:

* n filas de prioridade;
* fila separada para processos bloqueados;
* escalonamento baseado em prioridade;
* execução dos processos utilizando quantum;
* rebaixamento de prioridade quando o processo excede seu quantum;
* bloqueio de processos que necessitam de uma entrada para continuar;
* desbloqueio de processos através de entradas no `input.txt`;
* leitura de múltiplos processos a partir de um arquivo;
* utilização de filas circulares para gerenciamento dos processos;
* acompanhamento do estado atual do processo.

O número de filas é determinado no header da aplicação, sendo testando com 4.

---

## Tecnologias

* **C**
* Estruturas de dados
* Filas circulares
* Gerenciamento de processos
* Escalonamento de processos
* Manipulação de arquivos
* Medição de tempo com `clock()`

---

## Funcionamento do escalonador

O programa começa inicializando as filas de prioridade e a fila de processos bloqueados. Em seguida, abre o arquivo `input.txt` e começa a processar suas entradas.

O fluxo geral é:

```text
                 ┌──────────────┐
                 │   input.txt  │
                 └──────┬───────┘
                        │
                        ▼
                ┌───────────────┐
                │   Ler entrada │
                └───────┬───────┘
                        │
                        ▼
                ┌───────────────┐
                │ Criar processo│
                └───────┬───────┘
                        │
                        ▼
          ┌───────────────────────────┐
          │ Fila de prioridade 0      │
          │ Fila de prioridade 1      │
          │ Fila de prioridade 2      │
          │ Fila de prioridade 3      │
          └─────────────┬─────────────┘
                        │
                        ▼
                ┌───────────────┐
                │    Executar   │
                └───────┬───────┘
                        │
          ┌─────────────┼──────────────┐
          │             │              │
          ▼             ▼              ▼
       Finaliza      Bloqueia       Quantum
          │             │           excedido
          │             │              │
          ▼             ▼              ▼
       Processo      Fila de       Rebaixar  
       concluído    bloqueados     prioridade
```

A função `manageProcesses()` percorre as filas começando pela prioridade `0` e executa o primeiro processo encontrado.

---

## Mudança de prioridade

Quando um processo utiliza todo o seu quantum sem terminar, ele pode ser colocado em uma fila de prioridade inferior.

A aplicação possui quatro níveis:

```text
Prioridade 0
    ↓
Prioridade 1
    ↓
Prioridade 2
    ↓
Prioridade 3
```

Se a prioridade atual for menor que `3`, ela é incrementada em uma unidade quando o quantum é excedido:

```c
int newPriority = processExec->priority + 1;
```

O processo então é inserido na nova fila de prioridade.

---

## Bloqueio de processos

Um processo pode possuir uma entrada necessária para continuar sua execução.

Quando o tempo de execução do processo é atingido e ainda existe uma entrada associada a ele, o processo é colocado na fila de bloqueados:

```text
Processo em execução
        │
        │ precisa de entrada
        ▼
Fila de processos bloqueados
```

Nesse momento, seu estado é alterado para `BLOCKED`.

---

## Desbloqueio

O arquivo `input.txt` também pode fornecer uma entrada destinada a desbloquear um processo.

Quando uma entrada de um único caractere é encontrada, o programa verifica se existe algum processo bloqueado e compara o caractere recebido com a entrada esperada pelo processo.

Se houver correspondência, o processo é retirado da fila de bloqueados e retornado à fila de prioridade correspondente.

O fluxo é:

```text
          Entrada
             │
             ▼
    ┌─────────────────┐
    │ Existe processo │
    │    bloqueado?   │
    └────────┬────────┘
             │
             ▼
    ┌─────────────────┐
    │ Entrada coincide│
    │ com a esperada? │
    └────────┬────────┘
             │
            SIM
             │
             ▼
       Desbloquear
             │
             ▼
    Fila de prioridade
```

---

## Formato do `input.txt`

O programa aceita três formatos principais de entrada.

### 1. Número isolado

Uma linha contendo somente números representa um processo sem entrada adicional.

Exemplo:

```text
500
```

O processo é criado e inserido na fila correspondente à sua prioridade.

---

### 2. Número seguido de caractere

Também é possível fornecer pares de **número + caractere**:

```text
100a
200b
300c
```

Nesse formato, o número representa o tempo associado à entrada e o caractere representa a entrada necessária para o processo continuar.

O programa também permite vários pares na mesma linha:

```text
100a200b300c
```

Esses pares são processados sequencialmente.

---

### 3. Caractere isolado

Um caractere isolado é interpretado como uma tentativa de desbloquear um processo:

```text
a
```

Se o caractere corresponder à entrada esperada pelo processo bloqueado, ele retorna para sua fila de prioridade.

---

## Exemplo conceitual

Um `input.txt` poderia conter:

```text
500
200a
100
a
```

O simulador interpretaria as entradas de acordo com seus formatos:

1. cria um processo com tempo `500`;
2. cria um processo associado à entrada `a`;
3. cria outro processo com tempo `100`;
4. utiliza `a` como tentativa de desbloqueio de um processo bloqueado.

---

## Estados dos processos

O escalonador trabalha com diferentes estados de processo, incluindo:

```text
READY
  │
  ▼
RUNNING
  │
  ├──────────────► BLOCKED
  │                    │
  │                    │ entrada recebida
  │                    ▼
  └───────────────► READY
  │
  ▼
Concluído
```

O processo selecionado para execução tem seu estado alterado para `RUNNING`.

Quando precisa aguardar uma entrada, passa para `BLOCKED`.

---

## Estrutura das filas

Cada fila possui um limite de 100 processos:

```c
#define MAX_SIZE 100
```

e existem quatro filas de prioridade:

```c
#define NUMBER_PRIORITIES 4
```

A estrutura utilizada é:

```c
typedef struct {
    Process* processes[MAX_SIZE];
    int front;
    int rear;
} QueueProcess;
```

O processo sempre é obtido a partir da frente (`front`) da fila.

---

## Compilação

Considerando que todos os arquivos necessários estão no mesmo diretório, o programa pode ser compilado com:

```bash
gcc EscalonamentoProcesso.c -o escalonador
```

Depois:

```bash
./escalonador
```

No Windows, caso esteja utilizando MinGW:

```bash
gcc EscalonamentoProcesso.c -o escalonador.exe
```

e:

```bash
escalonador.exe
```

---

## Saída

Durante a execução, o programa apresenta mensagens de acompanhamento indicando as principais operações realizadas pelo escalonador.

Exemplos:

```text
[INIT] Inicializando escalonador...
[INIT] Filas de prioridade inicializadas.
[INIT] Fila de bloqueados inicializada.
```

Ao terminar o processamento do arquivo, as filas de prioridade e a fila de bloqueados são impressas para indicar se há processos não finalizados.

---

## Autor

Thiago Soares, desenvolvido para fins acadêmicos dentro da matéria de Sistemas Operacionais 2026/2 do curso de Ciência da Computação na UFT, campus Palmas.

