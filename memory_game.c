#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// o jogador:
typedef struct {
    char nome[50];
    int  pontos;
} Jogador;

// ----------------------------------------------------------------------
//                              FUNÇÕES DO JOGO
// ----------------------------------------------------------------------

//  Função:  embaralhar_cartas
//  Entradas:
//      - cartas:      Matriz com as cartas usadas no jogo
//      - combinacoes: Matriz com as combinações que serão embaralhadas
//
//  Lógica:
//      - Para cada item em "combinacoes", seleciona uma posição aleatória na matriz "cartas" e armazena.
//
//  Saída:
//      - As combinações embaralhadas aleatoriamente dentro da matriz "cartas".

void embaralhar_cartas(char cartas[6][6], char combinacoes[6][6]){
	int altura_random, largura_random, altura, largura;
	altura = largura = 0;
	
	int carta_escolhida;
	
	for(carta_escolhida = 0; carta_escolhida < 36; carta_escolhida++){
		
		do{
			altura_random  = rand() % 6; // escolher um valor de 0 a 5
			largura_random = rand() % 6; // escolher um valor de 0 a 5
		}while (cartas[altura_random][largura_random] != '*'); // Loop: só sai quando uma posição vazia ('*') for encontrada
		
		// Copia a carta de "combinacoes" para a posição aleatória disponível em "cartas"
		cartas[altura_random][largura_random] = combinacoes[altura][largura];
		
		largura++;
		if(largura == 6){
			largura = 0;
			altura++;
		}
	}
}

// ----------------------------------------------------------------------

//  Função:  exibir_inicio_jogo
//  Entradas:
//  	- nome_jogador:            Nome do jogador (se houver)
//      - combinacoes_encontradas: Total de combinações encontradas
//  	- tabela:                  Matriz representando o tabuleiro a ser exibido
//  	- jogador_rodada:          Jogador da rodada atual
//   	- multiplayer:             (0 ou 1) indicando se o modo é multiplayer
//   	- jogadores_pontos:        Vetor com o total de pontos de cada jogador
//   	- computador_rodada:       (0 ou 1) indicando se é a rodada do computador
//
//  Lógica:
//   	- Imprime as regras se for rodada de um jogador
//   	- Mostra o total de combinações encontradas
//   	- Exibe visualmente a tabela no terminal
//  	- Indica de quem é a rodada
//   	- Mostra os pontos do jogador da rodada
//
//  Saída:
//   	- Visualização do início da rodada no terminal

void exibir_inicio_jogo(char nome_jogador[],int combinacoes_encontradas, char tabela[6][6], int jogador_rodada, int multiplayer, int *jogadores_pontos, int computador_rodada) {
    int linha, coluna;
    
    // 1. Exibe as regras (apenas se não for o computador)
    if (!computador_rodada) {
    	puts("Regras:\nEscolha 2 cartas por rodada (linha+coluna juntas, ex: linha 1 e coluna 2 = 12).");
    	puts("Par = 1 ponto e joga de novo.");
    	puts("Nao formar par = cartas viram e troca a vez.");
    	puts("Termina ao achar as 18 combinacoes.");
    	puts("---------------------------------------------------------------------");
    }

    // 2. Exibe total de acertos
    printf("Acertos: %d/18\n\n", combinacoes_encontradas);
    
    // 3. Imprime o tabuleiro
    puts("   1  2  3  4  5  6");
    for (coluna = 0; coluna < 6; coluna++) {
        printf("%d ", coluna + 1);
        for (linha = 0; linha < 5; linha++) {
            printf(" %c ", tabela[coluna][linha]);
        }
        printf(" %c \n", tabela[coluna][linha]);
    }

    // 4. Mensagem da rodada
    if ( multiplayer ) {      // Rodada multiplayer
        printf("\nRodada do Jogador %d!\n", jogador_rodada + 1);
    } else {
        if ( jogador_rodada ) { // Rodada do computador
            printf("\nRodada do Computador!\n");
        } else {            // Rodada do jogador no modo singleplayer
            printf("\nRodada de %s!\n", nome_jogador);
        }
    }
    
    // 5. Exibe pontuação
    printf("Total de pontos: %d\n", jogadores_pontos[jogador_rodada]);
}

// ----------------------------------------------------------------------

