#include "comandos.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "colorcodes.h"
#include <string.h>


int teste(){
    printf("Testando comando\n");
    return 0;
}

void alocaMemoria(int ***tabela, int n){
    *tabela = malloc(n * sizeof(int*));

    for(int i=0; i<n; i++){
        (*tabela)[i]= malloc(n * sizeof(int));
    }

}

void alocaMemoriaDicas(int **vetor, int n){
    *vetor = malloc(n * sizeof(int));
}

void preencheDisplay(int ***tabela, int n){
    
    srand(time(NULL));

    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            (*tabela)[i][j] = ((rand()%12)+1);
    
}

void preencheGabarito(int ***tabela, int n){
    int aux;

    srand(time(NULL));

    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++){
            aux = (rand()%10)+1;
            if(aux<4) (*tabela)[i][j] = 1;
            else (*tabela)[i][j] = 0;
        }
    
}

void zeraStatus(int ***tabela, int n){
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            (*tabela)[i][j] = 0;
}

void imprime(int ***tabela, int ***status, int **dicasH, int **dicasV, int n){

    printf("\n");

    printf("  ");
    for(int i=0; i<n; i++) printf("%-3d |", i+1);
    printf("\n");

    for(int i=0; i<n; i++){
        printf("%d ", i+1);
        for(int j=0; j<n; j++){
            if((*status)[i][j] == 1) printf(GREEN("%-3d |"), (*tabela)[i][j]);
            else if((*status)[i][j] == 0) printf(RED("%-3d |"), (*tabela)[i][j]);
            else printf("%-3d ", (*tabela)[i][j]);
        }
        printf("%d", (*dicasV)[i]);

        printf("\n");
    }

    printf("  ");
    for(int i=0; i<n; i++){
        printf("%-3d |", (*dicasH)[i]);  
    }
    
    /*printf("\n");
    printf(TAB_TL TAB_HOR TAB_TJ TAB_HOR TAB_TJ TAB_HOR TAB_TR "\n");
    printf(TAB_VER "a" TAB_VER "b" TAB_VER "c" TAB_VER "\n");
    printf(TAB_ML TAB_HOR TAB_MJ TAB_HOR TAB_MJ TAB_HOR TAB_MR "\n");
    printf(TAB_VER "d" TAB_VER "e" TAB_VER "f" TAB_VER "\n");
    printf(TAB_BL TAB_HOR TAB_BJ TAB_HOR TAB_BJ TAB_HOR TAB_BR "\n");
    printf("\n");

    printf("\n");*/
    
}

void preencheDicas(int ***tabela, int ***gabarito, int **vetor, int n, char c[2]){

    if(!strcmp(c, "v\0")){
        for(int i=0; i<n; i++){
                (*vetor)[i] = 0;
                for(int j=0; j<n; j++){
                    if((*gabarito)[i][j] == 1) (*vetor)[i] += (*tabela)[i][j]; 
                }
        }

    }else{
        for(int i=0; i<n; i++){
                (*vetor)[i] = 0;
                for(int j=0; j<n; j++){
                    if((*gabarito)[j][i] == 1) (*vetor)[i] += (*tabela)[j][i]; 
                }
        }
    }
}

void liberaMatrizes(int ***tabela, int n){

for(int i=0; i<n; i++)
        free((*tabela)[i]);
    free(*tabela);
}

void liberaVetores(int **vetor, int n){
    free(*vetor);
}

void limpaBuffer(){
    while(getchar() != '\n');
}

void mudaStatus(int ***tabela, int i, int j, int operacao){
    switch(operacao){
        case 1:
            (*tabela)[i][j] = 1;
            break;
        case 0:
            (*tabela)[i][j] = 0;
            break;
        case -1:
            (*tabela)[i][j] = -1;
            break;
        default: printf(BOLD(RED("\nErro na operação")));
    }
}