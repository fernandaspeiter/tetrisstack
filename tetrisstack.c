/**
 * Simulação do Gerenciador de Peças do jogo Tetris Stack.
 * Nível Avançado: Fila Circular + Pilha Linear com Trocas Estratégicas.
 *
 * Este programa implementa:
 * 1. Uma Fila Circular (capacidade 5), mantida sempre cheia.
 * 2. Uma Pilha Linear (capacidade 3).
 *
 * Ações:
 * 1. Jogar (Dequeue da Fila + Refil)
 * 2. Reservar (Dequeue da Fila -> Push para Pilha + Refil)
 * 3. Usar (Pop da Pilha)
 * 4. Trocar Atual (Swap Fila[frente] <-> Pilha[topo])
 * 5. Troca Múltipla (Swap 3 da Fila <-> 3 da Pilha)
 */

#include <stdio.h>
#include <stdlib.h> // Para rand(), srand()
#include <time.h>   // Para time()

// --- Definições Globais e Estruturas ---

#define TAM_FILA 5
#define TAM_PILHA 3

/**
 * @brief Contador global para garantir IDs únicos.
 */
int proximoId = 0;

/**
 * @brief Estrutura da Peça.
 */
typedef struct {
    char nome; // I, O, T, L
    int id;
} Peca;

/**
 * @brief Estrutura da Fila Circular (FIFO).
 */