//  Função: pensamento_do_computador
//  Entradas:
//   	- memoria_das_cartas:   Matriz onde o computador armazena as cartas que já foram reveladas.
//   	- memoria_das_posicoes: Matriz com as posições correspondentes de cada carta.
//   	- proxima_rodada_bot:   Array onde o computador guarda as posições das cartas
//                              que pretende jogar na próxima rodada.
//
//  Lógica:
//   	- O computador analisa "memoria_das_cartas" procurando pares de cartas iguais
//     	  em posições diferentes.
//   	- Se encontrar um par, armazena as posições correspondentes em "proxima_rodada_bot".
//
//  Saída:
//   	- Define em "proxima_rodada_bot" as posições da próxima jogada, se houver par encontrado.

void pensamento_do_computador(char memoria_das_cartas[6][6], int memoria_das_posicoes[6][6], int *proxima_rodada_bot){
	
	// O valor que ele analisa:
	int largura_fixa      = 0;
	int altura_fixa       = 0;
	
	// O valor que ele compara:
	int largura_dinamica  = 0;
	int altura_dinamica   = 0;
	
	// Quando achar sai do loop
	int acabar_pensamento = 0;
	
	// Percorre toda a tabela "memoria_das_cartas" para analise
	int fixo, dinamico;
	for ( fixo = 0; fixo < 36; fixo++){
		// Verifica o valor em uma posicao especefica da tabela:
		for( dinamico = 0; dinamico < 36; dinamico++){
		// Procura na tabela uma carta igual a analisada, mas em posicao diferente
			if((memoria_das_cartas[altura_fixa][largura_fixa] == memoria_das_cartas[altura_dinamica][largura_dinamica])&&(memoria_das_posicoes[altura_fixa][largura_fixa] != memoria_das_posicoes[altura_dinamica][largura_dinamica])&&(memoria_das_cartas[altura_fixa][largura_fixa] != '*')){
				proxima_rodada_bot[0] = memoria_das_posicoes[altura_fixa][largura_fixa];
				proxima_rodada_bot[1] = memoria_das_posicoes[altura_dinamica][largura_dinamica];
				acabar_pensamento = 1;
				break;
			}
			largura_dinamica++;
			if ( largura_dinamica == 6 ){
				largura_dinamica  = 0;
				altura_dinamica++;
			}
		}
		largura_dinamica = altura_dinamica = 0;
		if(acabar_pensamento){
			break;
		}
		largura_fixa++;
		if (largura_fixa == 6){
			largura_fixa = 0;
			altura_fixa++;
		}
	}
}

// ----------------------------------------------------------------------

//  Função: tempo
//  Entrada:
//   	- number: Inteiro representando o tempo (em segundos) que a função irá contar.
//
//  Lógica:
//   	- Realiza uma contagem regressiva de "number" até 0.
//   	- Pausa 1 segundo a cada decremento.
//
//  Saída:
//   	- Exibe a contagem regressiva no terminal.

void tempo(int number){
	printf("\n%d",number);
	for( number-1 ; number > 1 ; number--){
		printf("\b%d", number);
		Sleep(1000); // Função de <windows.h>
	}
	printf("\b1\b");
	Sleep(1000);
}

// ----------------------------------------------------------------------

//  Função: rodada_do_computador
//  Entrada:
//   	- (As mesmas da função "exibir_inicio_jogo")
//		- proxima_rodada_bot:  Vetor contendo jogadas previamente descobertas pelo
//                             computador (ex.: posições de um par já identificado).
//   	- escolha:             Array temporário usado para converter a jogada salva
//                             (inteiro) para string e recuperar suas posições.
//   	- posicoes:            Vetor onde serão salvas as posições escolhidas pelo
//                             computador para a rodada atual.
//   	- memoria_das_cartas:  Matriz onde o computador armazena cartas que já foram
//                             reveladas no jogo.
//   	- cartas:              Matriz contendo todas as cartas do tabuleiro.
//
//  Lógica:
//   	- A função executa duas escolhas: a primeira carta e a segunda carta.
//   	- Para cada carta:
//        	1. Se existir uma jogada registrada em "proxima_rodada_bot", ela é usada.
//        	2. Caso contrário, o computador escolhe uma posição aleatória válida:
//           	- A carta escolhida deve estar oculta ('*').
//           	- Na segunda jogada, a carta não pode ser a mesma da primeira.
//   	- As posições escolhidas são salvas em "posicoes".
//   	- A carta é revelada no tabuleiro e mostrada ao jogador.
//   	- A função também registra a carta revelada na memória do computador.
//
//  Saída:
//   	- Atualiza o vetor "posicoes" com as duas escolhas do computador, revelando
//        ambas no tabuleiro para análise posterior.

