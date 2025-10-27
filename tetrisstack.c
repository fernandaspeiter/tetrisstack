/**
 * Simulação da Fila de Peças Futuras do jogo Tetris Stack.
 *
 * Este programa implementa uma fila circular em C para gerenciar as peças
 * (Struct Peca) que aguardam para entrar no jogo.
 *
 * Funcionalidades:
 * 1. Inicializa a fila com um número fixo de peças.
 * 2. Permite ao usuário "Jogar" uma peça (dequeue).
 * 3. Permite ao usuário "Inserir" uma nova peça (enqueue), se houver espaço.
 * 4. Exibe o estado da fila após cada operação.
 */

#include <stdio.h>
#include <stdlib.h> // Para rand(), srand()
#include <time.h>   // Para time()

// --- Definições Globais e Estruturas ---

// Define o tamanho máximo da fila de peças futuras
#define TAM_FILA 5

/**
 * @brief Contador global para garantir que cada peça tenha um ID único.
 * Inicia em 0.
 */
int proximoId = 0;

/**
 * @brief Estrutura que representa uma peça do jogo.
 * Contém um 'nome' (tipo) e um 'id' único.
 */
typedef struct {
    char nome; // Tipo da peça (I, O, T, L)
    int id;    // Identificador único da peça
} Peca;

/**
 * @brief Estrutura da Fila Circular.
 * Armazena as peças em um array e controla o fluxo usando
 * índices de 'frente', 'tras' e um contador de 'quantidade'.
 */
typedef struct {
    Peca pecas[TAM_FILA]; // Array fixo para armazenar as peças
    int frente;          // Índice do primeiro elemento (para remoção)
    int tras;            // Índice da próxima posição livre (para inserção)
    int quantidade;      // Número atual de elementos na fila
} Fila;

// --- Protótipos das Funções (Assinaturas) ---

// Funções de Gerenciamento da Fila
void inicializarFila(Fila *f);
int filaEstaVazia(Fila *f);
int filaEstaCheia(Fila *f);
int enqueue(Fila *f, Peca p);
int dequeue(Fila *f, Peca *pRemovida);

// Funções Auxiliares e de Interface
Peca gerarPeca();
void exibirFila(Fila *f);
void exibirMenu();
void limparBufferEntrada();

// --- Função Principal (main) ---

int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    Fila filaDePecas;
    inicializarFila(&filaDePecas);

    // Requisito: Inicializar a fila com um número fixo de elementos (5)
    printf("Inicializando a fila com %d peças...\n", TAM_FILA);
    for (int i = 0; i < TAM_FILA; i++) {
        // Gera e insere (enqueue) as peças iniciais
        enqueue(&filaDePecas, gerarPeca());
    }

    int opcao;
    do {
        // 1. Exibir o estado atual
        printf("\n-------------------------------------\n");
        printf("Confira a seguir seu estado:\n\n");
        exibirFila(&filaDePecas);

        // 2. Exibir o menu
        exibirMenu();

        // 3. Ler a ação do usuário
        printf("Escolha sua ação: ");
        scanf("%d", &opcao);
        limparBufferEntrada(); // Evita loops infinitos se o usuário digitar letras

        // 4. Processar a ação
        switch (opcao) {
            case 1: { // Jogar peça (dequeue)
                Peca pecaJogada;
                // Tenta remover a peça da frente
                if (dequeue(&filaDePecas, &pecaJogada)) {
                    printf("\n-> Ação: Peça jogada: [%c %d]\n", pecaJogada.nome, pecaJogada.id);
                } else {
                    printf("\n-> Ação: Fila está vazia! Nenhuma peça para jogar.\n");
                }
                break;
            }
            case 2: { // Inserir nova peça (enqueue)
                // Verifica se a fila não está cheia
                if (filaEstaCheia(&filaDePecas)) {
                    printf("\n-> Ação: Fila está cheia! Jogue uma peça antes de adicionar.\n");
                } else {
                    // Gera e insere a nova peça
                    Peca novaPeca = gerarPeca();
                    enqueue(&filaDePecas, novaPeca);
                    printf("\n-> Ação: Nova peça inserida: [%c %d]\n", novaPeca.nome, novaPeca.id);
                }
                break;
            }
            case 0: // Sair
                printf("\nSaindo do Tetris Stack...\n");
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    return 0; // Indica que o programa terminou com sucesso
}

// --- Implementação das Funções ---

/**
 * @brief Inicializa a fila, zerando os indicadores.
 * @param f Ponteiro para a fila a ser inicializada.
 */
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = 0;
    f->quantidade = 0;
}

