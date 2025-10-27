/**
 * Simulação do Gerenciamento de Peças do jogo Tetris Stack.
 * Nível Intermediário: Fila + Pilha.
 *
 * Este programa implementa:
 * 1. Uma Fila Circular: para as peças futuras (sempre cheia).
 * 2. Uma Pilha Linear: para as peças reservadas (capacidade limitada).
 *
 * O jogador pode "Jogar" (dequeue da fila), "Reservar" (dequeue da
 * fila e push para a pilha) ou "Usar" (pop da pilha).
 */

#include <stdio.h>
#include <stdlib.h> // Para rand(), srand()
#include <time.h>   // Para time()

// --- Definições Globais e Estruturas ---

// Define os tamanhos máximos das estruturas
#define TAM_FILA 5
#define TAM_PILHA 3

/**
 * @brief Contador global para garantir que cada peça tenha um ID único.
 */
int proximoId = 0;

/**
 * @brief Estrutura que representa uma peça do jogo.
 */
typedef struct {
    char nome; // Tipo da peça (I, O, T, L)
    int id;    // Identificador único da peça
} Peca;

/**
 * @brief Estrutura da Fila Circular (FIFO - First-In, First-Out).
 */
typedef struct {
    Peca pecas[TAM_FILA];
    int frente;     // Índice do primeiro elemento (para remoção)
    int tras;       // Índice da próxima posição livre (para inserção)
    int quantidade; // Número atual de elementos
} Fila;

/**
 * @brief Estrutura da Pilha Linear (LIFO - Last-In, First-Out).
 */
typedef struct {
    Peca pecas[TAM_PILHA];
    int topo; // Índice do elemento no topo (-1 se vazia)
} Pilha;

// --- Protótipos das Funções (Assinaturas) ---

// Funções de Gerenciamento da Fila
void inicializarFila(Fila *f);
int filaEstaVazia(Fila *f);
int filaEstaCheia(Fila *f);
int enqueue(Fila *f, Peca p);
int dequeue(Fila *f, Peca *pRemovida);
void exibirFila(Fila *f);

// Funções de Gerenciamento da Pilha
void inicializarPilha(Pilha *p);
int pilhaEstaVazia(Pilha *p);
int pilhaEstaCheia(Pilha *p);
int push(Pilha *p, Peca peca);
int pop(Pilha *p, Peca *pRemovida);
void exibirPilha(Pilha *p);

// Funções Auxiliares e de Interface
Peca gerarPeca();
void exibirEstadoAtual(Fila *f, Pilha *p);
void exibirMenu();
void limparBufferEntrada();

// --- Função Principal (main) ---