void rodada_do_computador(int combinacoes_encontradas, char tabela[6][6], int jogador_rodada, int multiplayer, int *jogadores_pontos, int *proxima_rodada_bot, char *escolha, int *posicoes, char memoria_das_cartas[6][6], char cartas[6][6]){
	
	int rodada;
	for(rodada = 0; rodada < 4; rodada += 2){
		if(!rodada){
			printf("O computador escolhe a primeira carta:");
		} else {
			printf("O computador escolhe a segunda carta:");
		}
		
		// 0 / 2 = 0
		// 2 / 2 = 1
		if(proxima_rodada_bot[rodada/2]){
			// converter inteiro para string:
			sprintf(escolha, "%d", proxima_rodada_bot[rodada/2]);
			// Armazena as posicoes selecionadas para analise
			posicoes[rodada]   = (escolha[0] - '0') - 1;
			posicoes[rodada+1] = (escolha[1] - '0') - 1;
			proxima_rodada_bot[rodada/2] = 0;
		} else {
			// Seleciona cartas aleatorias nao reveladas caso nao haja jogada futura registrada
			while(1){
				posicoes[rodada]   = rand() % 6;
				posicoes[rodada+1] = rand() % 6;
				if(!rodada){ // jogada 1
					if(tabela[posicoes[rodada]][posicoes[rodada+1]] == '*'){
						memoria_das_cartas[posicoes[rodada]][posicoes[rodada+1]] = cartas[posicoes[rodada]][posicoes[rodada+1]];
						break;
					}
				} else { // jogada 2 ( garanti que ele nao vai escolher o mesmo valor da jogada anterior ).
					if(tabela[posicoes[rodada]][posicoes[rodada+1]] == '*' && (posicoes[rodada] != posicoes[0] || posicoes[rodada+1] != posicoes[1]) ){
						memoria_das_cartas[posicoes[rodada]][posicoes[rodada+1]] = cartas[posicoes[rodada]][posicoes[rodada+1]];
						break;
					}
				}
			}		
		}
		// aparecer valor na tabela:
		tabela[posicoes[rodada]][posicoes[rodada+1]] = cartas[posicoes[rodada]][posicoes[rodada+1]];
		system("cls");
		exibir_inicio_jogo("nao" ,combinacoes_encontradas, tabela, jogador_rodada, multiplayer, jogadores_pontos, 1);
		tempo(4);
	}
}

// ----------------------------------------------------------------------

// Função: rodada_do_player
// Entradas:
//   	- (As mesmas da função "exibir_inicio_jogo")
//   	- escolha:             Array temporário usado para armazenar a entrada
//                             digitada pelo jogador.
//   	- posicoes:            Vetor onde serão salvas as posições escolhidas pelo
//                             jogador para a rodada atual.
//   	- memoria_das_cartas:  Matriz onde são registradas cartas reveladas, usada
//                             pelo computador apenas no modo singleplayer.
//   	- cartas:              Matriz contendo todas as cartas reais do tabuleiro.
//
//  Lógica:
//   	- A função solicita ao jogador que escolha duas cartas (primeira e segunda).
//   	- Cada escolha é convertida e armazenada em "posicoes".
//   	- A carta correspondente é revelada no tabuleiro e exibida na tela.
//   	- Caso esteja no modo singleplayer, a carta revelada também é armazenada na
//        memória do computador ("memoria_das_cartas") para uso posterior.
//
//  Saída:
//   	- Atualiza o vetor "posicoes" com as duas escolhas feitas pelo jogador,
//        revelando ambas no tabuleiro para análise da jogada atual.

