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
    int n=0, *dicasH, *dicasV;
    matriz mz;

    printf("\ec\e[3J");
    limpaBuffer();
    printf("Digite o nome do jogador 1: ");
    scanf("%s", nome);

    while(n<3 || n>9){
        printf("\nDigite o tamanho do tabuleiro (3 a 9): ");
        scanf("%d", &n);
        if(n<3 || n>9) printf(RED("Tamanho inválido. Digite novamente."));
    }

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
    printf("\n\nMemória Liberada\n");
    exit(0);

}

void operador(char nome[], matriz *mz, int *dicasH, int *dicasV, int n){
    char leitura[16], comando[12], num[3], c;
    int x=11, i, j, pos=0, auxpos=0;
    limpaBuffer();
    
    

    while(1){
        //printf("\ec\e[3J");

        printf(BOLD(BLUE("+ o + !!SUMPLETE!! o + o \n")));
        imprime(&(mz->display), &(mz->status), &dicasH, &dicasV, n);
        printf("\n");
        //imprime(&(mz->gabarito), &(mz->status), &dicasH, &dicasV, n);
        //printf("\n");
        //imprime(&(mz->status), &(mz->status), &dicasH, &dicasV, n);
        //limpaBuffer();
        printf("\n%s, digite o comando: ", nome);

        //Leitura do comando
        fgets(leitura, 16, stdin);

        //aritmética para isolar o comando da posição, em caso dos comandos manter ou voltar
        int tam = strlen(leitura);
        for(int i=0; i<tam; i++){
            if(leitura[i]==' '){
                pos = i;
                break;
            }else pos = tam-1;
        }

        //o comando é isolado
        for(int j=0; j<pos; j++)
            comando[j] = leitura[j];
        comando[pos] = '\0';

        //no caso dos comandos manter ou remover, a posição é isolada e transformada em um inteiro
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

        //posições que não abrangem o tamanho do tabuleiro são inválidas
        if(x < 11 || x>((10*n)+n)){
            printf(BOLD(RED("\nPosição inválida.\n")));
            sleep(2);
            continue;
        }

        
        if(strcmp(comando, "salvar")){
            char nomearquivo[15];
            auxpos=0;
            while(pos < tam && leitura[pos]!='\n'){
                nomearquivo[auxpos] = leitura[pos];
                pos++;
                auxpos++;
            }
            nomearquivo[auxpos] = '\0';
            strcat(nomearquivo, ".txt");
            printf("\nNOME AQRUIVO: %s", nomearquivo);
        }
        


        
    
        
        if(!strcmp(comando, "manter")){
            mudaStatus(&(mz->status), i-1, j-1, 1);
        }else if(!strcmp(comando, "remover")){
            mudaStatus(&(mz->status), i-1, j-1, 0);
        }else if(!strcmp(comando, "voltar")){
            printf("\nEntrou em voltar");
            salvaJogo(nome, "temporario.txt", &(mz->display), &(mz->gabarito), &(mz->status), &dicasH, &dicasV, n);
            menu();
            return;
        }else if(!strcmp(comando, "salvar")){
            salvaJogo(nome, nomearquivo, &(mz->display), &(mz->gabarito), &(mz->status), &dicasH, &dicasV, n);
        }
    }

}

void menu(){
    int op;
    printf(BLUE("+ o + !!Bem vindo ao jogo SUMPLETE!! o + o "));

    printf("\n\n0. Sair do jogo\n1. Começar um novo jogo\n2. Continuar um jogo salvo em arquivo\n3. Continuar o jogo atual\n4. Exibir o ranking\nDurante o jogo, digite 'voltar' para retonar a esse menu.\n");
    printf("\n\nEscolha uma opção: ");

    
    scanf("%d", &op);

    while(op<0 || op>4){
        printf("\n\nEscolha uma opção: ");
        scanf("%d", &op);
        if(op<0 || op>4)
            printf(RED("Comando inválido. Digite novamente"));
        
    }

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
            carregaSalvo("temporario.txt");
            break;
        case 4:
            break;
    
    
    }

}


