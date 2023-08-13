#include "comandos.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "colorcodes.h"
#include <string.h>
#include <unistd.h>

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
            (*tabela)[i][j] = -1;
}

void imprime(int ***tabela, int ***status, int **dicasH, int **dicasV, int n){

    printf("\n");

    printf("  ");
    for(int i=0; i<n; i++) printf(TAB_VER" %-3d ", i+1);
    printf("\n");

    for(int i=0; i<n; i++){
        printf("%d ", i+1);
        for(int j=0; j<n; j++){
            if((*status)[i][j] == 1) printf(TAB_VER"\x1b[32m %-3d \x1b[0m", (*tabela)[i][j]);
            else if((*status)[i][j] == 0) printf(TAB_VER"\x1b[31m %-3d \x1b[0m", (*tabela)[i][j]);
            else if((*status)[i][j] == 2) printf(TAB_VER"\x1b[34m %-3d \x1b[0m", (*tabela)[i][j]);
            else printf(TAB_VER" %-3d ", (*tabela)[i][j]);
        }
        printf(TAB_VER" %-3d ", (*dicasV)[i]);

        printf("\n");
    }

    printf("  ");
    for(int i=0; i<n; i++){
        printf(TAB_VER" %-3d ", (*dicasH)[i]);  
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
    int c;
    while((c = getchar()) != '\n' && c != EOF);
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

void salvaJogo(char nome[], char nomearquivo[], int ***display, int ***gabarito, int ***status, int **dicasH, int **dicasV, int n){
    int marcados=0, removidos=0;
    FILE *fp;
    fp = fopen(nomearquivo, "w");

    fprintf(fp, "%d\n", n);
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++)
            fprintf(fp, "%d ", (*display)[i][j]);
        fprintf(fp, "\n");
    }

    for(int i=0; i<n; i++) fprintf(fp,"%d ", (*dicasV)[i]);
    fprintf(fp, "\n");
    for(int i=0; i<n; i++) fprintf(fp, "%d ", (*dicasH)[i]);
    fprintf(fp, "\n");



    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if((*status)[i][j] == 1) marcados++;
            if((*status)[i][j] == 0) removidos++;
        }
            
    }
    fprintf(fp, "%d\n", marcados);


    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++)
            if((*status)[i][j] == 1) fprintf(fp, "%d %d\n", i, j);
    }

    fprintf(fp, "%d\n", removidos);


    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++)
            if((*status)[i][j] == 0) fprintf(fp, "%d %d\n", i, j);
    }

    fprintf(fp, "%s\n", nome);

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++)
            fprintf(fp, "%d ", (*gabarito)[i][j]);
        fprintf(fp, "\n");
    }
    

    fclose(fp);

}


void dica(int ***gabarito, int ***status, int n, int flag){
    if(flag){
    printf("\nFLAG 1");
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++){
            if((*gabarito)[i][j] == 1 && ((*status)[i][j]!=1)){
                printf("\nFLAG 2");
                (*status)[i][j] = 1;
                return;
            }
        }
    }else{
        printf(BLUE("Você já recebeu uma dica :)\n"));
        sleep(1);
    } 

}

void resolve(int ***gabarito, int ***status, int n){
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++){
            if((*gabarito)[i][j] == 1 && (*status)[i][j]!=1)
                (*status)[i][j] = 1;
        }
}

void verificaStatus(int **gabarito, int **status, int n){
    int flag = 1;

    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++){
            if(status[i][j] == 2)
                status[i][j] = 1;
        }

    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++){
            if(status[i][j] != gabarito[i][j])
                flag = 0;
        }
        
    if(flag){
        printf("\nVocê ganhou!!");
        sleep(2);
    }

}



void titulo(){

    printf(MAGENTA(
         "  ███████╗██╗   ██╗███╗   ███╗██████╗ ██╗     ███████╗████████╗███████╗\n"
         "  ██╔════╝██║   ██║████╗ ████║██╔══██╗██║     ██╔════╝╚══██╔══╝██╔════╝\n"
         "  ███████╗██║   ██║██╔████╔██║██████╔╝██║     █████╗     ██║   █████╗  \n"
         "  ╚════██║██║   ██║██║╚██╔╝██║██╔═══╝ ██║     ██╔══╝     ██║   ██╔══╝  \n"
         "  ███████║╚██████╔╝██║ ╚═╝ ██║██║     ███████╗███████╗   ██║   ███████╗\n\n")); 

    int totalIterations = 30; // Total number of iterations for the loading bar
    int currentIteration = 0; // Current iteration

    printf("Loading: [");

    while (currentIteration <= totalIterations) {
        // Calculate the percentage completion
        float progress = (float)currentIteration / totalIterations * 100;

        // Print the progress bar
        printf("%.1f%%", progress);

        // Print the bar itself
        int numBars = progress / 4; // Scale progress to the number of bars
        printf(" [");
        for (int i = 0; i < numBars; i++) {
            printf("#");
        }
        for (int i = numBars; i < 25; i++) {
            printf(" ");
        }
        printf("]");

        // Clear the line to update the progress
        fflush(stdout);
        printf("\r");

        // Sleep for a short duration to simulate loading time
        usleep(100000); // Sleep for 100 milliseconds

        currentIteration++;
    }

}