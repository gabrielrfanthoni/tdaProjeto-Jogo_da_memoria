// memory_game.c — Jogo da Memória 4x4 em C (C99)

#include <stdio.h>      // Inclui funções de entrada/saída como printf, scanf, FILE*
#include <stdlib.h>     // Inclui malloc, free, rand, srand, exit
#include <string.h>     // Inclui funções de string como strcpy, strlen, fgets
#include <time.h>       // Inclui time (usado para srand)

// ------------------- CONFIGURAÇÕES GERAIS -------------------

#define N 4             // Define o tamanho do tabuleiro como 4 (4x4) conforme solicitado
#define MAX_NAME 32     // Tamanho máximo do nome de um jogador/ranking
#define MAX_SCORES 100  // Quantidade máxima de registros que carregaremos do ranking

// ------------------- ESTRUTURAS (struct) -------------------

typedef struct {
    int id;         // Identificador do par (0..(N*N/2 - 1)); para 4x4 temos 8 pares: 0..7
    int revealed;   // Flag: 1 se a carta está revelada temporariamente, 0 se está oculta
    int matched;    // Flag: 1 se a carta já foi combinada e retirada do jogo, 0 caso contrário
} Card;             // Define o tipo Card

typedef struct {
    char name[MAX_NAME]; // Nome do jogador (string terminada em '\0')
    int points;          // Pontos do jogador (quantidade de pares encontrados)
    int is_cpu;          // Flag: 1 se este "jogador" for a CPU, 0 se for humano
} Player;                // Define o tipo Player

typedef struct {
    char name[MAX_NAME]; // Nome do jogador para o ranking
    int points;          // Pontos salvos no ranking
} Score;                 // Define o tipo Score (para persistência)

// ------------------- FUNÇÕES AUXILIARES DE ENTRADA -------------------

void flush_line(void) {                       // Função para limpar o restante da linha no stdin
    int c;                                    // Variável para ler caracteres
    while ((c = getchar()) != '\n' && c != EOF) { // Consome até achar quebra de linha ou EOF
        /* nada */                            // Corpo vazio intencionalmente
    }
}

void read_line(char *buf, size_t size) {      // Função para ler uma linha segura (com fgets)
    if (fgets(buf, (int)size, stdin)) {       // Lê até size-1 caracteres e inclui '\n' se couber
        size_t len = strlen(buf);             // Calcula o tamanho lido
        if (len && buf[len - 1] == '\n') {    // Se o último caractere for '\n'
            buf[len - 1] = '\0';              // Remove o '\n' substituindo por terminador de string
        }
    }
}

// ------------------- ALOCAÇÃO E INICIALIZAÇÃO DO TABULEIRO -------------------

Card **alloc_board(void) {                              // Aloca dinamicamente uma matriz N×N de Card
    Card **board = (Card **)malloc(N * sizeof(Card*));  // Aloca vetor de N ponteiros para linhas
    if (!board) {                                       // Verifica se a alocação falhou
        perror("malloc");                               // Imprime erro do sistema
        exit(1);                                        // Encerra o programa com código de erro
    }
    for (int i = 0; i < N; i++) {                       // Itera sobre as N linhas
        board[i] = (Card *)malloc(N * sizeof(Card));    // Aloca N Cards para cada linha
        if (!board[i]) {                                // Verifica falha de alocação por linha
            perror("malloc");                           // Imprime erro
            exit(1);                                    // Encerra
        }
    }
    return board;                                       // Retorna a matriz alocada
}

void free_board(Card **board) {                         // Libera a memória da matriz alocada
    for (int i = 0; i < N; i++) {                       // Para cada linha
        free(board[i]);                                 // Libera o bloco de Cards
    }
    free(board);                                        // Libera o vetor de ponteiros
}

