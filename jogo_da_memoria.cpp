#include <stdio.h>
#include <windows.h> // limpar terminal
#include <stdlib.h>
#include <time.h>
#include <string.h>

// o jogador:
typedef struct {
    char nome[50];
    int pontos;
} Jogador;

// ================================= FUN��ES DO JOGO =================================

// =============== Fun��o que embaralha as cartas ===============
// Distribuir as cartas da matriz "combinacoes"
// aleatoriamente dentro da matriz "cartas"
void embaralhar_cartas(char cartas[6][6], char combinacoes[6][6]){
	int altura_random, largura_random, altura, largura;
	altura = largura = 0;
	for(int carta = 0; carta < 36; carta++){
		// Tenta colocar uma carta em uma posi��o vazia do tabuleiro
		while(1){
			altura_random  = rand() % 6; // escolher um valor de 0 a 5
			largura_random = rand() % 6; // escolher um valor de 0 a 5
			if (cartas[altura_random][largura_random] == '*'){
				cartas[altura_random][largura_random] = combinacoes[altura][largura];
				break;
			}
		}
		largura++;
		if(largura > 5){
			largura = 0;
			altura++;
		}
	}
}

// ============ Fun��o para exibir o come�o do jogo ============ 
// Mostra total de combina��es encontradas, o tabuleiro,
// de quem � a rodada e as instru��es b�sicas.
void exibir_inicio_jogo(char nome[],int pontos, char tabela[6][6], int jogador, int multiplayer, int *jogadores_pontos, int computador) {
    int linha, coluna;
	
	// Exibe as regras:
	if(!computador){
    	printf("Regras:\nEscolha 2 cartas por rodada (linha+coluna juntas, ex: 1 e 2 = 12).\nPar = 1 ponto e joga de novo.\nNao formar par = cartas viram e troca a vez.\nTermina ao achar as 18 combinacoes.\n");
    	printf("---------------------------------------------------------------------\n");
	}

    // Exibe total de acertos:
    printf("Acertos: %d/18\n\n", pontos);
    
    // Cabe�alho do tabuleiro:
    printf("\n   1  2  3  4  5  6\n");
    
    // Imprime o tabuleiro:
    for (coluna = 0; coluna < 6; coluna++) {
        printf("%d ", coluna + 1);
        for (linha = 0; linha < 5; linha++) {
            printf(" %c ", tabela[coluna][linha]);
        }
        printf(" %c \n", tabela[coluna][linha]);
    }

    // Mensagem da rodada:
    if (multiplayer) {
        // Modo multiplayer:
        printf("\nRodada do Jogador %d!\n", jogador + 1);
    } else {
        // Modo singleplayer:
        if (jogador) {
            printf("\nRodada do Computador!\n");
        } else {
            printf("\nRodada de %s!\n", nome);
        }
    }
    
    // Exibe a pontua��o:
    printf("Total de pontos: %d\n", jogadores_pontos[jogador]);
}

