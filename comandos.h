#ifndef COMANDOS_H //evita múltiplas inclusões
#define COMANDOS_H


int teste();

void alocaMemoria(int ***matriz, int n);

void preencheDisplay(int ***tabela, int n);

void imprime(int ***tabela, int ***status, int **dicasH, int **dicasV, int n);

void preencheGabarito(int ***tabela, int n);

void zeraStatus(int ***tabela, int n);

void alocaMemoriaDicas(int **vetor, int n);

void preencheDicas(int ***tabela, int ***gabarito, int **vetor, int n, char c[2]);

void liberaMatrizes(int ***tabela, int n);

void liberaVetores(int **vetor, int n);

void limpaBuffer();

void mudaStatus(int ***tabela, int i, int j, int operacao);

void titulo();

void salvaJogo(char nome[], char nomearquivo[], int ***display, int ***gabarito, int ***status, int **dicasH, int **dicasV, int n);

void carregaSalvo(char nomearquivo[]);

#endif //COMANDOS_H