void shuffle(int *arr, int len) {                       // Embaralha um vetor de inteiros (Fisher–Yates)
    for (int i = len - 1; i > 0; --i) {                 // Começa do final até 1
        int j = rand() % (i + 1);                       // Escolhe um índice aleatório entre 0 e i
        int tmp = arr[i];                               // Troca arr[i] com arr[j]
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

void init_board(Card **board) {                         // Inicializa o tabuleiro com pares embaralhados
    int total = N * N;                                  // Total de casas (para 4x4 = 16)
    int pairs = total / 2;                              // Total de pares (para 16 = 8)
    int *ids = (int *)malloc(total * sizeof(int));     // Aloca vetor temporário para IDs duplicados
    if (!ids) {                                        // Verifica alocação
        perror("malloc");                               // Imprime erro
        exit(1);                                        // Encerra
    }

    for (int i = 0; i < pairs; ++i) {                   // Para cada par
        ids[2 * i] = i;                                 // Coloca o id i em posição par
        ids[2 * i + 1] = i;                             // Duplica o id i na próxima posição
    }
    shuffle(ids, total);                                // Embaralha os IDs para randomizar posições

    int k = 0;                                          // Índice para percorrer ids
    for (int r = 0; r < N; ++r) {                       // Percorre linhas
        for (int c = 0; c < N; ++c) {                   // Percorre colunas
            board[r][c].id = ids[k++];                  // Atribui um id embaralhado à carta
            board[r][c].revealed = 0;                   // Marca como oculta inicialmente
            board[r][c].matched = 0;                    // Marca como não combinada
        }
    }
    free(ids);                                          // Libera o vetor temporário de IDs
}

// ------------------- EXIBIÇÃO E VALIDAÇÃO -------------------

char id_to_char(int id) {                               // Converte id numérico em uma letra visual
    return (char)('A' + id);                            // 0->'A', 1->'B', ..., 7->'H'
}

void print_board(Card **board) {                        // Imprime o tabuleiro na tela
    printf("\n    ");                                   // Espaço inicial e alinhamento
    for (int c = 0; c < N; ++c) printf("%2d ", c);      // Cabeçalho de colunas: 0..N-1
    printf("\n");                                       // Quebra de linha

    for (int r = 0; r < N; ++r) {                       // Para cada linha
        printf("%2d  ", r);                             // Mostra índice da linha
        for (int c = 0; c < N; ++c) {                   // Para cada coluna
            if (board[r][c].matched) {                  // Se a carta já foi combinada
                printf(" . ");                          // Mostra ponto (removida)
            } else if (board[r][c].revealed) {          // Se a carta está momentaneamente revelada
                printf(" %c ", id_to_char(board[r][c].id)); // Mostra a letra correspondente
            } else {                                    // Caso contrário (oculta)
                printf(" * ");                          // Mostra asterisco
            }
        }
        printf("\n");                                   // Quebra de linha ao fim da linha do tabuleiro
    }
}

int in_bounds(int r, int c) {                           // Verifica se (r,c) está dentro dos limites
    return r >= 0 && r < N && c >= 0 && c < N;          // Retorna 1 se dentro, 0 se fora
}

int valid_pick(Card **board, int r, int c) {            // Verifica se a escolha de (r,c) é válida
    if (!in_bounds(r, c)) return 0;                     // Fora do tabuleiro → inválida
    if (board[r][c].matched) return 0;                  // Carta já combinada → inválida
    if (board[r][c].revealed) return 0;                 // Carta já revelada agora → inválida
    return 1;                                           // Caso contrário → válida
}

// ------------------- ENTRADAS DO JOGADOR E CPU -------------------

void read_human_pick(Card **board, int *r, int *c, const char *label) { // Lê jogada do humano
    while (1) {                                          // Loop até entrada válida
        printf("%s (linha coluna): ", label);            // Mostra rótulo (1ª ou 2ª carta)
        int ok = scanf("%d %d", r, c);                   // Lê dois inteiros (linha e coluna)
        if (ok != 2) {                                   // Se falhou a leitura
            printf("Entrada invalida. Tente de novo.\n"); // Mensagem de erro
            flush_line();                                // Limpa a linha restante
            continue;                                    // Recomeça o loop
        }
        flush_line();                                    // Limpa resíduos após leitura válida
        if (!valid_pick(board, *r, *c)) {                // Se a posição não for válida
            printf("Posicao invalida/ocupada. Tente de novo.\n"); // Mensagem
            continue;                                    // Pede outra entrada
        }
        break;                                           // Sai do loop quando válido
    }
}

void cpu_pick(Card **board, int *r, int *c) {            // Escolha simples da CPU
    int tries = 0;                                       // Contador de tentativas
    do {                                                 // Tenta até achar posição válida
        *r = rand() % N;                                 // Sorteia linha entre 0..N-1
        *c = rand() % N;                                 // Sorteia coluna entre 0..N-1
        tries++;                                         // Incrementa tentativas
        if (tries > 1000) break;                         // Fuga de segurança para não travar
    } while (!valid_pick(board, *r, *c));                // Repete se não for válida
}

// ------------------- ESTADO DO JOGO -------------------

int board_done(Card **board) {                           // Verifica se todas as cartas foram combinadas
    for (int r = 0; r < N; ++r) {                        // Percorre linhas
        for (int c = 0; c < N; ++c) {                    // Percorre colunas
            if (!board[r][c].matched) return 0;          // Se encontrar alguma não combinada → ainda não acabou
        }
    }
    return 1;                                            // Se todas combinadas → jogo terminou
}

// ------------------- RANKING: CARREGAR / MOSTRAR / SALVAR -------------------

int load_scores(Score *scores, int maxn) {               // Carrega até maxn entradas do arquivo ranking.txt
    FILE *f = fopen("ranking.txt", "r");                 // Tenta abrir ranking.txt para leitura
    if (!f) return 0;                                    // Se não existe, retorna 0 (sem erro)
    int n = 0;                                           // Contador de linhas lidas
    while (n < maxn &&                                   // Enquanto tiver espaço no vetor
           fscanf(f, "%31s %d",                          // Lê nome (até 31 chars) e pontos
                  scores[n].name, &scores[n].points) == 2) { // Se leu os dois com sucesso
        n++;                                             // Incrementa quantidade lida
    }
    fclose(f);                                           // Fecha o arquivo
    return n;                                            // Retorna quantas entradas foram lidas
}

int cmp_score_desc(const void *a, const void *b) {       // Função de comparação para qsort (ordem decrescente)
    const Score *sa = (const Score *)a;                  // Converte ponteiro genérico para Score
    const Score *sb = (const Score *)b;                  // Converte ponteiro genérico para Score
    return (sb->points - sa->points);                    // Retorna diferença invertida para ordem decrescente
}

void show_ranking(void) {                                // Mostra o ranking ordenado
    Score scores[MAX_SCORES];                            // Vetor local para armazenar entradas
    int n = load_scores(scores, MAX_SCORES);             // Carrega entradas do arquivo
    if (n == 0) {                                        // Se não carregou nada
        printf("\nRanking vazio.\n");                    // Informa que está vazio
        return;                                          // Retorna ao menu
    }
    qsort(scores, n, sizeof(Score), cmp_score_desc);     // Ordena por pontos (maior → menor)
    printf("\n=== RANKING ===\n");                       // Título
    for (int i = 0; i < n && i < 20; ++i) {              // Mostra no máximo top 20
        printf("%2d) %-16s  %3d\n",                      // Formatação: posição, nome alinhado, pontos
               i + 1, scores[i].name, scores[i].points); // i+1 é a posição humana (1-based)
    }
}

void save_score(const char *name, int points) {          // Salva uma entrada ao final do arquivo
    FILE *f = fopen("ranking.txt", "a");                 // Abre em modo append (cria se não existir)
    if (!f) {                                            // Se falhar ao abrir
        perror("fopen");                                  // Mostra erro do sistema
        return;                                          // Não encerra o programa; apenas não salva
    }
    fprintf(f, "%s %d\n", name, points);                 // Escreve "nome pontos" em uma linha
    fclose(f);                                           // Fecha o arquivo
}

// ------------------- LÓGICA PRINCIPAL DO JOGO -------------------

void play_game(void) {                                   // Função que conduz uma partida completa
    Card **board = alloc_board();                        // Aloca a matriz dinâmica N×N
    init_board(board);                                   // Preenche/embaralha as cartas
    Player p1 = {0};                                     // Zera estrutura do jogador 1
    Player p2 = {0};                                     // Zera estrutura do jogador 2

    printf("Digite o nome do Jogador 1: ");              // Pede nome do jogador 1
    read_line(p1.name, MAX_NAME);                        // Lê linha com segurança
    if (strlen(p1.name) == 0) strcpy(p1.name, "Jogador1");// Se vazio, usa nome padrão
    p1.points = 0;                                       // Zera pontos do jogador 1
    p1.is_cpu = 0;                                       // Jogador 1 é sempre humano

    printf("Existe Jogador 2? (s/n): ");                 // Pergunta se haverá segundo jogador humano
    char ans[8];                                         // Buffer para resposta
    read_line(ans, sizeof(ans));                         // Lê a resposta
    if (ans[0] == 's' || ans[0] == 'S') {                // Se usuário respondeu sim
        printf("Digite o nome do Jogador 2: ");          // Pede nome do jogador 2
        read_line(p2.name, MAX_NAME);                    // Lê nome do jogador 2
        if (strlen(p2.name) == 0) strcpy(p2.name, "Jogador2"); // Se vazio, usa padrão
        p2.is_cpu = 0;                                   // Jogador 2 é humano
    } else {                                             // Caso contrário
        strcpy(p2.name, "CPU");                          // Define nome como "CPU"
        p2.is_cpu = 1;                                   // Marca como jogador computador
    }
    p2.points = 0;                                       // Zera pontos do jogador 2

    int current = 0;                                     // Indica de quem é a vez (0 = p1, 1 = p2)

    while (!board_done(board)) {                         // Loop principal enquanto houver cartas não combinadas
        Player *P = (current == 0) ? &p1 : &p2;          // Ponteiro para o jogador atual
        printf("\nVez de %s%s\n",                        // Mensagem indicando de quem é a vez
               P->name, P->is_cpu ? " (CPU)" : "");      // Se for CPU, anexa "(CPU)"
        print_board(board);                              // Mostra o tabuleiro atual

        int r1, c1, r2, c2;                              // Variáveis para posições escolhidas

        if (!P->is_cpu) {                                // Se o jogador atual é humano
            read_human_pick(board, &r1, &c1, "Escolha 1a carta"); // Lê a primeira carta
        } else {                                         // Se for CPU
            cpu_pick(board, &r1, &c1);                   // Deixa a CPU escolher aleatoriamente
            printf("CPU escolheu: %d %d\n", r1, c1);     // Mostra a escolha da CPU
        }
        board[r1][c1].revealed = 1;                      // Revela a primeira carta
        print_board(board);                              // Mostra o tabuleiro com a primeira carta aberta

        if (!P->is_cpu) {                                // Se o jogador atual é humano
            read_human_pick(board, &r2, &c2, "Escolha 2a carta"); // Lê a segunda carta
        } else {                                         // Se for CPU
            do {                                         // Repete até achar uma segunda carta válida e diferente da primeira
                cpu_pick(board, &r2, &c2);               // Escolha da CPU
            } while (r2 == r1 && c2 == c1);              // Garante que não repetiu a mesma posição
            printf("CPU escolheu: %d %d\n", r2, c2);     // Mostra a segunda escolha
        }
        board[r2][c2].revealed = 1;                      // Revela a segunda carta
        print_board(board);                              // Mostra o tabuleiro com as duas cartas abertas

        if (board[r1][c1].id == board[r2][c2].id) {      // Verifica se formou par (ids iguais)
            printf("Par encontrado! %s ganha 1 ponto e joga novamente.\n", P->name); // Mensagem de acerto
            P->points += 1;                              // Incrementa pontos do jogador atual
            board[r1][c1].matched = 1;                   // Marca a primeira carta como combinada
            board[r2][c2].matched = 1;                   // Marca a segunda carta como combinada
            // Nota: não alternamos 'current' aqui — quem acerta joga de novo
        } else {                                         // Se não formou par
            printf("Nao foi par.\n");                    // Mensagem de erro
            board[r1][c1].revealed = 0;                  // Esconde novamente a primeira carta
            board[r2][c2].revealed = 0;                  // Esconde novamente a segunda carta
            current = 1 - current;                       // Alterna a vez (0->1, 1->0)
        }
    }

    print_board(board);                                  // Imprime tabuleiro final (tudo combinado como '.')
    printf("\nJogo encerrado! Pontos: %s=%d, %s=%d\n",   // Mostra placar final
           p1.name, p1.points, p2.name, p2.points);      // Imprime nomes e pontos

    if (p1.points > p2.points) {                         // Compara pontos para decidir vencedor
        printf("Vencedor: %s\n", p1.name);               // Anuncia vencedor
        save_score(p1.name, p1.points);                  // Salva pontuação do vencedor no ranking
    } else if (p2.points > p1.points) {                  // Se o jogador 2/CPU venceu
        printf("Vencedor: %s\n", p2.name);               // Anuncia vencedor
        save_score(p2.name, p2.points);                  // Salva pontuação do vencedor
    } else {                                             // Caso de empate
        printf("Empate!\n");                             // Informa empate
        save_score(p1.name, p1.points);                  // Salva pontos do jogador 1
        if (strcmp(p2.name, "CPU") != 0) {               // Se o jogador 2 não for CPU (evita poluir ranking com "CPU")
            save_score(p2.name, p2.points);              // Salva também pontos do jogador 2
        }
    }

    free_board(board);                                   // Libera a memória alocada para o tabuleiro
}

// ------------------- ITENS DE MENU -------------------

void show_credits(void) {                                // Imprime créditos do projeto
    printf("\nCreditos:\n");                             // Jogo da Memória 4X4
    printf("- Equipe: Gabriel, Fillipe Ribeiro Berssot Mori, ");   // Nome das pessoas do grupo
    printf("Vitória, Jadson, Maria Eduarda.\n");         // Nome das pessoas do grupo
    printf("- Disciplina: TDA 2025.2\n");                // TÉCNICAS E DESENVOLVIMENTO DE ALGORITMOS / 2ª Unidade
}

void menu(void) {                                        // Imprime e trata o menu principal
    while (1) {                                          // Loop do menu até o usuário escolher sair
        printf("\n=== JOGO DA MEMORIA (4x4) ===\n");     // Título do jogo/menu
        printf("1) Jogar\n");                            // Opção 1: iniciar partida
        printf("2) Ver Ranking\n");                      // Opção 2: mostrar ranking
        printf("3) Creditos\n");                         // Opção 3: créditos
        printf("0) Sair\n");                             // Opção 0: sair do programa
        printf("Escolha: ");                             // Solicita escolha ao usuário

        int op = -1;                                     // Inicializa variável da opção
        if (scanf("%d", &op) != 1) {                     // Tenta ler um inteiro
            printf("Entrada invalida.\n");               // Se falhar, informa inválido
            flush_line();                                // Limpa o buffer
            continue;                                    // Volta ao menu
        }
        flush_line();                                    // Limpa resíduos após leitura

        if (op == 1) {                                   // Se escolha for 1
            play_game();                                 // Inicia partida
        } else if (op == 2) {                            // Se escolha for 2
            show_ranking();                              // Mostra ranking
        } else if (op == 3) {                            // Se escolha for 3
            show_credits();                              // Mostra créditos
        } else if (op == 0) {                            // Se escolha for 0
            printf("Saindo...\n");                       // Mensagem de saída
            break;                                       // Encerra o loop do menu
        } else {                                         // Qualquer outro número
            printf("Opcao invalida.\n");                 // Informa opção inválida
        }
    }
}

// ------------------- FUNÇÃO PRINCIPAL -------------------

int main(void) {                                         // Função principal do programa
    srand((unsigned)time(NULL));                         // Inicializa gerador de números aleatórios com base no tempo
    menu();                                              // Chama o menu principal
    return 0;                                            // Retorna 0 indicando execução bem-sucedida
}
