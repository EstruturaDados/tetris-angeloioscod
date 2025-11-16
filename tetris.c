#include <stdio.h>
#include <stdlib.h>

// ============================================================================
//                           PROJETO Tetris Stack 
// ============================================================================
// Matrícula: 20250212488
// Faculdade: Estácio
// Aluno: Ângelo Oliveira dos Santos
// Cuso: Análise e desenvolvimento de sistemas
// Desafio: nível novato
// Tema 3 - Integração de Fila e Pilha
// ============================================================================

// Constantes para definir o tamanho da fila
#define TAMANHO_FILA 5
#define NUM_TIPOS_PECAS 4

// Estrutura que representa uma peça do jogo
typedef struct {
    char nome;  // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;     // Identificador único da peça
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca pecas[TAMANHO_FILA];  // Array para armazenar as peças
    int frente;                // Índice da frente da fila
    int tras;                  // Índice do final da fila
    int tamanho;               // Número atual de elementos na fila
    int contador_id;           // Contador para gerar IDs únicos
} FilaPecas;

// Protótipos das funções
void inicializarFila(FilaPecas *fila);
Peca gerarPeca(FilaPecas *fila);
int enqueue(FilaPecas *fila, Peca peca);
Peca dequeue(FilaPecas *fila);
int filaCheia(FilaPecas *fila);
int filaVazia(FilaPecas *fila);
void exibirFila(FilaPecas *fila);
void exibirMenu();

int main() {
    FilaPecas fila;
    int opcao;
    
    // Inicializa a fila com peças
    inicializarFila(&fila);
    
    printf("=== Tetris Stack - Fila de Pecas Futuras ===\n\n");
    
    do {
        // Exibe o estado atual da fila
        printf("Estado atual da fila:\n");
        exibirFila(&fila);
        
        // Exibe o menu de opções
        exibirMenu();
        
        // Lê a opção do usuário
        printf("\nDigite sua opcao: ");
        scanf("%d", &opcao);
        
        // Processa a opção escolhida
        switch(opcao) {
            case 0:
                printf("Saindo do jogo...\n");
                break;
                
            case 1:
                // Jogar peça (dequeue)
                if (!filaVazia(&fila)) {
                    Peca pecaJogada = dequeue(&fila);
                    printf("Peca jogada: [%c %d]\n", pecaJogada.nome, pecaJogada.id);
                } else {
                    printf("Erro: Nao ha pecas para jogar!\n");
                }
                break;
                
            case 2:
                // Inserir nova peça (enqueue)
                if (!filaCheia(&fila)) {
                    Peca novaPeca = gerarPeca(&fila);
                    if (enqueue(&fila, novaPeca)) {
                        printf("Nova peca adicionada: [%c %d]\n", novaPeca.nome, novaPeca.id);
                    } else {
                        printf("Erro: Nao foi possivel adicionar a peca!\n");
                    }
                } else {
                    printf("Erro: Fila cheia! Jogue uma peca antes de adicionar nova.\n");
                }
                break;
                
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
        
        printf("\n");
        
    } while (opcao != 0);
    
    return 0;
}

/**
 * Inicializa a fila com peças geradas automaticamente
 */
void inicializarFila(FilaPecas *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
    fila->contador_id = 0;
    
    // Preenche a fila inicial com peças
    for (int i = 0; i < TAMANHO_FILA; i++) {
        Peca novaPeca = gerarPeca(fila);
        enqueue(fila, novaPeca);
    }
}

/**
 * Gera uma nova peça com tipo aleatório e ID único
 */
Peca gerarPeca(FilaPecas *fila) {
    Peca novaPeca;
    
    // Tipos de peças disponíveis
    char tipos[NUM_TIPOS_PECAS] = {'I', 'O', 'T', 'L'};
    
    // Seleciona um tipo aleatório
    int indiceTipo = rand() % NUM_TIPOS_PECAS;
    novaPeca.nome = tipos[indiceTipo];
    
    // Atribui um ID único e incrementa o contador
    novaPeca.id = fila->contador_id++;
    
    return novaPeca;
}

/**
 * Adiciona uma peça ao final da fila (enqueue)
 * Retorna 1 em caso de sucesso, 0 em caso de falha
 */
int enqueue(FilaPecas *fila, Peca peca) {
    if (filaCheia(fila)) {
        return 0; // Fila cheia
    }
    
    // Avança o índice do final (com wrap-around)
    fila->tras = (fila->tras + 1) % TAMANHO_FILA;
    fila->pecas[fila->tras] = peca;
    fila->tamanho++;
    
    return 1; // Sucesso
}

/**
 * Remove e retorna a peça da frente da fila (dequeue)
 */
Peca dequeue(FilaPecas *fila) {
    Peca pecaVazia = {' ', -1}; // Peça vazia para retorno em caso de erro
    
    if (filaVazia(fila)) {
        return pecaVazia;
    }
    
    // Obtém a peça da frente
    Peca peca = fila->pecas[fila->frente];
    
    // Avança o índice da frente (com wrap-around)
    fila->frente = (fila->frente + 1) % TAMANHO_FILA;
    fila->tamanho--;
    
    return peca;
}

/**
 * Verifica se a fila está cheia
 */
int filaCheia(FilaPecas *fila) {
    return fila->tamanho == TAMANHO_FILA;
}

/**
 * Verifica se a fila está vazia
 */
int filaVazia(FilaPecas *fila) {
    return fila->tamanho == 0;
}

/**
 * Exibe o estado atual da fila
 */
void exibirFila(FilaPecas *fila) {
    if (filaVazia(fila)) {
        printf("Fila vazia!\n");
        return;
    }
    
    // Percorre a fila a partir da frente
    int indice = fila->frente;
    for (int i = 0; i < fila->tamanho; i++) {
        printf("[%c %d] ", fila->pecas[indice].nome, fila->pecas[indice].id);
        indice = (indice + 1) % TAMANHO_FILA;
    }
    printf("\n");
}

/**
 * Exibe o menu de opções disponíveis
 */
void exibirMenu() {
    printf("\nOpcoes disponiveis:\n");
    printf("1 - Jogar peca (dequeue)\n");
    printf("2 - Inserir nova peca (enqueue)\n");
    printf("0 - Sair\n");
}