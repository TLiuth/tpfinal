/*
Trabalho Final Introdução à Programação 2023/1
Professor: Puca Huachi V. Penna
Aluno: Thiago Ayolphi Liuth
Matrícula: 2314005
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comandos.h"
#include "colorcodes.h"
#include <time.h>
#include <unistd.h>

//Struct usada para as matrizes principais do jogo
typedef struct 
{
    int **display; //Matriz com os números que serão impressos aos jogadores
    int **gabarito; //Matriz com a combinação de valores Os e 1s que definem quais valores entram na contagem
    int **status; //Matriz que armazena o estado definido pelo jogador para a posição (mantido ou removido)
}matriz;

void menu();
void inicializar();
void operador(char nome[], matriz *mz, int *dicasH, int *dicasV, int n, double);
void carregaSalvo(char nomearquivo[]);

int main(){

    int flag = 0; //Valor auxiliar para definição de se há jogo a ser continuado ou não
    FILE *fp = fopen("gamedata.dat", "wb");
    fwrite(&flag, sizeof(int), 1, fp); //O valor auxiliar é armazenado em um arquivo binário
    fclose(fp);

    limpaTerminal(); //Limpa o espaço do terminal

    titulo(); //Função título de abertura do jogo

    menu(); //Menu principal do jogo

    return 0;
}

void inicializar(){
    //Declarção de variáveis
    char nome[TAMNOME], dificuldade;
    int n=0, *dicasH, *dicasV, flag=1; //n = dimensão da matriz, dicas armazenam as somas das linhas/colunas, flag = variável auxiliar que indica se há jogo a ser continuado
    char tamt; //Tamanho do tabuleiro em formato de char
    matriz mz; // Struct com as três matrizes principais do jogo
    double tempoTotal = 0; // Tempo total de jogo
    
    //Armazena a variável auxiliar indicando que agora existe um jogo iniciado que pode ser continuado
    FILE *fp = fopen("gamedata.dat", "wb");
    fwrite(&flag, sizeof(int), 1, fp);
    fclose(fp);



    printf("Digite o nome do jogador 1: "); //Entra o nome do jogador
    fgets(nome, TAMNOME, stdin); //Leitura do nome do jogador

    //O nome deve ser composto por no mínimo um caracter válido. Um espaço ou quebra de linha não são válidos
    while(nome[0] == '\n' || nome[0] == ' '){
        printf("\nEsse nome não é válido. Tente outro nome: ");
        fgets(nome, TAMNOME, stdin);
    }
    nome[strlen(nome)-1] = '\0';


    printf("\nDigite o tamanho do tabuleiro (3 a 9): "); //Dimensão do tabuleiro lido em formato char
    scanf("%c", &tamt); 
    n = atoi(&tamt); //Transforma o char da dimensão em int
    
    //Garante que o tamanho entrado seja válido
    while(n<3 || n>9){
        if(n<3 || n>9){
            limpaBuffer2();
            printf(RED("Tamanho inválido. Digite novamente: "));
            scanf("%c", &tamt);
            n = atoi(&tamt);
        }
    }

    limpaBuffer2();

    //Nos casos em que seja necessário, as condições abaixo pedem ao jogador para que faça a escolha da dificulde e armazena seu valor
    if(n > 6){
        while(1){
            printf("\nDigite o nível de dificuldade (Fácil, Médio ou Difícil): ");
            scanf("%c", &dificuldade);
            getchar();
            if(dificuldade == 'F' || dificuldade == 'f' || dificuldade == 'M' || dificuldade == 'm' || dificuldade == 'D' || dificuldade == 'd') break;
            else{
                printf("\nOpção de dificuldade inválida.");
                limpaBuffer2();;
            }
        }
    }else if(n>4){
        while(1){
            printf("\nDigite o nível de dificuldade (Fácil ou Médio): ");
            scanf("%c", &dificuldade);
            getchar();
            if(dificuldade == 'F' || dificuldade == 'f' || dificuldade == 'M' || dificuldade == 'm') break;
            else{
                printf("\nOpção de dificuldade inválida.");
                limpaBuffer2();
            }
        }
    }else dificuldade = 'F'; //Tabuleiros com dimensão 3 e 4 sempre terão a dificuldade definida como fácil
    
    
    printf("\n\n");

    //Todas as entradas realizadas, é feita a alocação dinâmica das matrizes e vetores de acordo com a dimensão definida
    alocaMemoria(&(mz.display), n);
    alocaMemoria(&(mz.gabarito), n);
    alocaMemoria(&(mz.status), n);
    alocaMemoriaDicas(&dicasH, n);
    alocaMemoriaDicas(&dicasV, n);

    //O display é preenchido com valores aleatórios de acordo com as definições da dificuldade
    preencheDisplay(&(mz.display), n, dificuldade);
    //O gabarito é preenchido com 0s e 1s de acordo com as definições da dificuldade
    preencheGabarito(&(mz.gabarito), n, dificuldade);
    //O tabuleiro status tem todas as suas posições definidas para -1
    resetaStatus(&(mz.status), n);

    //Preenche os dois vetores das dicas com suas somas, de acordo com a matriz gabarito
    preencheDicas(&(mz.display), &(mz.gabarito), &dicasV, n, "v\0");
    preencheDicas(&(mz.display), &(mz.gabarito), &dicasH, n, "h\0");

    //Chama a função operador, que faz todo o controle dos comandos do jogo
    operador(nome, &mz, dicasH, dicasV, n, tempoTotal);

}

void operador(char nome[], matriz *mz, int *dicasH, int *dicasV, int n, double tempoTotal){
    char leitura[50], comando[12], num[3]; //Leitura pega a frase inteira q for digitada no terminal, comando armazena o comando após isolado, num mantém a string do número antes de ser transformada em inteiro
    int x=11, i, j, pos=0, auxpos=0, flag=1; //x armazena a posição alvo. É iniciada como 11 por motivos de validação.
    //Pos e auxpos são usadas para a aritmética na manipulação das strings. Flag só permite uma dica por sessão
    time_t tempoInicio, tempoFinal; //Variáveis de tempo de início e de fim da partida
    time(&tempoInicio); //O primeiro tempo é pego, dando início à contagem
    limpaBuffer();


    while(1){
        limpaTerminal();

        //A cada iteração do while é chamada a função de verificação de status, para avaliar se já pode ser declarada vitória ao jogador
        if(verificaStatus(mz->gabarito, mz->status, n)){   
            time(&tempoFinal); //Pega o tempo no momento da vitória
            tempoTotal += difftime(tempoFinal, tempoInicio); //Calcula o tempo discorrido

            //É chamada a tela de vitória, que imprime o nome do jogador e o tempo que ele levou para completar o jogo
            telaVitoria(nome, &(mz->display), &(mz->gabarito), dicasH, dicasV, n, tempoTotal);
            //É feita a liberação das matrizes e vetores, e o jogo volta ao menu
            liberaMatrizes(&(mz->display), n);
            liberaMatrizes(&(mz->gabarito), n);
            liberaMatrizes(&(mz->status), n);
            liberaVetores(&dicasH, n);
            liberaVetores(&dicasV, n);
            menu();
        }

        //Imprime um título
        printf(BOLD(BLUE("+ o + !!SUMPLETE!! o + o \n")));
        //Imprime a matriz com os valores de display
        imprime(&(mz->display), &(mz->status), &dicasH, &dicasV, n);
        printf("\n");

        limpaBuffer();
        printf("\n%s, digite o comando: ", nome); 

        //Leitura do comando
        fgets(leitura, 50, stdin);

        //aritmética para saber a posição de separação entre o comando e a posição, em caso dos comandos manter ou voltar
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
        if(!strcmp(comando, "manter") || (!strcmp(comando, "remover")) || !strcmp(comando, "MANTER") || !strcmp(comando, "REMOVER")){
            for(int j=0; j<2; j++){
                pos++;
                num[j] = leitura[pos];
                printf("%s", num);
            }
            num[2] = '\0';
            //O inteiro é então dividido em duas partes (linha e coluna)
            x = atoi(num);
            j = x%10;
            i = x/10;   
        }
        
        //posições que não abrangem o tamanho do tabuleiro são inválidas
        if(x < 11 || x>((10*n)+n)){
            printf(BOLD(RED("\nPosição inválida.\n")));
            sleep(2);
            operador(nome, mz, dicasH, dicasV, n, tempoTotal);
            continue;
        }

        //No caso do comando "salvar" é separado agora o nome do arquivo alvo
        char nomearquivo[TAMNOME] = "a";
        if(!strcmp(comando, "salvar") || !strcmp(comando, "SALVAR")){
                auxpos=0;
                while(pos < tam){
                    nomearquivo[auxpos] = leitura[pos+1];
                    pos++;
                    auxpos++;
                }
                nomearquivo[auxpos-2] = '\0';
 
            }
            
        //Aqui são chamadas as funções de operação separadamente
        if(!strcmp(comando, "manter") || !strcmp(comando, "MANTER")){
            mudaStatus(&(mz->status), i-1, j-1, 1);

        }else if(!strcmp(comando, "remover") || !strcmp(comando, "REMOVER")){
            mudaStatus(&(mz->status), i-1, j-1, 0);

        }else if(!strcmp(comando, "voltar") || !strcmp(comando, "VOLTAR")){
            //Ao voltar ao menu, o tempo total já discorrido é armazenado no arquivo temporário
            time(&tempoFinal);
            tempoTotal += difftime(tempoFinal, tempoInicio);
            printf("\nFLAG 1 VOLTAR");
            //O arquivo é salvo em "temporario.txt", e as matrizes e vetores são liberados
            salvaJogo(nome, "temporario.txt", &(mz->display), &(mz->gabarito), &(mz->status), &dicasH, &dicasV, n, tempoTotal);
            printf("\nFLAG 2 VOLTAR");
            liberaMatrizes(&(mz->display), n);
            liberaMatrizes(&(mz->gabarito), n);
            liberaMatrizes(&(mz->status), n);
            liberaVetores(&dicasH, n);
            liberaVetores(&dicasV, n);
            menu();

        }else if(!strcmp(comando, "salvar") || !strcmp(comando, "SALVAR")){
            //É verificado o formato do arquivo definido como arquivo final. Caso seja inválido, deve ser entrado novamente
            while(!verificaFormato(nomearquivo)){
                printf("\nNome de arquivo inválido. Entre o nome do arquivo novamente: ");
                limpaBuffer();
                scanf("%s", nomearquivo);
            }

            //O tempo já discorrido é captado e armazenado juntamente ao arquivo
            time(&tempoFinal);
            tempoTotal += difftime(tempoFinal, tempoInicio);
            salvaJogo(nome, nomearquivo, &(mz->display), &(mz->gabarito), &(mz->status), &dicasH, &dicasV, n, tempoTotal);

        }else if(!strcmp(comando, "dica") || !strcmp(comando, "DICA")){
            //Chama a função que dá uma dica ao jogador. Só pode ser usada uma vez por sessão
            dica(&(mz->gabarito), &(mz->status), n, flag);
            flag=0;

        }else if(!strcmp(comando, "resolver") || !strcmp(comando, "RESOLVER")){
            //Chama a função que resolve o código
            resolve(&(mz->gabarito), &(mz->status), n);
        }
    }

}

void menu(){
    char nomearquivo[TAMNOME], op; //a variável op armazena o valor da operação a ser realizada

    int flag; //Variável auxiliar para saber se existe jogo a ser continuado
    FILE *fp = fopen("gamedata.dat", "rb");
    fread(&flag, sizeof(int), 1, fp);
    fclose(fp);

    limpaTerminal();
    printf(BLUE("+ o + !!Bem vindo ao jogo SUMPLETE!! o + o "));

    //São impressas as opções de ações a serem realizadas
    printf("\n\n0. Sair do jogo\n1. Começar um novo jogo\n2. Continuar um jogo salvo em arquivo\n3. Continuar o jogo atual\n4. Exibir o ranking\nDurante o jogo, digite 'voltar' para retonar a esse menu.\n");
    printf("\n\nEscolha uma opção: ");

    //leitura do operador
    scanf("%c", &op);

    

    while(1){
        switch(op){
        case '0': //Encerra o programa
            exit(0);
        case '1': //Inicializa um novo jogo
            limpaBuffer2();
            inicializar();
            flag = 1;
            break;
        case '2': //Carrega um jogo salvo
            limpaBuffer2();
            printf("\nEntre o nome do arquivo: ");
            fgets(nomearquivo, TAMNOME, stdin);
            nomearquivo[strlen(nomearquivo)-1] = '\0';
            //limpaBuffer2();
            carregaSalvo(nomearquivo);
            break;
        case '3': //Continua o jogo em andamento, caso exista um
            if(flag){
                carregaSalvo("temporario.txt");
                break;
            }else{
                // Caso não exista jogo em andamento, o jogador é avisado disso
                printf("\nVocê ainda não iniciou um jogo.\nEscolha novamente: ");
                sleep(2);
                menu();
                break;
            }   
        case '4': //Imprime o ranking
            Ranking rk = leRanking();
            imprimeRanking(rk, 't', 3);
            //leitura de novo operador para sair do loop
            limpaBuffer2();
            printf("\n\nEscolha uma opção: ");
            //leitura do operador
            scanf("%c", &op);
            break;
        default: //Caso o comando seja inválido, é requisitado novamente
            limpaBuffer2();
            printf("Comando não reconhecido. Digite novamente: ");
            scanf("%c", &op);
            break;
        }    
    }

    

}

//Função para carregar jogo salvo em arquivo. É chamada tanto para jogos deliberadamente salvos pelo jogador, quanto para resumir jogos em andamento
void carregaSalvo(char nomearquivo[]){

    char nome[TAMNOME];
    int n=0, *dicasH, *dicasV, marcados, removidos=0, p, q; //p e q leem e armazenam coordenadas
    matriz mz;
    double tempoTotal;

    FILE *fp;
    fp = fopen(nomearquivo, "r");

    
    //Caso não exista o arquivo destino, a função retorna ao menu
    if(fp==NULL){
        printf("\nArquivo inexistente.\n");
        menu();
    }

    // n
    fscanf(fp, "%d", &n);

    alocaMemoria(&(mz.display), n);
    alocaMemoria(&(mz.gabarito), n);
    alocaMemoria(&(mz.status), n);
    alocaMemoriaDicas(&dicasH, n);
    alocaMemoriaDicas(&dicasV, n);

    resetaStatus(&(mz.status), n);

    // Matriz
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++)
            fscanf(fp, "%d", &mz.display[i][j]);
    }

    // Dicas (vertical)
    for(int i=0; i<n; i++) fscanf(fp,"%d", &dicasV[i]);
    
    // Dicas (horizontal)
    for(int i=0; i<n; i++) fscanf(fp,"%d", &dicasH[i]);

    //Número e coordenadas das posições marcadas, salvas em p e q
    fscanf(fp, "%d", &marcados);
    for (int i=0; i<marcados; i++){
        fscanf(fp, "%d%d", &p, &q);
        mz.status[p][q] = 1;
    }
    
    //Número e coordendas das posições removidas
    fscanf(fp, "%d", &removidos);
    for(int i=0; i<removidos; i++){
        fscanf(fp, "%d%d", &p, &q);
        mz.status[p][q] = 0;
    }

    //Carrega o gabarito
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++)
            fscanf(fp, "%d", &mz.gabarito[i][j]);
    }

    //Nome do jogador
    fgets(nome, TAMNOME, fp);
    fgets(nome, TAMNOME, fp);
    nome[strlen(nome)-1] = '\0';

    //Tempo total já discorrido
    fscanf(fp, "%lf", &tempoTotal);

    fclose(fp);

    //Chama a função operador() para resumir o jogo
    operador(nome, &mz, dicasH, dicasV, n, tempoTotal);

}