int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    Fila filaDePecas;
    Pilha pilhaDeReserva;

    // Prepara as estruturas
    inicializarFila(&filaDePecas);
    inicializarPilha(&pilhaDeReserva);

    // Requisito: Inicializar a fila com um número fixo de elementos
    printf("Inicializando o jogo...\n");
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&filaDePecas, gerarPeca());
    }

    int opcao;
    do {
        // 1. Exibir o estado atual
        exibirEstadoAtual(&filaDePecas, &pilhaDeReserva);

        // 2. Exibir o menu
        exibirMenu();

        // 3. Ler a ação do usuário
        printf("Opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada(); // Garante limpeza do buffer de entrada

        Peca pecaTemp; // Peça temporária para operações

        // 4. Processar a ação
        switch (opcao) {
            case 1: // Jogar peça
                if (dequeue(&filaDePecas, &pecaTemp)) {
                    printf("\n-> Ação: Peça jogada: [%c %d]\n", pecaTemp.nome, pecaTemp.id);
                    
                    // Requisito: Adiciona nova peça para manter a fila cheia
                    enqueue(&filaDePecas, gerarPeca());
                } else {
                    // Isso não deve acontecer se a lógica de refil estiver correta
                    printf("\n-> Ação: Fila de peças vazia!\n");
                }
                break;

            case 2: // Reservar peça
                if (pilhaEstaCheia(&pilhaDeReserva)) {
                    printf("\n-> Ação: Pilha de reserva está cheia! Use uma peça antes.\n");
                } else {
                    // 1. Tira da fila
                    if (dequeue(&filaDePecas, &pecaTemp)) {
                        // 2. Coloca na pilha
                        push(&pilhaDeReserva, pecaTemp);
                        printf("\n-> Ação: Peça [%c %d] movida para a reserva.\n", pecaTemp.nome, pecaTemp.id);
                        
                        // 3. Requisito: Adiciona nova peça na fila
                        enqueue(&filaDePecas, gerarPeca());
                    } else {
                        printf("\n-> Ação: Fila de peças vazia!\n");
                    }
                }
                break;

            case 3: // Usar peça reservada
                if (pop(&pilhaDeReserva, &pecaTemp)) {
                    printf("\n-> Ação: Peça [%c %d] usada da reserva.\n", pecaTemp.nome, pecaTemp.id);
                    // Nota: A fila NÃO é reabastecida aqui, pois a peça não veio dela.
                } else {
                    printf("\n-> Ação: Pilha de reserva vazia!\n");
                }
                break;

            case 0: // Sair
                printf("\nSaindo do Tetris Stack...\n");
                break;

            default:
                printf("\nOpção inválida! Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}

// --- Implementação das Funções ---

// --- Funções da Fila (FIFO) ---

void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = 0;
    f->quantidade = 0;
}

int filaEstaVazia(Fila *f) {
    return (f->quantidade == 0);
}

int filaEstaCheia(Fila *f) {
    return (f->quantidade == TAM_FILA);
}

int enqueue(Fila *f, Peca p) {
    if (filaEstaCheia(f)) return 0; // Falha
    f->pecas[f->tras] = p;
    f->tras = (f->tras + 1) % TAM_FILA; // Lógica circular
    f->quantidade++;
    return 1; // Sucesso
}

int dequeue(Fila *f, Peca *pRemovida) {
    if (filaEstaVazia(f)) return 0; // Falha
    *pRemovida = f->pecas[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA; // Lógica circular
    f->quantidade--;
    return 1; // Sucesso
}

void exibirFila(Fila *f) {
    printf("Fila de peças:\t");
    if (filaEstaVazia(f)) {
        printf("[ VAZIA ]\n");
        return;
    }

    // Itera da 'frente' até a 'tras' de forma circular
    int i = f->frente;
    for (int count = 0; count < f->quantidade; count++) {
        Peca p = f->pecas[i];
        printf("[%c %d] ", p.nome, p.id);
        i = (i + 1) % TAM_FILA;
    }
    printf("\n");
}

// --- Funções da Pilha (LIFO) ---

/**
 * @brief Inicializa a pilha, definindo o topo como -1 (vazia).
 */
void inicializarPilha(Pilha *p) {
    p->topo = -1; // -1 indica que a pilha está vazia
}

/**
 * @brief Verifica se a pilha está vazia.
 */
int pilhaEstaVazia(Pilha *p) {
    return (p->topo == -1);
}

/**
 * @brief Verifica se a pilha está cheia.
 */
int pilhaEstaCheia(Pilha *p) {
    // O topo é um índice (base 0), então o máximo é TAM_PILHA - 1
    return (p->topo == TAM_PILHA - 1);
}

/**
 * @brief Adiciona um item ao topo da pilha (Push).
 * @return 1 se sucesso, 0 se a pilha estava cheia.
 */
int push(Pilha *p, Peca peca) {
    if (pilhaEstaCheia(p)) {
        return 0; // Falha
    }
    // 1. Incrementa o topo
    p->topo++;
    // 2. Adiciona o item na nova posição do topo
    p->pecas[p->topo] = peca;
    return 1; // Sucesso
}

/**
 * @brief Remove um item do topo da pilha (Pop).
 * @param pRemovida Ponteiro de saída para a peça removida.
 * @return 1 se sucesso, 0 se a pilha estava vazia.
 */
int pop(Pilha *p, Peca *pRemovida) {
    if (pilhaEstaVazia(p)) {
        return 0; // Falha
    }
    // 1. Obtém o item do topo
    *pRemovida = p->pecas[p->topo];
    // 2. Decrementa o topo
    p->topo--;
    return 1; // Sucesso
}

/**
 * @brief Exibe o estado da pilha, do topo para a base.
 */
void exibirPilha(Pilha *p) {
    printf("Pilha de reserva\t(Topo -> Base): ");
    if (pilhaEstaVazia(p)) {
        printf("[ VAZIA ]\n");
        return;
    }

    // Itera do topo (último a entrar) para a base (índice 0)
    for (int i = p->topo; i >= 0; i--) {
        Peca peca = p->pecas[i];
        printf("[%c %d] ", peca.nome, peca.id);
    }
    printf("\n");
}

// --- Funções Auxiliares ---

/**
 * @brief Gera uma nova peça aleatória com um ID único.
 */
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    int indiceTipo = rand() % 4; // Sorteia um índice de 0 a 3

    Peca novaPeca;
    novaPeca.nome = tipos[indiceTipo];
    novaPeca.id = proximoId; // Atribui o ID único global
    proximoId++;           // Incrementa o contador para a próxima

    return novaPeca;
}

/**
 * @brief Exibe o estado consolidado (Fila e Pilha).
 */
void exibirEstadoAtual(Fila *f, Pilha *p) {
    printf("\n-------------------------------------------------\n");
    printf("Estado atual:\n\n");
    exibirFila(f);
    exibirPilha(p);
    printf("-------------------------------------------------\n");
}

/**
 * @brief Exibe o menu de ações para o jogador.
 */
void exibirMenu() {
    printf("\nOpções de Ação:\n");
    printf("Código  Ação\n");
    printf("  1     Jogar peça (da Fila)\n");
    printf("  2     Reservar peça (Fila -> Pilha)\n");
    printf("  3     Usar peça reservada (da Pilha)\n");
    printf("  0     Sair\n");
}

/**
 * @brief Limpa o buffer de entrada (stdin) para evitar
 * leituras incorretas em loops (especialmente após scanf).
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}