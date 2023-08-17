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
void operador(char nome[], matriz *mz, int *dicasH, int *dicasV, int n, double);
void carregaSalvo(char nomearquivo[]);

int main(){
    int data = 0;
    printf("\ec\e[3J");

    //titulo();

    FILE *fp = fopen("gamedata.dat", "wb");

    fwrite(&data, sizeof(int), 1, fp);
    fclose(fp);

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
    int n=0, *dicasH, *dicasV, data=1;
    char tamt;
    matriz mz;
    double tempoTotal = 0;


    FILE *fp = fopen("gamedata.dat", "wb");
    fwrite(&data, sizeof(int), 1, fp);
    fclose(fp);


    printf("\ec\e[3J");
    limpaBuffer();
    printf("Digite o nome do jogador 1: ");
    scanf("%s", nome);
    limpaBuffer2();
    printf("\nDigite o tamanho do tabuleiro (3 a 9): ");
    scanf("%c", &tamt);
    n = atoi(&tamt);
    

    while(n<3 || n>9){
        if(n<3 || n>9){
            printf(RED("Tamanho inválido. Digite novamente: "));
            limpaBuffer2();
            scanf("%c", &tamt);
            n = atoi(&tamt);
        }
    }

    limpaBuffer2();

    if(n > 6){
        while(1){
            printf("\nDigite o nível de dificuldade (Fácil, Médio ou Difícil): ");
            scanf("%c", &dificuldade);
            getchar();
            if(dificuldade == 'F' || dificuldade == 'f' || dificuldade == 'M' || dificuldade == 'm' || dificuldade == 'D' || dificuldade == 'd') break;
        else printf("\nOpção de dificuldade inválida. Digite novamente: ");
        }
    }else if(n>4){
        while(1){
            printf("\nDigite o nível de dificuldade (Fácil ou Médio): ");
            scanf("%c", &dificuldade);
            getchar();
            if(dificuldade == 'F' || dificuldade == 'f' || dificuldade == 'M' || dificuldade == 'm') break;
        else printf("\nOpção de dificuldade inválida. Digite novamente: ");
        }
    }else dificuldade = 'F';
    
    
    printf("\n\n");


    alocaMemoria(&(mz.display), n);
    alocaMemoria(&(mz.gabarito), n);
    alocaMemoria(&(mz.status), n);
    alocaMemoriaDicas(&dicasH, n);
    alocaMemoriaDicas(&dicasV, n);

    preencheDisplay(&(mz.display), n, dificuldade);
    preencheGabarito(&(mz.gabarito), n, dificuldade);
    zeraStatus(&(mz.status), n);

    preencheDicas(&(mz.display), &(mz.gabarito), &dicasV, n, "v\0");
    preencheDicas(&(mz.display), &(mz.gabarito), &dicasH, n, "h\0");


    operador(nome, &mz, dicasH, dicasV, n, tempoTotal);
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

void operador(char nome[], matriz *mz, int *dicasH, int *dicasV, int n, double tempoTotal){
    char leitura[50], comando[12], num[3], c;
    int x=11, i, j, pos=0, auxpos=0, flag=1;
    time_t tempoInicio, tempoFinal;
    time(&tempoInicio);
    limpaBuffer();

    criaRanking(nome, tempoTotal, n);

    

    while(1){
        //printf("\ec\e[3J");

        if(verificaStatus(mz->gabarito, mz->status, n)){   
            time(&tempoFinal);
            tempoTotal += difftime(tempoFinal, tempoInicio);


            telaVitoria(nome, &(mz->display), &(mz->gabarito), dicasH, dicasV, n, tempoTotal);
            liberaMatrizes(&(mz->display), n);
            liberaMatrizes(&(mz->gabarito), n);
            liberaMatrizes(&(mz->status), n);
            liberaVetores(&dicasH, n);
            liberaVetores(&dicasV, n);
            menu();
        }

        printf(BOLD(BLUE("+ o + !!SUMPLETE!! o + o \n")));
        imprime(&(mz->display), &(mz->status), &dicasH, &dicasV, n);
        printf("\n");
        imprime(&(mz->gabarito), &(mz->status), &dicasH, &dicasV, n);
        //printf("\n");
        imprime(&(mz->status), &(mz->status), &dicasH, &dicasV, n);
        limpaBuffer();
        printf("\n%s, digite o comando: ", nome);

        //Leitura do comando
        fgets(leitura, 50, stdin);

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

        printf("\nCOMANDO: |%s|\n", comando);
        char nomearquivo[15] = "a";
        if(!strcmp(comando, "salvar")){
            auxpos=0;
            while(pos < tam){
                printf("\nEntrou na comparação");
                nomearquivo[auxpos] = leitura[pos+1];
                pos++;
                auxpos++;
            }
            nomearquivo[auxpos-2] = '\0';
            strcat(nomearquivo, ".txt");
        }
        


        
    
        
        if(!strcmp(comando, "manter")){
            mudaStatus(&(mz->status), i-1, j-1, 1);
        }else if(!strcmp(comando, "remover")){
            mudaStatus(&(mz->status), i-1, j-1, 0);
        }else if(!strcmp(comando, "voltar")){
            time(&tempoFinal);
            tempoTotal += difftime(tempoFinal, tempoInicio);
            salvaJogo(nome, "temporario.txt", &(mz->display), &(mz->gabarito), &(mz->status), &dicasH, &dicasV, n, tempoTotal);
            liberaMatrizes(&(mz->display), n);
            liberaMatrizes(&(mz->gabarito), n);
            liberaMatrizes(&(mz->status), n);
            liberaVetores(&dicasH, n);
            liberaVetores(&dicasV, n);
            menu();
            return;
        }else if(!strcmp(comando, "salvar")){
            printf("\n\nEntrou em salvaJogo\n");
            time(&tempoFinal);
            tempoTotal += difftime(tempoFinal, tempoInicio);
            salvaJogo(nome, nomearquivo, &(mz->display), &(mz->gabarito), &(mz->status), &dicasH, &dicasV, n, tempoTotal);
        }else if(!strcmp(comando, "dica")){
            dica(&(mz->gabarito), &(mz->status), n, flag);
            flag=0;
        }else if(!strcmp(comando, "resolver")){
            resolve(&(mz->gabarito), &(mz->status), n);
            printf("\nEntrou em resolver :) \n");
        }
    }

}

void menu(){
    int flag;
    char nomearquivo[25], op;
    FILE *fp = fopen("gamedata.dat", "rb");
    
    
    /*FILE *ini = fopen("sumplete.ini", "w");

    for(int i=3; i<10; i++)
        fprintf(ini, "size = %d\n\n", i);*/
    
    leRanking();

    fread(&flag, sizeof(int), 1, fp);
    fclose(fp);

    limpaTerminal();
    printf(BLUE("+ o + !!Bem vindo ao jogo SUMPLETE!! o + o "));

    printf("\n\n0. Sair do jogo\n1. Começar um novo jogo\n2. Continuar um jogo salvo em arquivo\n3. Continuar o jogo atual\n4. Exibir o ranking\nDurante o jogo, digite 'voltar' para retonar a esse menu.\n");
    printf("\n\nEscolha uma opção: ");

    
    scanf("%c", &op);

    while(1){
        switch(op){
        case '0':
            exit(0);
        case '1':
            inicializar();
            flag = 1;
            break;
        case '2':
            limpaBuffer();
            printf("\nEntre o nome do arquivo: ");
            fgets(nomearquivo, 25, stdin);
            nomearquivo[strlen(nomearquivo)-1] = '\0';
            carregaSalvo(nomearquivo);
            break;
        case '3':
            if(flag){
                carregaSalvo("temporario.txt");
                break;
            }else{
                printf("\nVocê ainda não iniciou um jogo.\nEscolha novamente: ");
                sleep(2);
                menu();
                break;
            }   
        case '4':
            break;
        default:
            limpaBuffer();
            printf("\rComando não reconhecido. Digite novamente: ");
            scanf("%c", &op);
            break;
        }    
    }

    

}


void carregaSalvo(char nomearquivo[]){
    // *Nota: pode não ser interessante tratar o dado "nomearquivo" aqui dentro
    // *Nota: não tenha medo de expandir o código em prol da legibilidade; este é um digno objetivo junto à resolução do seu problema a ser alcançado

    char arquivo[15], nome[15];
    int n=0, *dicasH, *dicasV, marcados, removidos=0, p, q;
    matriz mz;
    double tempoTotal;

    FILE *fp;
    fp = fopen(nomearquivo, "r");

    if(fp==NULL){
        printf("Arquivo inexistente.\n");
        sleep(2);
        menu();
    }

    // n
    fscanf(fp, "%d", &n);

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

    fscanf(fp, "%d", &marcados);

    for (int i=0; i<marcados; i++){
        fscanf(fp, "%d%d", &p, &q);
        mz.status[p][q] = 1;
    }

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

    fscanf(fp, "%lf", &tempoTotal);

    fclose(fp);

    operador(nome, &mz, dicasH, dicasV, n, tempoTotal);

}