void rodada_do_player(char nome[],char memoria_das_cartas[6][6], char *escolha, int *posicoes, char tabela[6][6], char cartas[6][6], int pontos,int jogador, int multiplayer, int *jogadores_pontos){
	int rodada;
	for(rodada = 0;rodada < 4;rodada += 2){
		while(1){
			if(!rodada){
				printf("Primeira carta: ");
			} else {
				printf("Segunda carta: ");
			}
			scanf("%s", escolha);
			posicoes[rodada]   = ( escolha[0] - '0' ) - 1; // Converte caractere para inteiro
			posicoes[rodada+1] = ( escolha[1] - '0' ) - 1; // Converte caractere para inteiro
			if((posicoes[rodada] < 7) && (posicoes[rodada] > -1) && (posicoes[rodada+1] < 7) && (posicoes[rodada+1] > -1) && (tabela[posicoes[rodada]][posicoes[rodada+1]] == '*')){
				tabela[posicoes[rodada]][posicoes[rodada+1]]               = cartas[posicoes[rodada]][posicoes[rodada+1]];
				if(!multiplayer){
					memoria_das_cartas[posicoes[rodada]][posicoes[rodada+1]]   = cartas[posicoes[rodada]][posicoes[rodada+1]]; // guardar na memoria do bot se nao for multiplayer
				}
				system("cls");
				if(!multiplayer){
					exibir_inicio_jogo(nome,pontos, tabela, jogador, multiplayer, jogadores_pontos, 0);
				} else {
					exibir_inicio_jogo("nao",pontos, tabela, jogador, multiplayer, jogadores_pontos, 0);
				}
				break;
			}
		}
	}
	tempo(3);
}

// -----------------------------------------------------------------------------

// Função: jogo_da_memoria
// Entradas:
//   - ranking:   Índice onde o jogador será salvo no sistema de ranking ao final.
//   - jogadores: Array contendo os dados dos jogadores armazenados.
//
// Lógica:
//   - Pergunta ao usuário o modo de jogo e armazena em "multiplayer".
//   - Se o modo for singleplayer, solicita o nome do jogador.
//
//   - Cria todas as matrizes necessárias para o funcionamento do jogo:
//       1. tabela:               Representação visual das cartas mostradas ao jogador.
//       2. combinacoes:          Lista com todas as combinações possíveis.
//       3. cartas:               As cartas reais embaralhadas no tabuleiro.
//       4. memoria_das_cartas:   Memória do computador (singleplayer) para lembrar cartas reveladas.
//       5. memoria_das_posicoes: Posições correspondentes às cartas para lógica do bot.
//
//   - Cria listas e variáveis auxiliares:
//       1. proxima_rodada_bot:   Próxima jogada planejada pelo computador.
//       2. pontos:               Total de combinações encontradas no jogo.
//       3. jogadores_pontos:     Pontuação individual de cada jogador.
//       4. jogador:              Indica de quem é a vez (0 = jogador 1, 1 = jogador 2/computador).
//
//   - Embaralha as cartas chamando a função apropriada.
//
//   - Entra no loop principal do jogo:
//       1. Recebe as posições escolhidas na rodada e as armazena em "posicoes".
//       2. Analisa as posições para verificar se formam uma combinação válida.
//       3. Caso positivo, incrementa:
//             - "pontos" (total geral)
//             - "jogadores_pontos[jogador]"
//       4. Se o jogador acertar, joga novamente; caso contrário, passa a vez
//          para o próximo jogador ou para o computador.
//
//   - Após o fim do jogo:
//       - Exibe quem venceu e os pontos de cada jogador.
//       - Se o modo for singleplayer, salva na memória o nome do jogador e sua pontuação.
//
// Saída:
//   - Executa o jogo completo e registra pontuações quando aplicável.
// -----------------------------------------------------------------------------

