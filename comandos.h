/*
Trabalho Final Introdução à Programação 2023/1
Professor: Puca Huachi V. Penna
Aluno: Thiago Ayolphi Liuth
Matrícula: 2314005
*/
#ifndef COMANDOS_H //evita múltiplas inclusões
#define COMANDOS_H
#define MAXJOGADORES 5
#define TAMNOME 64
#define QTDGAPS 7 //Quantidade de dimensões
#define TAMPALAVRA 50

typedef struct {
    char nome[TAMNOME]; //Nome do jogador
    double tempo; //Tempo do jogador
} Jogador;

typedef struct {
    int tam; //Dimensão
    int qtd; //Quantidade de jogadores registrados nesse tamanho
    Jogador jogadores[MAXJOGADORES];
}SizeGap;

//Struct que armazena o ranking
typedef struct{
    SizeGap gap[QTDGAPS]; //Quantidade de dimensões
}Ranking;

int teste();

void limpaTerminal();

void alocaMemoria(int ***matriz, int n);

void preencheDisplay(int ***tabela, int n, char dificuldade);

void imprime(int ***tabela, int ***status, int **dicasH, int **dicasV, int n);

void preencheGabarito(int ***tabela, int n, char dificuldade);

void resetaStatus(int ***tabela, int n);

void alocaMemoriaDicas(int **vetor, int n);

void preencheDicas(int ***tabela, int ***gabarito, int **vetor, int n, char c[2]);

void liberaMatrizes(int ***tabela, int n);

void liberaVetores(int **vetor, int n);

void limpaBuffer();

void limpaBuffer2();

void mudaStatus(int ***tabela, int i, int j, int operacao);

void titulo();

void salvaJogo(char nome[], char nomearquivo[], int ***display, int ***gabarito, int ***status, int **dicasH, int **dicasV, int n, double);

void dica(int ***gabarito, int ***status, int n, int flag);

void resolve(int ***gabarito, int ***status, int n);

int verificaStatus(int **gabarito, int **status, int n);

int verificaVetor(int vetor[], int **tabela, int **status, int n, int i);

int verificaVetorH(int vetor[], int **tabela, int **status, int n, int i);

int verificaFormato(char nome[TAMNOME]);

void telaVitoria(char nome[], int ***tabela, int ***gabarito, int *dicasH, int *dicasV, int n, double);

Ranking atualizaRanking(double tempo, char nome[TAMNOME], int);

Ranking ordenaRanking(Ranking rk);

void imprimeRanking(Ranking rk, char, int);

Ranking leRanking();

#endif //COMANDOS_H