// ============ Fun��o de pensamento do computador =============
// Analisa a mem�ria do computador e procura por duas cartas
// iguais que j� foram reveladas anteriormente. Se encontrar,
// salva as posi��es das cartas em proxima_rodada_bot[].
void pensamento_do_computador(char memoria_das_cartas[6][6], int memoria_das_posicoes[6][6], int *proxima_rodada_bot){
	
	// O valor que ele an�lisa:
	int largura_fixa      = 0;
	int altura_fixa       = 0;
	
	// O valor que ele compara:
	int largura_dinamica  = 0;
	int altura_dinamica   = 0;
	
	// Quando achar sai do loop
	int acabar_pensamento = 0;
	
	// Percorre toda a tabela "memoria_das_cartas" para an�lise
	for (int i = 0; i < 36; i++){
		// Verifica o valor em uma posi��o espec�fica da tabela:
		for(int a = 0; a < 36; a++){
		// Procura na tabela uma carta igual � analisada, mas em posi��o diferente
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


// ======= Tempo pro jogador lembrar da posicao da carta =======
// Mostra uma contagem regressiva no terminal, dando tempo para
// o jogador memorizar as cartas reveladas antes delas virarem de novo.
void tempo(int number){
	printf("\n%d",number);
	for(number-1;number > 1;number--){
		printf("\b%d",number);
		Sleep(1000); // Fun��o de windows.h
	}
	printf("\b1\b");
	Sleep(1000);
}

// =================== Jogada do computador ====================
// Analisa "proxima_rodada_bot" se tiver algo ele pega os 2 valores para a jogada
// Caso contr�rio, escolhe posi��es aleat�rias v�lidas.
void rodada_do_computador(int pontos, char tabela[6][6], int jogador, int multiplayer, int *jogadores_pontos, int *proxima_rodada_bot, char *escolha, int *posicoes, char memoria_das_cartas[6][6], char cartas[6][6]){
	for(int rodada = 0; rodada < 4; rodada += 2){
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
			// Armazena as posi��es selecionadas para an�lise
			posicoes[rodada]   = (escolha[0] - '0') - 1;
			posicoes[rodada+1] = (escolha[1] - '0') - 1;
			proxima_rodada_bot[rodada/2] = 0;
		} else {
			// Seleciona cartas aleat�rias n�o reveladas caso n�o haja jogada futura registrada
			while(1){
				posicoes[rodada]   = rand() % 6;
				posicoes[rodada+1] = rand() % 6;
				if(!rodada){ // jogada 1
					if(tabela[posicoes[rodada]][posicoes[rodada+1]] == '*'){
						memoria_das_cartas[posicoes[rodada]][posicoes[rodada+1]] = cartas[posicoes[rodada]][posicoes[rodada+1]];
						break;
					}
				} else { // jogada 2 ( garanti que ele n�o vai escolher o mesmo valor da jogada anterior ).
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
		exibir_inicio_jogo("nao" ,pontos, tabela, jogador, multiplayer, jogadores_pontos, 1);
		tempo(4);
	}
}


// ===================== Jogada do jogador =====================
// Espera a entrada correta do jogador. Quando ele fizer as 2 jogadas v�lidas,
// armazena as coordenadas em "posicoes" para an�lise posterior.
void rodada_do_player(char nome[],char memoria_das_cartas[6][6], char *escolha, int *posicoes, char tabela[6][6], char cartas[6][6], int pontos,int jogador, int multiplayer, int *jogadores_pontos){
	for(int rodada = 0;rodada < 4;rodada += 2){
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
				memoria_das_cartas[posicoes[rodada]][posicoes[rodada+1]]   = cartas[posicoes[rodada]][posicoes[rodada+1]]; // guardar na memoria do bot se n�o for multiplayer
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

// ==================== O Jogodo da memoria ====================
void jogo_da_memoria(int ranking, Jogador jogadores[100]){
	
	// Entrada do usu�rio para definir o modo de jogo atual
	int multiplayer;
	printf("====== MODOS DE JOGO ======\n(0) -> Jogador vs Computador\n(1) -> Jogador vs Jogador\nDigite sua escolha (0 ou 1): ");
	scanf("%d", &multiplayer);
	system("cls");
	Jogador player;

	if (!multiplayer){
	    printf("Digite seu nome: ");
	    scanf(" %49[^\n]", player.nome); // l� at� 49 caracteres ou Enter
	    player.pontos = 0; // inicializa a pontua��o
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

	// Lista de todas as combina��es
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
	
	// Mem�ria do computador para armazenar as cartas 
	char memoria_das_cartas[6][6]   = {
	{'*','*','*','*','*','*'},
	{'*','*','*','*','*','*'},
	{'*','*','*','*','*','*'},
	{'*','*','*','*','*','*'},
	{'*','*','*','*','*','*'},
	{'*','*','*','*','*','*'}
	};
	
	// Armazena as posi��es das cartas no baralho para refer�ncia e compara��o
	int  memoria_das_posicoes[6][6] = {
	{11,12,13,14,15,16},
	{21,22,23,24,25,26},
	{31,32,33,34,35,36},
	{41,42,43,44,45,46},
	{51,52,53,54,55,56},
	{61,62,63,64,65,66}
	};
	
	// Pr�xima jogada do computador caso encontre cartas iguais em posi��es diferentes
	int  proxima_rodada_bot[2] = { 0,0 };
	
	embaralhar_cartas(cartas, combinacoes);
	
	// Total de combina��es para acabar o jogo: 18
	int pontos              = 0;
	// Pontua��o dos jogadores: [Jogador 1] e [Jogador 2]
	int jogadores_pontos[2] = {0,0};
	// Alterna o jogador a cada rodada
	// jogador 1 = false
	// jogador 2 = true
	int jogador             = 0;
	
	// Loop principal do jogo:
	while(pontos < 18){
		int  posicoes[4];  // [linha 1], [coluna 1], [linha 2], [coluna 2]
		char escolha[3];   // [linha], [coluna] 
		
		if (!multiplayer){
			exibir_inicio_jogo(player.nome ,pontos, tabela, jogador, multiplayer, jogadores_pontos, 0);
		} else {
			exibir_inicio_jogo("nao",pontos, tabela, jogador, multiplayer, jogadores_pontos, 0);
		}
		
		
		// L�gica de decis�o do computador durante sua vez:
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

// ===================================================================================

int main(){
	// srand() --> Define a �semente� usada pela fun��o rand().
	// time()  --> Retorna o tempo atual em segundos
	srand(time(NULL));
	// Faz com que "rand()" gere n�meros diferentes a cada execu��o.
	
	// Guardar informa��es dos jogadores.
	Jogador jogadores[100];
	int ranking = 0;
	
	// ler quantidade de pessoas do arquivo "ranking.txt"
	FILE *arquivo = fopen("ranking.txt", "a+"); // se n�o existir ele cria o arquivo  
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
			    fclose(arquivo); // fechar o arquivo depois que voc� terminar de usar ele.
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
			    printf("Jogadores lidos do arquivo:\n\n");
			    while (fscanf(arquivo, "%49s %d", nome, &pontos) == 2) {
			        printf("Nome: %s, Pontos: %d\n", nome, pontos);
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