void jogo_da_memoria(int ranking, Jogador jogadores[100]){
	
	// Entrada do usuario para definir o modo de jogo atual
	int multiplayer;
	printf("====== MODOS DE JOGO ======\n(0) -> Jogador vs Computador\n(1) -> Jogador vs Jogador\nDigite sua escolha (0 ou 1): ");
	scanf("%d", &multiplayer);
	system("cls");
	Jogador player;

	if (!multiplayer){
	    printf("Digite seu nome: ");
	    scanf(" %49[^\n]", player.nome); // l ate 49 caracteres ou Enter
	    player.pontos = 0; // inicializa a pontuacao
	}
	system("cls");
	
	// Tabela visual representando as cartas 
	char tabela[6][6]      = {
	{'*','*','*','*','*','*'},
	{'*','*','*','*','*','*'},
	{'*','*','*','*','*','*'},
	{'*','*','*','*','*','*'},
	{'*','*','*','*','*','*'},
	{'*','*','*','*','*','*'}
	};

	// Lista de todas as combinacoes
	char combinacoes[6][6] = {
	{'A','A','B','B','C','C'},
	{'D','D','E','E','F','F'},
	{'G','G','H','H','I','I'},
	{'J','J','K','K','L','L'},
	{'M','M','N','N','O','O'},
	{'Q','Q','R','R','S','S'}
	};
	
	// cartas embaralhadas
	char cartas[6][6]      = {
	{'*','*','*','*','*','*'},
	{'*','*','*','*','*','*'},
	{'*','*','*','*','*','*'},
	{'*','*','*','*','*','*'},
	{'*','*','*','*','*','*'},
	{'*','*','*','*','*','*'}
	};
	
	// Memoria do computador para armazenar as cartas 
	char memoria_das_cartas[6][6]   = {
	{'*','*','*','*','*','*'},
	{'*','*','*','*','*','*'},
	{'*','*','*','*','*','*'},
	{'*','*','*','*','*','*'},
	{'*','*','*','*','*','*'},
	{'*','*','*','*','*','*'}
	};
	
	// Armazena as posicoes das cartas no baralho para referencia e comparacao
	int  memoria_das_posicoes[6][6] = {
	{11,12,13,14,15,16},
	{21,22,23,24,25,26},
	{31,32,33,34,35,36},
	{41,42,43,44,45,46},
	{51,52,53,54,55,56},
	{61,62,63,64,65,66}
	};
	
	// Proxima jogada do computador caso encontre cartas iguais em posi??es diferentes
	int  proxima_rodada_bot[2] = { 0,0 };
	// Total de combinacoes para acabar o jogo: 18
	int pontos                 = 0;
	// Pontuacao dos jogadores: [Jogador 1] e [Jogador 2]
	int jogadores_pontos[2]    = {0,0};
	// Alterna o jogador a cada rodada
	// jogador 1 = false
	// jogador 2 = true
	int jogador                = 0;
	
	embaralhar_cartas(cartas, combinacoes);
	
	// Loop principal do jogo:
	while(pontos < 18){
		int  posicoes[4];  // [linha 1], [coluna 1], [linha 2], [coluna 2]
		char escolha[3];   // [linha], [coluna] 
		
		if (!multiplayer && !jogador){
			exibir_inicio_jogo(player.nome ,pontos, tabela, jogador, multiplayer, jogadores_pontos, 0);
		} else if (!jogador) {
			exibir_inicio_jogo("nao",pontos, tabela, jogador, multiplayer, jogadores_pontos, 0);
		}
		
		
		// L?gica de decisao do computador durante sua vez:
		if (!multiplayer && jogador){
			pensamento_do_computador( memoria_das_cartas, memoria_das_posicoes, proxima_rodada_bot);
		}
		
		// Turno do computador
		if (!multiplayer && jogador) {
			rodada_do_computador(pontos, tabela, jogador, multiplayer, jogadores_pontos,proxima_rodada_bot,escolha,posicoes,memoria_das_cartas,cartas);
		// Vez do jogador, conta tambem se for multiplayer:
		} else {
			if (!multiplayer){
				rodada_do_player(player.nome, memoria_das_cartas, escolha, posicoes, tabela, cartas, pontos,jogador, multiplayer, jogadores_pontos);
			} else {
				rodada_do_player("nao", memoria_das_cartas, escolha, posicoes, tabela, cartas, pontos,jogador, multiplayer, jogadores_pontos);
			}
		}
		
		// Compara o valor das duas cartas selecionadas:
		if (cartas[posicoes[0]][posicoes[1]] == cartas[posicoes[2]][posicoes[3]]){
			tabela[posicoes[0]][posicoes[1]] = tabela[posicoes[2]][posicoes[3]] = '.';
			system("cls");
			if(!multiplayer){
				memoria_das_cartas[posicoes[0]][posicoes[1]] = '*';
				memoria_das_cartas[posicoes[2]][posicoes[3]] = '*';
			}
			jogadores_pontos[jogador]++;
			pontos++;
		} else {
			tabela[posicoes[0]][posicoes[1]] = tabela[posicoes[2]][posicoes[3]] = '*';	
			system("cls");
			if (!jogador){
				jogador++;
			} else {
				jogador--;
			}
		}
	}
	
	// Fim do jogo:
	switch (multiplayer) {
    case 0:
        if(jogadores_pontos[0] > jogadores_pontos[1]){
        	printf("Vitoria de %s!\n", player.nome);
        } else if(jogadores_pontos[0] < jogadores_pontos[1]){
        	printf("Vitoria do Computador!\n");
    	} else {
    		printf("Empate!\n");
		}
		printf("%s pontos: %d\nComputador pontos: %d\n",player.nome,jogadores_pontos[0],jogadores_pontos[1]);
		strcpy(jogadores[ranking].nome, player.nome);
    	jogadores[ranking].pontos = jogadores_pontos[0];
        break;

    case 1:
    	if(jogadores_pontos[0] > jogadores_pontos[1]){
        	printf("Vitoria do Jogador 1!\n");
        } else if(jogadores_pontos[0] < jogadores_pontos[1]){
        	printf("Vitoria do Jogador 2!\n");
    	} else {
    		printf("Empate!\n");
		}
    	break;
	}
	
}

