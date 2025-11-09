#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

int main(){
	srand(time(NULL));

	// Tabela visual representando as cartas 
	char tabela[4][6] = {
	{'?','?','?','?','?','?'},
	{'?','?','?','?','?','?'},
	{'?','?','?','?','?','?'},
	{'?','?','?','?','?','?'}
	};

	// Lista de todas as combinações
	char combinacoes[4][6] = {
	{'A','A','B','B','C','C'},
	{'D','D','E','E','F','F'},
	{'G','G','H','H','I','I'},
	{'J','J','K','K','L','L'},
	};
	
	// cartas embaralhadas
	char cartas[4][6] = {
	{'?','?','?','?','?','?'},
	{'?','?','?','?','?','?'},
	{'?','?','?','?','?','?'},
	{'?','?','?','?','?','?'}
	};

	// embaralhar combinacoes para a lista de cartas
	int altura_random, largura_random, altura, largura;
	altura = largura = 0;
	for(int i = 0; i < 24; i++){
		while(1){
			altura_random  = rand() % 4;
			largura_random = rand() % 6;
			if (cartas[altura_random][largura_random] == '?'){
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
	
	// Pontuação máxima: 12
	int pontos              = 0;
	// [jogador 1] e [jogador 2]
	int jogadores_pontos[2] = {0,0};
	// Troca de jogador a cada rodada
	int jogador             = 0;
	
	// O jogo:
	while(pontos < 12){
		int  coluna, linha;
		int  posicoes[4]; //  [linha 1], [coluna 1], [linha 2], [coluna 2]
		char escolha[2];  //  [linha], [coluna] 
		
		printf("acertos: %d/12\n\n",pontos);
		
		// imprimir tabela no terminal:
		printf("    1   2   3   4   5   6\n  +---+---+---+---+---+---+\n");
		for(coluna = 0; coluna < 4; coluna++){
			printf("%d ",coluna+1);
			for(linha = 0; linha < 5; linha++){
				printf("| %c ",tabela[coluna][linha]);
			}
			printf("| %c |\n",tabela[coluna][linha]);
			printf("  +---+---+---+---+---+---+\n");
		}
			
		printf("\nRodada do Jogador %d!\nTotal de pontos: %d\nEscolha a carta que deseja revelar, seguindo a ordem: [linha][coluna].\n\n",jogador+1,jogadores_pontos[jogador]);
		
		// Primeira escolha de carta
		while(1){
			printf("Primeira carta:");
			scanf("%s", escolha);
			posicoes[0] = escolha[0] - '0'; // Converter char pra int
			posicoes[1] = escolha[1] - '0'; // Converter char pra int
			if((posicoes[0] < 7) && (posicoes[0] > 0) && (posicoes[1] < 7) && (posicoes[1] > 0) && (tabela[posicoes[0]-1][posicoes[1]-1] == '?')){
				break;
			}
		}
		printf("valor da carta[%d][%d]: %c\n\n",posicoes[0],posicoes[1],cartas[posicoes[0]-1][posicoes[1]-1]);
		
		// Segunda escolha de carta
		while(1){
			printf("Segunda carta:");
			scanf("%s", escolha);
			posicoes[2] = escolha[0] - '0'; // Converter char pra int
			posicoes[3] = escolha[1] - '0'; // Converter char pra int
			if((posicoes[2] < 7) && (posicoes[2] > 0) && (posicoes[3] < 7) && (posicoes[3] > 0) && (tabela[posicoes[2]-1][posicoes[3]-1] == '?') && (posicoes[0] != posicoes[2] || posicoes[1] != posicoes[3])){
				break;
			}
		}
		printf("valor da carta[%d][%d]: %c\n",posicoes[2],posicoes[3],cartas[posicoes[2]-1][posicoes[3]-1]);
		
		
		// Tempo para visualizar o valor da carta:
		printf("\n3");
		Sleep(1000);
		printf("\b2");
		Sleep(1000);
		printf("\b1");
		Sleep(1000);
		
		// Compara o valor das duas cartas selecionadas:
		if (cartas[posicoes[0]-1][posicoes[1]-1] == cartas[posicoes[2]-1][posicoes[3]-1]){
			system("cls");
			tabela[posicoes[0]-1][posicoes[1]-1] = tabela[posicoes[2]-1][posicoes[3]-1] = cartas[posicoes[0]-1][posicoes[1]-1];
			jogadores_pontos[jogador]++;
			pontos++;
		} else {
			system("cls");
			if (jogador == 0){
				jogador++;
			} else {
				jogador--;
			}
		}
	}
	
	return 0;
	
}