/*void carregaSalvo(char nomearquivo[]){
    FILE *fp;
    fp = fopen(nomearquivo, "r");
    char arquivo[15], nome[15];
    int n=0, *dicasH, *dicasV, marcados=0, removidos=0, p, q;
    matriz mz;

    printf("Entrou em carregaSalvo");
    sleep(2);

    while(fp == NULL){
        printf("Arquivo não existe. Digite novamente.\n");
        scanf("%s", arquivo);
        fp = fopen(nomearquivo, "r");
    }

    fscanf(fp, "%d", &n);

    printf("\nN: %d\n", n); //Debugar

    alocaMemoria(&(mz.display), n);
    alocaMemoria(&(mz.gabarito), n);
    alocaMemoria(&(mz.status), n);
    alocaMemoriaDicas(&dicasH, n);
    alocaMemoriaDicas(&dicasV, n);

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++)
            fscanf(fp, "%d", &mz.display[i][j]);
    }

    
    for(int i=0; i<n; i++) fscanf(fp,"%d", &dicasV[i]); //Debugar
    for(int i=0; i<n; i++) fscanf(fp,"%d", &dicasH[i]); //Debugar

    for(int i=0; i<n; i++) printf("%d ", dicasV[i]); //Debugar
    printf("\n"); //Debugar
    for(int i=0; i<n; i++) printf("%d ", dicasH[i]); //Debugar
    printf("\n"); //Debugar



    fscanf(fp, "%d", &marcados);
    printf("\nMARCADOS: %d", marcados); //Debugar

    for(int i=0; i<marcados; i++){
        fscanf(fp, "%d%d", &p, &q);
        mz.status[p][q] = 1;
    }
    
    fscanf(fp, "%d", &removidos);
    for(int i=0; i<removidos; i++){
        fscanf(fp, "%d%d", &p, &q);
        mz.status[p][q] = 0;
    }

    fscanf(fp, "%s", nome);

    fclose(fp);

    sleep(15);
    operador(nome, &mz, dicasH, dicasV, n);


}*/

void carregaSalvo(char nomearquivo[]){
    // *Nota: pode não ser interessante tratar o dado "nomearquivo" aqui dentro
    // *Nota: não tenha medo de expandir o código em prol da legibilidade; este é um digno objetivo junto à resolução do seu problema a ser alcançado

    char arquivo[15], nome[15];
    int n=0, *dicasH, *dicasV, marcados, removidos=0, p, q;
    matriz mz;

    FILE *fp;
    fp = fopen(nomearquivo, "r");

    // n
    fscanf(fp, "%d", &n);

    printf("\nN: %d\n", n); //Debugar

    // "matriz" é um tipo com matrizes dentro? Pode levar à confusão ksksk
    alocaMemoria(&(mz.display), n);
    alocaMemoria(&(mz.gabarito), n);
    alocaMemoria(&(mz.status), n);
    alocaMemoriaDicas(&dicasH, n);
    alocaMemoriaDicas(&dicasV, n);

    zeraStatus(&(mz.status), n);

    // Matriz
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++)
            fscanf(fp, "%d", &mz.display[i][j]);
    }

    // Dicas (vertical)
    for(int i=0; i<n; i++) fscanf(fp,"%d", &dicasV[i]);
    
    // Dicas (horizontal)
    for(int i=0; i<n; i++) fscanf(fp,"%d", &dicasH[i]);

    // Debug (dicas)
    /*for(int i=0; i<n; i++) printf("%d ", dicasV[i]);
    printf("\n");
    for(int i=0; i<n; i++) printf("%d ", dicasH[i]);
    printf("\n");*/

    fscanf(fp, "%d", &marcados);
    //printf("\nMARCADOS: %d\n", marcados); //Debugar

    for (int i=0; i<marcados; i++){
        fscanf(fp, "%d%d", &p, &q);
        mz.status[p][q] = 1;
    }
    
    // *Tem que ler denovo
    //fscanf(fp, "%d", &marcados);

    fscanf(fp, "%d", &removidos);
    for(int i=0; i<removidos; i++){
        fscanf(fp, "%d%d", &p, &q);
        mz.status[p][q] = 0;
    }

    fscanf(fp, "%s", nome);

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++)
            fscanf(fp, "%d", &mz.gabarito[i][j]);
    }

    /*for(int i=0; i<n; i++){
        for(int j=0; j<n; j++)
            printf("%d ", mz.gabarito[i][j]);
        printf("\n");
    }*/
    
    /*printf("\n");
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++)
            printf("%d ", mz.status[i][j]);
        printf("\n");
    }*/

    fclose(fp);

    operador(nome, &mz, dicasH, dicasV, n);


}