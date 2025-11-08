#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

int main(){
	
	// Tabela visual representando as cartas 
	char tabela[4][6] = {
	{'?','?','?','?','?','?'},
	{'?','?','?','?','?','?'},
	{'?','?','?','?','?','?'},
	{'?','?','?','?','?','?'}
	};
	
	// Lista todas as combinações
	char cartas[4][6] = {
	{'A','O','F','E','C','U'},
	{'P','G','B','D','D','I'},
	{'F','A','P','H','U','C'},
	{'O','G','B','H','E','I'}
	};
	
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
		printf("    1   2   3   4   5   6\n  _________________________\n");
		for(coluna = 0; coluna < 4; coluna++){
			printf("%d ",coluna+1);
			for(linha = 0; linha < 5; linha++){
				printf("| %c ",tabela[coluna][linha]);
			}
			printf("| %c |\n",tabela[coluna][linha]);
			printf("  |---|---|---|---|---|---|\n");
		}
		
		// Primeira escolha de carta
		printf("\nRodada do Jogador %d!\nTotal de pontos: %d\nEscolha a carta que deseja revelar, seguindo a ordem: [linha][coluna].\nPrimeira carta:",jogador+1,jogadores_pontos[jogador]);
		scanf("%s", escolha);
		posicoes[0] = escolha[0] - '0'; // Converter char pra int
		posicoes[1] = escolha[1] - '0'; // Converter char pra int
		printf("%c\n",cartas[posicoes[0]-1][posicoes[1]-1]);
		
		// Segunda escolha de carta
		printf("Segunda carta:");
		scanf("%s", escolha);
		posicoes[2] = escolha[0] - '0'; // Converter char pra int
		posicoes[3] = escolha[1] - '0'; // Converter char pra int
		printf("%c\n",cartas[posicoes[2]-1][posicoes[3]-1]);
		
		
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