/**
 * @brief Verifica se a fila está vazia.
 * @param f Ponteiro para a fila.
 * @return 1 (true) se a quantidade for 0, 0 (false) caso contrário.
 */
int filaEstaVazia(Fila *f) {
    return (f->quantidade == 0);
}

/**
 * @brief Verifica se a fila está cheia.
 * @param f Ponteiro para a fila.
 * @return 1 (true) se a quantidade for igual ao tamanho máximo, 0 (false) caso contrário.
 */
int filaEstaCheia(Fila *f) {
    return (f->quantidade == TAM_FILA);
}

/**
 * @brief Gera uma nova peça aleatória com um ID único.
 * Sorteia um dos 4 tipos de peça ('I', 'O', 'T', 'L') e
 * atribui o próximo ID sequencial.
 * @return A nova Peca gerada.
 */
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'}; // Tipos de peças disponíveis
    int indiceTipo = rand() % 4;         // Sorteia um índice de 0 a 3

    Peca novaPeca;
    novaPeca.nome = tipos[indiceTipo]; // Define o tipo
    novaPeca.id = proximoId;           // Atribui o ID único global

    proximoId++; // Incrementa o contador de ID para a próxima peça

    return novaPeca;
}

/**
 * @brief Adiciona uma peça ao final da fila (enqueue).
 * A inserção ocorre na posição 'tras'. O índice 'tras' é
 * então atualizado de forma circular.
 * @param f Ponteiro para a fila.
 * @param p Peca a ser adicionada.
 * @return 1 (true) se sucesso, 0 (false) se a fila estava cheia.
 */
int enqueue(Fila *f, Peca p) {
    if (filaEstaCheia(f)) {
        return 0; // Falha, fila cheia
    }

    // 1. Adiciona a peça na posição 'tras' (próxima posição livre)
    f->pecas[f->tras] = p;

    // 2. Atualiza o índice 'tras' de forma circular
    // (Ex: se TAM_FILA=5 e tras=4, (4+1)%5 = 0 -> volta ao início)
    f->tras = (f->tras + 1) % TAM_FILA;

    // 3. Incrementa a quantidade de elementos
    f->quantidade++;

    return 1; // Sucesso
}

/**
 * @brief Remove uma peça do início da fila (dequeue).
 * A remoção ocorre da posição 'frente'. O índice 'frente' é
 * então atualizado de forma circular.
 * @param f Ponteiro para a fila.
 * @param pRemovida Ponteiro (saída) para onde a peça removida será armazenada.
 * @return 1 (true) se sucesso, 0 (false) se a fila estava vazia.
 */
int dequeue(Fila *f, Peca *pRemovida) {
    if (filaEstaVazia(f)) {
        return 0; // Falha, fila vazia
    }

    // 1. Obtém a peça da 'frente' (posição do primeiro elemento)
    *pRemovida = f->pecas[f->frente];

    // 2. Atualiza o índice 'frente' de forma circular
    f->frente = (f->frente + 1) % TAM_FILA;

    // 3. Decrementa a quantidade de elementos
    f->quantidade--;

    return 1; // Sucesso
}

/**
 * @brief Exibe o estado atual da fila no console, da frente para trás.
 * @param f Ponteiro para a fila.
 */
void exibirFila(Fila *f) {
    if (filaEstaVazia(f)) {
        printf("Fila de peças: [ VAZIA ]\n");
        return;
    }

    printf("Fila de peças: ");

    // Para exibir na ordem correta, começamos em 'frente'
    // e iteramos 'quantidade' vezes
    int i = f->frente;
    for (int count = 0; count < f->quantidade; count++) {
        
        Peca p = f->pecas[i];
        printf("[%c %d] ", p.nome, p.id);

        // Move o índice 'i' circularmente para o próximo elemento
        i = (i + 1) % TAM_FILA;
    }
    printf("\n");
}

/**
 * @brief Exibe o menu de ações para o jogador.
 */
void exibirMenu() {
    printf("\nOpções de ação:\n");
    printf("Código  Ação\n");
    printf("  1     Jogar peça (dequeue)\n");
    printf("  2     Inserir nova peça (enqueue)\n");
    printf("  0     Sair\n");
}

/**
 * @brief Limpa o buffer de entrada (stdin).
 * Essencial após um scanf() para evitar que um '\n' ou caracteres
 * inválidos causem loops infinitos.
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}