// ----------------------------------------------------------------------
//                              MENU DO JOGO
// ----------------------------------------------------------------------

int main(){
	// srand() --> Define a semente usada pela funcao rand().
	// time()  --> Retorna o tempo atual em segundos
	srand(time(NULL));
	// Faz com que "rand()" gere numeros diferentes a cada execucao.
	
	// Guardar informacoes dos jogadores.
	Jogador jogadores[100];
	int ranking = 0;
	
	// ler quantidade de pessoas do arquivo "ranking.txt"
	FILE *arquivo = fopen("ranking.txt", "a+"); // se n?o existir ele cria o arquivo  
	char nome[50];
	int  pontos;
	while (fscanf(arquivo, "%49s %d", nome, &pontos) == 2) {
		ranking++;
	}
	fclose(arquivo);
	
	while(1){
		
		// texto do menu:
		printf("======= MENU =======\n(0) -> Jogar\n(1) -> Ver Ranking\n(2) -> Creditos\n(3) -> Sair\n\nDigite sua escolha (0 a 3): ");
		
		int  escolha_menu;
		
		int  sair = 0;
		char voltar;
		
		
		scanf(" %d", &escolha_menu);
		
		switch(escolha_menu){
			case 0: {
				system("cls");
				jogo_da_memoria(ranking, jogadores);
				
				// Salvar jogador na memoria:
				FILE *arquivo = fopen("ranking.txt", "a"); // "a" = adicionar no fina
			    if (arquivo == NULL) {
			        printf("Erro ao abrir o arquivo!\n");
			        return 1;
			    }
			    fprintf(arquivo, "%s %d\n", jogadores[ranking].nome, jogadores[ranking].pontos); // escreve no arquivo: NOME PONTOS
			    fclose(arquivo); // fechar o arquivo depois que voce terminar de usar ele.
			    ranking++;
			    
				printf("Digite (s) e aperte Enter para voltar: ");
				scanf(" %c", &voltar);
				if(voltar == 's' || voltar == 'S'){
					system("cls");
					continue;
				}
				
				break;
			}
			case 1: {
				system("cls");
				
				// Procurar jogador na memoria:
				FILE *arquivo = fopen("ranking.txt", "r"); // "r" = ler texto
			    if (arquivo == NULL) {
			        printf("Erro ao abrir o arquivo para leitura!\n");
			        return 1;
			    }
			    printf("Jogadores:\n\n");
			    while (fscanf(arquivo, "%49s %d", nome, &pontos) == 2) {
			        printf("%s -> Pontos: %d\n", nome, pontos);
			    }
			
			    fclose(arquivo);
			    
			    
			    printf("\nDigite (s) e aperte Enter para voltar: ");
				scanf(" %c", &voltar);
				if(voltar == 's' || voltar == 'S'){
					system("cls");
					continue;
				}
			    break;
			}
			case 2:{
				system("cls");
				printf("=================================================\nProjeto Tecnicas e Desenvolvimento de Algoritmos\n=================================================\nGrupo:\n\n");
				printf("Fillipe Berssot\nGabriel Anthoni\nJadson\nMaria Eduarda\nMaria Vitoria\n\n");
				printf("Digite (s) e aperte Enter para voltar: ");
				scanf(" %c", &voltar);
				if(voltar == 's' || voltar == 'S'){
					system("cls");
					continue;
				}
				break;
			}
			case 3:{
				sair = 1;
				break;
			}
		}
		if (sair){
			break;
		}
	}
}
