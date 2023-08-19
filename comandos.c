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

void limpaTerminal(){
    printf("\ec\e[3J");
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

void preencheDisplay(int ***tabela, int n, char dificuldade){
    int sinal, aux;
    srand(time(NULL));

    if(dificuldade=='F' || dificuldade == 'f'){
        for(int i=0; i<n; i++)
            for(int j=0; j<n; j++)
                (*tabela)[i][j] = ((rand()%9)+1);

    }else if(dificuldade == 'M' || dificuldade == 'm'){
        for(int i=0; i<n; i++)
            for(int j=0; j<n; j++){
                aux = (rand()%3)+1;
                if(aux==1) sinal=-1;
                else sinal=1;
                (*tabela)[i][j] = sinal*((rand()%9)+1);
            }
            
    }else if(dificuldade == 'D' || dificuldade == 'd'){
        for(int i=0; i<n; i++)
            for(int j=0; j<n; j++){
                aux = (rand()%3)+1;
                if(aux==1) sinal=-1;
                else sinal=1;
                (*tabela)[i][j] = sinal*((rand()%10));
            }
    }
    
}

void preencheGabarito(int ***tabela, int n, char dificuldade){
    int cont1, cont0, aux;

    srand(time(NULL));

    if(dificuldade=='F' || dificuldade == 'f'){
        for(int i=0; i<n; i++)
            for(int j=0; j<n; j++){
                aux = (rand()%10)+1;
                if(aux<5) (*tabela)[i][j] = 1;
                else (*tabela)[i][j] = 0;
            }

    }else if(dificuldade=='M' || dificuldade == 'm'){
        for(int i=0; i<n; i++){
            cont1 = 0;
            cont0 = 0;
            for(int j=0; j<n; j++){
                aux = (rand()%10)+1;
                if(aux<5){
                    (*tabela)[i][j] = 1;
                    cont1++;
                }else{
                    (*tabela)[i][j] = 0;
                    cont0++;
                }
                if(cont1 == n || cont0 == n) preencheGabarito(tabela, n, dificuldade); 
            }
        }

    }else if(dificuldade=='D' || dificuldade == 'd'){
        for(int i=0; i<n; i++){
            cont1 = 0;
            for(int j=0; j<n; j++){
                aux = (rand()%10)+1;
                if(aux<5){
                    (*tabela)[i][j] = 1;
                    cont1++;
                }else{
                    (*tabela)[i][j] = 0;
                }
                if(cont1 == n) preencheGabarito(tabela, n, dificuldade); 
            }
        }
            
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
    for(int i=0; i<n; i++) printf(TAB_VER"\x1b[36m %-3d \x1b[0m", i+1);
    printf("\n");

    for(int i=0; i<n; i++){
        printf(CYAN("%d "), i+1);
        for(int j=0; j<n; j++){
            if((*status)[i][j] == 1) printf(TAB_VER"\x1b[32m %-3d \x1b[0m", (*tabela)[i][j]);
            else if((*status)[i][j] == 0) printf(TAB_VER"\x1b[31m %-3d \x1b[0m", (*tabela)[i][j]);
            else if((*status)[i][j] == 2) printf(TAB_VER"\x1b[34m %-3d \x1b[0m", (*tabela)[i][j]);
            else printf(TAB_VER" %-3d ", (*tabela)[i][j]);
        }
        printf(TAB_VER"\x1b[34m %-3d \x1b[0m", (*dicasV)[i]);

        printf("\n");
    }

    printf("  ");
    for(int i=0; i<n; i++){
        printf(TAB_VER"\x1b[34m %-3d \x1b[0m", (*dicasH)[i]);  
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
    fflush(stdin);
}

void limpaBuffer2(){
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

void salvaJogo(char nome[], char nomearquivo[], int ***display, int ***gabarito, int ***status, int **dicasH, int **dicasV, int n, double tempoTotal){
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

    fprintf(fp, "%.0lf", tempoTotal);
    

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

int verificaStatus(int **gabarito, int **status, int n){
    int flag = 0;

    /*for(int i=0; i<n; i++)
        for(int j=0; j<n; j++){
            if(status[i][j] == 2)
                status[i][j] = 1;
        }*/

    /*for(int i=0; i<n; i++)
        for(int j=0; j<n; j++){
            if(status[i][j] != gabarito[i][j]){
                flag = 0;
                printf("\nEntrou no flag 0");
                sleep(2);
            }
        }*/

    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++){
            if((gabarito[i][j] == 1 && status[i][j] == 1) || (gabarito[i][j] == 1 && status[i][j] == -1))
                    flag++;
            else if(gabarito[i][j] == 0 && status[i][j]==0)
                    flag++;    
        }

        
    if(flag==n*n){
        printf("\nVocê ganhou!!");
        return 1;
        sleep(2);
    }else return 0;

}

void printInputBuffer() {
    int c;
    printf("Input buffer contents: ");
    while ((c = getchar()) != '\n' && c != EOF) {
        printf("%d ", c);
    }
    printf("\n");
}

void telaVitoria(char nome[], int ***tabela, int ***gabarito, int *dicasH, int *dicasV, int n, double tempoTotal){
    char flag[10] = "";
    int tam;


    while(strcmp(flag, "voltar") != 0){
        limpaTerminal();

        printf(BOLD(GREEN("Parabéns %s, você concluiu o jogo!!\nTempo de jogo: %0.lf segundos")), nome, tempoTotal);

        printf("\nDigite 'voltar' para retornar ao menu principal: ");
        fgets(flag, 10, stdin);
        
        tam = strlen(flag);
        flag[tam-1] = '\0';

        //printf("\n|%s|", flag);
        if(!strcmp(flag, "voltar")) return;
    }

}

void imprimeRanking(Ranking rk){
    printf("Entrou em Imprimir ranking\n");
    for(int i=0; i<QTDGAPS; i++){
        if(rk.gap[i].qtd == 0) continue;
        printf("Size = %d\n", rk.gap[i].tam);

        for(int j=0; j<rk.gap[i].qtd; j++){
            printf("\tNome = %s\n\tTempo = %0.lf\n", rk.gap[i].jogadores[j].nome, rk.gap[i].jogadores[j].tempo);
        }
    }
    sleep(10);
}


Ranking leRanking(){
    FILE *fp = fopen("sumplete.ini", "r");
    Ranking rk;
    int size;
    char palavra[TAMPALAVRA];

    
    for (int i = 0; i < 7; i++) {
        rk.gap[i].qtd = 0;
        rk.gap[i].tam = i + 3;
    }

    fscanf(fp, "%s", palavra);

    for(int i=0; i<7 || !feof(fp); i++){
        
        fscanf(fp, "%s", palavra);  // =
        fscanf(fp, "%d", &size);

        int j = 0;
        for(j=0; j<MAXJOGADORES; j++){
            fscanf(fp, "%s", palavra);

            if(palavra[0] != 'j') {
                break;
            }   
                
            fscanf(fp, "%s", palavra);
            fgets(palavra, TAMPALAVRA, fp);
            palavra[strlen(palavra) - 1] = '\0';
            strcpy(rk.gap[size-3].jogadores[j].nome, palavra);
            

            fscanf(fp, "%s", palavra);
            fscanf(fp, "%s", palavra);
            fscanf(fp, "%lf", &rk.gap[size-3].jogadores[j].tempo);
            rk.gap[size-3].qtd++;
        }
        

    }

    for(int i=0; i<7; i++){
        printf("TAM: %d (%d)\n", i+3, rk.gap[i].qtd);
        
        for(int j=0; j<rk.gap[i].qtd; j++){
            printf("Nome: %s   Tempo: %lf\n", rk.gap[i].jogadores[j].nome, rk.gap[i].jogadores[j].tempo);
        }
        printf("\n");
    }

    fclose(fp);
    return rk;
}

int verificaFormato(char nome[25]){
    char aux[5];
    int tam, cont=3;

    tam = strlen(nome);

    for(int i=tam-1; i>tam-4-1 && cont >= 0; i--){
        aux[cont] = nome[i];
        cont --;
    }

    aux[4] = '\0';

    if(strcmp(aux, ".txt")==0) return 1;
    else return 0;
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