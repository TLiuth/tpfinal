//Thiago Ayolphi Liuth 2314005
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comandos.h"
#include "colorcodes.h"
#include <time.h>
#include <unistd.h>


typedef struct 
{
    int **display;
    int **gabarito;
    int **status;
}matriz;

void menu();
void inicializar();
void operador(char nome[], matriz *mz, int *dicasH, int *dicasV, int n);

int main(){
    
    printf("\ec\e[3J");

    //titulo();

    menu();




    /*for(int i=0; i<n; i++) printf("\n%d", dicasV[i]);
    printf("\n\n");
    for(int i=0; i<n; i++) printf("\n%d", dicasH[i]);
    printf("\n\n");*/

    /*mz.status[1][3] = -1;
    mz.status[2][3] = -1;
    mz.status[2][4] = -1;
    mz.status[4][4] = 1;
    mz.status[5][4] = 1;
    mz.status[4][5] = 1;*/

    /*imprime(&(mz.display), &(mz.status), n);
    printf("\nGabarito:\n");
    imprime(&(mz.gabarito), &(mz.status), n);*/


    /*liberaMatrizes(&(mz.display), n);
    liberaMatrizes(&(mz.gabarito), n);
    liberaMatrizes(&(mz.status), n);

    liberaVetores(&dicasV, n);
    liberaVetores(&dicasH, n);*/
    

    return 0;
}

void inicializar(){
    char nome[15], dificuldade;
    int n, *dicasH, *dicasV;
    matriz mz;

    limpaBuffer();
    printf("Digite o nome do jogador 1: ");
    scanf("%s", nome);

    printf("\nDigite o tamanho do tabuleiro (3 a 9): ");
    scanf("%d", &n);

    limpaBuffer();

    printf("\nDigite o nível de dificuldade (Fácil, Médio ou Difícil): ");
    scanf("%c", &dificuldade);

    printf("\n\n");


    alocaMemoria(&(mz.display), n);
    alocaMemoria(&(mz.gabarito), n);
    alocaMemoria(&(mz.status), n);
    alocaMemoriaDicas(&dicasH, n);
    alocaMemoriaDicas(&dicasV, n);

    preencheDisplay(&(mz.display), n);
    preencheGabarito(&(mz.gabarito), n);
    zeraStatus(&(mz.status), n);

    preencheDicas(&(mz.display), &(mz.gabarito), &dicasV, n, "v\0");
    preencheDicas(&(mz.display), &(mz.gabarito), &dicasH, n, "h\0");


    operador(nome, &mz, dicasH, dicasV, n);
    menu();
    /*imprime(&(mz.display), &(mz.status), n);*/

    liberaMatrizes(&(mz.display), n);
    liberaMatrizes(&(mz.gabarito), n);
    liberaMatrizes(&(mz.status), n);

    liberaVetores(&dicasV, n);
    liberaVetores(&dicasH, n);
    printf("\n\nMemória Liberada");
    sleep(1);
    exit(0);

}

void operador(char nome[], matriz *mz, int *dicasH, int *dicasV, int n){
    char leitura[11], comando[9], num[3], c;
    int x=0, i, j, pos=0, auxpos=0;

    
    

    while(1){
        //printf("\ec\e[3J");

        printf(BOLD(BLUE("+ o + !!SUMPLETE!! o + o \n")));
        imprime(&(mz->display), &(mz->status), &dicasH, &dicasV, n);
        printf("\n");
        //imprime(&(mz->gabarito), &(mz->status), &dicasH, &dicasV, n);
        //printf("\n");
        //imprime(&(mz->status), &(mz->status), &dicasH, &dicasV, n);
        limpaBuffer();
        printf("\n%s, digite o comando: ", nome);

        fgets(leitura, 11, stdin);

        int tam = strlen(leitura);

        for(int i=0; i<tam; i++){
            if(leitura[i]==' '){
                pos = i;
                break;
            }else pos = tam-1;
        }

        for(int j=0; j<pos; j++)
            comando[j] = leitura[j];
        comando[pos] = '\0';

        if(!strcmp(comando, "manter") || (!strcmp(comando, "remover"))){
            for(int j=0; j<2; j++){
                pos++;
                num[j] = leitura[pos];
            }
            num[2] = '\0';
            x = atoi(num);
            j = x%10;
            i = x/10;
        }


        if(x < 11 || x>((10*n)+n)){
            printf(BOLD(RED("\nPosição inválida.\n")));
            continue;
        }
        

        
        if(!strcmp(comando, "manter")){
            mudaStatus(&(mz->status), i-1, j-1, 1);
        }else if(!strcmp(comando, "remover")){
            mudaStatus(&(mz->status), i-1, j-1, 0);
        }else if(!strcmp(comando, "voltar")){
            printf("\nEntrou em voltar");
            menu();
            return;
        }
    }

}

void menu(){
    int op;
    printf(BLUE("+ o + !!Bem vindo ao jogo SUMPLETE!! o + o "));

    printf("\n\n0. Sair do jogo\n1. Começar um novo jogo\n2. Continuar um jogo salvo em arquivo\n3. Continuar o jogo atual\n4. Exibir o ranking\nDurante o jogo, digite 'voltar' para retonar a esse menu.\n");
    printf("\n\nEscolha uma opção: ");

    scanf("%d", &op);

    switch(op){
        case 0:
            return;
            exit(0);
        case 1:
            inicializar();
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
    }

}