typedef struct {
    Peca pecas[TAM_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

/**
 * @brief Estrutura da Pilha Linear (LIFO).
 */
typedef struct {
    Peca pecas[TAM_PILHA];
    int topo; // -1 = vazia, 0 = 1 item, ..., 2 = cheia
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

// Funções de Ações Estratégicas
void trocarPecaAtual(Fila *f, Pilha *p);
void trocarMultiplasPecas(Fila *f, Pilha *p);

// Funções Auxiliares e de Interface
Peca gerarPeca();
void exibirEstadoAtual(Fila *f, Pilha *p);
void exibirMenu();
void limparBufferEntrada();

// --- Função Principal (main) ---

int main() {
    srand(time(NULL)); // Inicializa o gerador aleatório

    Fila filaDePecas;
    Pilha pilhaDeReserva;

    inicializarFila(&filaDePecas);
    inicializarPilha(&pilhaDeReserva);

    // Requisito: Inicializar a fila com 5 peças
    printf("Inicializando o Tetris Stack (Nível Avançado)...\n");
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&filaDePecas, gerarPeca());
    }

    int opcao;
    do {
        // 1. Exibir estado
        exibirEstadoAtual(&filaDePecas, &pilhaDeReserva);

        // 2. Exibir menu
        exibirMenu();

        // 3. Ler ação
        printf("Opção escolhida: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        Peca pecaTemp; // Peça temporária para operações

        // 4. Processar ação
        switch (opcao) {
            case 1: // Jogar peça (da fila)
                if (dequeue(&filaDePecas, &pecaTemp)) {
                    printf("\n-> Ação: Peça jogada: [%c %d].\n", pecaTemp.nome, pecaTemp.id);
                    // Requisito: Reabastece a fila
                    enqueue(&filaDePecas, gerarPeca());
                    printf("-> Ação: Nova peça gerada para a fila.\n");
                } else {
                    printf("\n-> Erro: Fila de peças vazia!\n");
                }
                break;

            case 2: // Enviar peça da fila para a pilha de reserva
                if (pilhaEstaCheia(&pilhaDeReserva)) {
                    printf("\n-> Erro: Pilha de reserva está cheia!\n");
                } else if (dequeue(&filaDePecas, &pecaTemp)) {
                    push(&pilhaDeReserva, pecaTemp);
                    printf("\n-> Ação: Peça [%c %d] movida para a reserva.\n", pecaTemp.nome, pecaTemp.id);
                    // Requisito: Reabastece a fila
                    enqueue(&filaDePecas, gerarPeca());
                    printf("-> Ação: Nova peça gerada para a fila.\n");
                } else {
                    printf("\n-> Erro: Fila de peças vazia!\n");
                }
                break;

            case 3: // Usar peça da pilha de reserva
                if (pop(&pilhaDeReserva, &pecaTemp)) {
                    printf("\n-> Ação: Peça [%c %d] usada da reserva.\n", pecaTemp.nome, pecaTemp.id);
                    // A fila não é reabastecida aqui, pois a peça não saiu dela.
                } else {
                    printf("\n-> Erro: Pilha de reserva vazia!\n");
                }
                break;

            case 4: // Trocar peça da frente da fila com o topo da pilha
                trocarPecaAtual(&filaDePecas, &pilhaDeReserva);
                break;

            case 5: // Trocar os 3 primeiros da fila com as 3 peças da pilha
                trocarMultiplasPecas(&filaDePecas, &pilhaDeReserva);
                break;

            case 0: // Sair
                printf("\nEncerrando o gerenciador Tetris Stack...\n");
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
    if (filaEstaCheia(f)) return 0;
    f->pecas[f->tras] = p;
    f->tras = (f->tras + 1) % TAM_FILA; // Lógica circular
    f->quantidade++;
    return 1;
}

int dequeue(Fila *f, Peca *pRemovida) {
    if (filaEstaVazia(f)) return 0;
    *pRemovida = f->pecas[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA; // Lógica circular
    f->quantidade--;
    return 1;
}

// --- Funções da Pilha (LIFO) ---

void inicializarPilha(Pilha *p) {
    p->topo = -1; // -1 indica pilha vazia
}

int pilhaEstaVazia(Pilha *p) {
    return (p->topo == -1);
}

int pilhaEstaCheia(Pilha *p) {
    // Índice 2 (0, 1, 2) é o topo quando cheia
    return (p->topo == TAM_PILHA - 1);
}

int push(Pilha *p, Peca peca) {
    if (pilhaEstaCheia(p)) return 0;
    p->topo++;
    p->pecas[p->topo] = peca;
    return 1;
}

int pop(Pilha *p, Peca *pRemovida) {
    if (pilhaEstaVazia(p)) return 0;
    *pRemovida = p->pecas[p->topo];
    p->topo--;
    return 1;
}

// --- Funções de Ações Estratégicas ---

/**
 * @brief (Ação 4) Troca a peça da frente da fila com o topo da pilha.
 * Realiza uma troca direta (swap) dos dados nas posições.
 */
void trocarPecaAtual(Fila *f, Pilha *p) {
    // Validação: Ambas as estruturas devem ter pelo menos uma peça.
    if (filaEstaVazia(f) || pilhaEstaVazia(p)) {
        printf("\n-> Erro: Para trocar, a Fila e a Pilha devem conter peças.\n");
        return;
    }

    // Armazena a peça da fila temporariamente
    Peca temp = f->pecas[f->frente];

    // Move a peça do topo da pilha para a frente da fila
    f->pecas[f->frente] = p->pecas[p->topo];

    // Move a peça temporária (antiga da fila) para o topo da pilha
    p->pecas[p->topo] = temp;

    printf("\n-> Ação: Peça da frente da fila trocada com o topo da pilha.\n");
}

/**
 * @brief (Ação 5) Troca as 3 primeiras peças lógicas da fila
 * com as 3 peças da pilha (base-a-base, topo-a-topo).
 */
void trocarMultiplasPecas(Fila *f, Pilha *p) {
    // Validação: Ambas devem ter 3 peças.
    // Pilha: topo = 2 (índices 0, 1, 2)
    // Fila: quantidade >= 3
    if (f->quantidade < 3 || p->topo < 2) {
        printf("\n-> Erro: Para troca múltipla, a Fila e a Pilha devem ter 3 peças.\n");
        return;
    }

    Peca temp;
    int indiceFila;

    // A lógica (baseada no exemplo) é:
    // Fila[frente+0] <-> Pilha[base] (índice 0)
    // Fila[frente+1] <-> Pilha[meio] (índice 1)
    // Fila[frente+2] <-> Pilha[topo] (índice 2)
    for (int i = 0; i < 3; i++) {
        // Calcula o índice circular correto para a fila
        indiceFila = (f->frente + i) % TAM_FILA;
        
        // O índice da pilha (p->pecas[i]) já corresponde à lógica (0, 1, 2)

        // Realiza o swap
        temp = f->pecas[indiceFila];
        f->pecas[indiceFila] = p->pecas[i];
        p->pecas[i] = temp;
    }

    printf("\n-> Ação: troca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
}


// --- Funções Auxiliares e de Interface ---

/**
 * @brief Gera uma nova peça aleatória com um ID único.
 */
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    int indiceTipo = rand() % 4;

    Peca novaPeca;
    novaPeca.nome = tipos[indiceTipo];
    novaPeca.id = proximoId;
    proximoId++;

    return novaPeca;
}

/**
 * @brief Exibe o estado consolidado (Fila e Pilha).
 */
void exibirEstadoAtual(Fila *f, Pilha *p) {
    printf("\n-------------------------------------------------\n");
    printf("Estado atual:\n\n");

    // Exibe Fila
    printf("Fila de peças:\t\t");
    if (filaEstaVazia(f)) {
        printf("[ VAZIA ]\n");
    } else {
        int i = f->frente;
        for (int count = 0; count < f->quantidade; count++) {
            Peca peca = f->pecas[i];
            printf("[%c %d] ", peca.nome, peca.id);
            i = (i + 1) % TAM_FILA;
        }
        printf("\n");
    }

    // Exibe Pilha
    printf("Pilha de reserva\t(Topo -> Base): ");
    if (pilhaEstaVazia(p)) {
        printf("[ VAZIA ]\n");
    } else {
        // Itera do topo (p->topo) para a base (0)
        for (int i = p->topo; i >= 0; i--) {
            Peca peca = p->pecas[i];
            printf("[%c %d] ", peca.nome, peca.id);
        }
        printf("\n");
    }
    printf("-------------------------------------------------\n");
}

/**
 * @brief Exibe o menu de ações para o jogador.
 */
void exibirMenu() {
    printf("\nOpções disponíveis:\n");
    printf("Código  Ação\n");
    printf("  1     Jogar peça da frente da fila\n");
    printf("  2     Enviar peça da fila para a pilha de reserva\n");
    printf("  3     Usar peça da pilha de reserva\n");
    printf("  4     Trocar peça da frente da fila com o topo da pilha\n");
    printf("  5     Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
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