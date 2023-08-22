#include "comandos.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "colorcodes.h"
#include <string.h>
#include <unistd.h>

//Função para limpar o terminal
void limpaTerminal(){
    printf("\ec\e[3J");
}

//Função para alocar memória para uma matriz
void alocaMemoria(int ***tabela, int n){
    *tabela = malloc(n * sizeof(int*));

    for(int i=0; i<n; i++){
        (*tabela)[i]= malloc(n * sizeof(int));
    }

}

//Função para alocar memória para um vetor
void alocaMemoriaDicas(int **vetor, int n){
    *vetor = malloc(n * sizeof(int));
}

//Preenche o display com números aleatórios, de acordo com a dificuldade
void preencheDisplay(int ***tabela, int n, char dificuldade){
    int sinal, aux;
    srand(time(NULL)); //Inicializa a semente do rand

    if(dificuldade=='F' || dificuldade == 'f'){
        for(int i=0; i<n; i++)
            for(int j=0; j<n; j++)
                (*tabela)[i][j] = ((rand()%9)+1); //As posições recebem um valor aleatório de 1 a 9

    }else if(dificuldade == 'M' || dificuldade == 'm'){
        for(int i=0; i<n; i++)
            for(int j=0; j<n; j++){
                aux = (rand()%3)+1; //Há uma chance em três de o número sorteado ser negativo
                if(aux==1) sinal=-1; 
                else sinal=1;
                (*tabela)[i][j] = sinal*((rand()%9)+1); //As posições recebem um valor aleatório de -9 a 9, excluso o zero
            }
            
    }else if(dificuldade == 'D' || dificuldade == 'd'){
        for(int i=0; i<n; i++)
            for(int j=0; j<n; j++){
                aux = (rand()%3)+1; //Há uma chance em três de o número sorteado ser negativo
                if(aux==1) sinal=-1;
                else sinal=1;
                (*tabela)[i][j] = sinal*((rand()%10)); //As posições recebem um valor aleatório de -9 a 9, incluso o zero
            }
    }
    
}

//Função para preencher o gabarito com valores aleatórios
void preencheGabarito(int ***tabela, int n, char dificuldade){
    int cont1, cont0, aux;

    srand(time(NULL)); //Inicializa a semente do rand

    if(dificuldade=='F' || dificuldade == 'f'){
        for(int i=0; i<n; i++)
            for(int j=0; j<n; j++){
                aux = (rand()%10)+1; 
                if(aux<5) (*tabela)[i][j] = 1; //Os números tem 50% de chance de serem inicializados em 0 ou 1
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
                    cont1++; //mantém controle de quantos dos números são iguais a 1
                }else{
                    (*tabela)[i][j] = 0;
                    cont0++; //mantém controle de quantos dos números são iguais a 0
                }
                //Caso uma linha seja feita completamente de 0s ou 1s, a função é chamada novamente
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
                    cont1++; //mantém controle de quantos dos números são iguais a 1
                }else{
                    (*tabela)[i][j] = 0;
                }
                //Caso uma linha seja feita completamente de 1s, a função é chamada novamente
                if(cont1 == n) preencheGabarito(tabela, n, dificuldade); 
            }
        }   
    }
}

//Inicializa todas as posições do status com -1
void resetaStatus(int ***tabela, int n){
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            (*tabela)[i][j] = -1;
}

//Imprime uma matriz
void imprime(int ***tabela, int ***status, int **dicasH, int **dicasV, int n){

    printf("\n");

    printf("  "); // Índice superior
    for(int i=0; i<n; i++) printf(TAB_VER"\x1b[33m %-3d \x1b[0m", i+1);
    printf("\n");

    for(int i=0; i<n; i++){
        printf("\x1b[33m%d \x1b[0m", i+1); //Índice da esquerda
        for(int j=0; j<n; j++){
            if((*status)[i][j] == 1) printf(TAB_VER"\x1b[32m %-3d \x1b[0m", (*tabela)[i][j]); //Número mantido, verde
            else if((*status)[i][j] == 0) printf(TAB_VER"\x1b[31m %-3d \x1b[0m", (*tabela)[i][j]); //Número removido, vermelho
            else printf(TAB_VER" %-3d ", (*tabela)[i][j]); //Número sem status, branco
        }
        if(verificaVetor(*dicasV, *tabela, *status, n, i)) printf(TAB_VER"\x1b[1m\x1b[34m %-3d \x1b[0m", (*dicasV)[i]); //Caso a soma da linha esteja correta, é marcada com uma cor mais forte e negrito
        else printf(TAB_VER"\x1b[36m %-3d \x1b[0m", (*dicasV)[i]); // dicas verticais

        printf("\n"); //Pula de uma linha para a outra
    }

    printf("  "); //Espaço na esquerda antes das dicas horizontais
    for(int i=0; i<n; i++){
        if(verificaVetorH(*dicasH, *tabela, *status, n, i)) printf(TAB_VER"\x1b[1m\x1b[34m %-3d \x1b[0m", (*dicasH)[i]); //Caso a soma da coluna esteja correta, é marcada com uma cor mais forte e negrito
        else printf(TAB_VER"\x1b[36m %-3d \x1b[0m", (*dicasH)[i]);  // dicas horizontais
    }
}

//Verifica se uma linha está com as posições mantidas e removidas que atendam à dica
int verificaVetor(int vetor[], int **tabela, int **status, int n, int i){
    int soma = 0;

    //A função utiliza uma soma que avalia se, ignorando os números removidos, é atentida a dica
    for(int j=0; j<n; j++){
        if(status[i][j] != 0) soma += tabela[i][j];
    }

    if(soma==vetor[i]) return 1;
    else return 0;
}

//Verifica se uma coluna está com as posições mantidas e removidas que atendam à dica
int verificaVetorH(int vetor[], int **tabela, int **status, int n, int i){
    int soma = 0;

    //A função utiliza uma soma que avalia se, ignorando os números removidos, é atentida a dica
    for(int j=0; j<n; j++){
        if(status[j][i] != 0) soma += tabela[j][i];
    }

    if(soma==vetor[i]) return 1;
    else return 0;
}


//Realiza a soma dos números da tabela status equivalentes às coordenadas da matriz gabarito e preenche as dicas
void preencheDicas(int ***tabela, int ***gabarito, int **vetor, int n, char c[2]){

    //Em ambos os casos, é percorrida uma linha ou coluna de cada vez, zerando-a ao início e somando os elementos devidos
    //Calculo para o vetor vertial
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

//Faz a liberação de uma matriz
void liberaMatrizes(int ***tabela, int n){

    for(int i=0; i<n; i++)
            free((*tabela)[i]);
        free(*tabela);
}

//Faz a liberação de um vetor
void liberaVetores(int **vetor, int n){
    free(*vetor);
}

//Realiza a limpeza do buffer
void limpaBuffer(){
    fflush(stdin);
}

//Realiza a limpeza do buffer de outra maneira
void limpaBuffer2(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

//Atualiza o status de acordo com o comando decidido no Operador()
void mudaStatus(int ***tabela, int i, int j, int operacao){
    switch(operacao){
        case 1:
            (*tabela)[i][j] = 1; //Marca a posição como mantida
            break;
        case 0:
            (*tabela)[i][j] = 0; //Marca a posição como removida
            break;
        default: printf(BOLD(RED("\nErro na operação")));
            break;
    }
}

//Função para criar um arquivo .txt com os dados do jogo
void salvaJogo(char nome[], char nomearquivo[], int ***display, int ***gabarito, int ***status, int **dicasH, int **dicasV, int n, double tempoTotal){
    int mantidos=0, removidos=0;
    FILE *fp, *bf;
    fp = fopen(nomearquivo, "w");

    //É criado um arquivo auxiliar para salvar o gabarito em formato binário
    char nomebinario[25];
    strcpy(nomebinario, nomearquivo);
    nomebinario[strlen(nomearquivo)-4] = '\0';
    strcat(nomebinario, ".dat\0");

    bf = fopen(nomebinario, "wb");

    //A matriz gabarito é salva em formato binário
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fwrite(&(*gabarito)[i][j], sizeof(int), 1, bf);
        }
    }
    fclose(bf);
    
    fprintf(fp, "%d\n", n); //Dimensão n
    //Matriz display
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++)
            fprintf(fp, "%d ", (*display)[i][j]);
        fprintf(fp, "\n");
    }

    //Somas Verticais e Horizontais
    for(int i=0; i<n; i++) fprintf(fp,"%d ", (*dicasV)[i]);
    fprintf(fp, "\n");
    for(int i=0; i<n; i++) fprintf(fp, "%d ", (*dicasH)[i]);
    fprintf(fp, "\n");


    //Calcula quantas posições foram mantidas e removidas
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if((*status)[i][j] == 1) mantidos++;
            if((*status)[i][j] == 0) removidos++;
        }
            
    }

    //Imprime quantos foram mantidos e suas respectivas coordenadas
    fprintf(fp, "%d\n", mantidos);
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++)
            if((*status)[i][j] == 1) fprintf(fp, "%d %d\n", i, j);
    }

    //Imprime quantos foram removidos e suas respectivas coordenadas
    fprintf(fp, "%d\n", removidos);
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++)
            if((*status)[i][j] == 0) fprintf(fp, "%d %d\n", i, j);
    }

    //Imprime o nome do jogador
    fprintf(fp, "%s\n", nome);

    //Imprime o tempo total do jogador
    fprintf(fp, "%.0lf", tempoTotal);
    

    fclose(fp);

}

//Dá uma dica ao jogador
void dica(int ***gabarito, int ***status, int n, int flag){
    //O uso da variável flag garante que só possa ser utilizada uma vez por sessão, mas é resetada ao retornar ao menu
    if(flag){
        /*O código percorre toda a matriz até encontrar uma posição marcada no gabarito que ainda não esteja mantida no status, e então a marca*/
        for(int i=0; i<n; i++)
            for(int j=0; j<n; j++){
                if((*gabarito)[i][j] == 1 && ((*status)[i][j]!=1)){
                    (*status)[i][j] = 1;
                    return;
                }
            }
        }else{
            printf(BLUE("Você já recebeu uma dica :)\n"));
            sleep(1);
        } 

}


//O código percorre toda a matriz alterando o valor do status das posições de acordo com seus valores na matriz gabarito
void resolve(int ***gabarito, int ***status, int n){
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++){
            if((*gabarito)[i][j] == 1 && (*status)[i][j]!=1)
                (*status)[i][j] = 1;
            else if((*gabarito)[i][j] == 0 && (*status)[i][j]!=0)
                (*status)[i][j] = 0;
        }
}

//É chamada a cada iteração do jogo, e verifica se o jogador atingiu as condições necessárias à vitória
int verificaStatus(int **gabarito, int **status, int n){
    int flag = 0;

    //A função percorre toda a matriz status, validando as posições de acordo com a matriz gabarito
    //Apenas remover as posições que não entram na soma já garante a Vitória
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++){
            if((gabarito[i][j] == 1 && status[i][j] == 1) || (gabarito[i][j] == 1 && status[i][j] == -1))
                //Posições marcadas com 1 no gabarito que tenham sido mantidas ou não utilizadas no status são válidas
                flag++;
            else if(gabarito[i][j] == 0 && status[i][j]==0)
                //Posições marcadas com 0 no gabarito que tenham sido removidas no status são válidas
                flag++;    
        }

    //Se todas as posições forem válidas, o jogador vence
    if(flag==n*n){
        return 1;
    }else{
        return 0;
    }
}

//Imprime a tela de vitória, com o nome do jogador e seu tempo
void telaVitoria(char nome[], int ***tabela, int ***gabarito, int *dicasH, int *dicasV, int n, double tempoTotal){
    char flag[10] = "";
    int tam;
    Ranking rk;

    
    while(strcmp(flag, "voltar") != 0){
        limpaTerminal();

        printf(BOLD(GREEN("Parabéns %s, você concluiu o jogo!!\nTempo de jogo: %0.lf segundos\n")), nome, tempoTotal);

        rk = atualizaRanking(tempoTotal, nome, n);
        imprimeRanking(rk, 't', n); //Impriem o ranking no terminal
        imprimeRanking(rk, 'a', n); //Imprime o ranking no arquivo

        //Enquanto o jogador não entrar o comando "voltar" a função fica presa em loop
        printf("\nDigite 'voltar' para retornar ao menu principal: ");
        fgets(flag, 10, stdin);
        
        tam = strlen(flag);
        flag[tam-1] = '\0';

        if(!strcmp(flag, "voltar")) return;
    }

}

//
Ranking atualizaRanking(double tempo, char nome[TAMNOME], int n){
    Ranking rk, auxrk;
    rk = leRanking();
    auxrk = rk;

    //Se não há jogadores registrados naquela dimensão de tabuleiro, é inserido na primeira posição
    if(rk.gap[n-3].qtd == 0){
        rk.gap[n-3].jogadores[0].tempo = tempo;
        strcpy(rk.gap[n-3].jogadores[0].nome, nome);
        if(rk.gap[n-3].qtd < 5) rk.gap[n-3].qtd++;
        return rk;
    }

    //Se a quantidade de jogadores inseridos na posição for menor que cinco, a qtd é incrementada
    if(rk.gap[n-3].qtd < 5) rk.gap[n-3].qtd++;

    //Em teoria, insere a jogada mais recente na posição correta, ao concluir um jogo. Funciona parcialmente
    for(int i = 0; i<rk.gap[n-3].qtd; i++){
        //Primeiro, encontra onde o jogador deve ser inserido
        if(tempo<rk.gap[n-3].jogadores[i].tempo){
            rk.gap[n-3].jogadores[i].tempo = tempo;
            strcpy(rk.gap[n-3].jogadores[i].nome, nome);

            //Depois, move todos os demais jogadores uma posição acima
            for(int j=rk.gap[n-3].qtd - 1; j>i; j--){
                rk.gap[n-3].jogadores[j].tempo = auxrk.gap[n-3].jogadores[j-1].tempo;
                strcpy(rk.gap[n-3].jogadores[j].nome, auxrk.gap[n-3].jogadores[j-1].nome);
            }
            return rk;
        }
    }
    return rk;
}

//Imprime o ranking no terminal ou no arquivo
void imprimeRanking(Ranking rk, char op, int n){
    char flag[10] = "";
    int tam;

        //Impressão no terminal
        if(op=='t'){
            for(int i=0; i<QTDGAPS; i++){
                if(rk.gap[i].qtd == 0) continue;
                printf("\nSize = %d\n", rk.gap[i].tam); /*Caso a quantidade de jogadores de uma determinada dimensão seja maior que zero, 
                é criada a nova categoria na impressão*/

                for(int j=0; j<rk.gap[i].qtd; j++){
                    printf("\tNome%d = %s\n\tTempo%d = %0.lf\n", j+1, rk.gap[i].jogadores[j].nome, j+1, rk.gap[i].jogadores[j].tempo);
                }
            }

        }else{
            FILE *fp = fopen("sumplete.ini", "w");
            for(int i=0; i<QTDGAPS; i++){
                if(rk.gap[i].qtd == 0) continue;
                fprintf(fp, "size = %d\n", rk.gap[i].tam); /*Caso a quantidade de jogadores de uma determinada dimensão seja maior que zero, 
                é criada a nova categoria na impressão*/

                for(int j=0; j<rk.gap[i].qtd; j++){
                    fprintf(fp, "player%d = %s\ntime%d = %0.lf\n", j+1, rk.gap[i].jogadores[j].nome, j+1, rk.gap[i].jogadores[j].tempo);
                }
            }
            fclose(fp);

            return;    
    }
}


//Le o ranking do arquivo "sumplete.ini"
Ranking leRanking(){
    FILE *fp;
    Ranking rk;
    int size;
    char palavra[TAMPALAVRA], palavraauxiliar[TAMPALAVRA];

    if((fp = fopen("sumplete.ini", "r"))==NULL){
        fp = fopen("sumplete.ini", "w");
    }

    //Zera as posições da struct onde será armazenado o ranking
    for (int i = 0; i < 7; i++) {
        rk.gap[i].qtd = 0;
        rk.gap[i].tam = i + 3;
    }

    //Começa a leitura
    fscanf(fp, "%s", palavra);

    //O primeiro for percorre cada categoria, caso exista
    for(int i=0; i<7 || !feof(fp); i++){
        
        //Elimina o = e captura o tamanho da dimensão
        fscanf(fp, "%s", palavra);  // =
        fscanf(fp, "%d", &size);

        //Percorre o arquivo até encontrar o próximo "size", incrementando a cada jogador encontrado
        int j = 0;
        for(j=0; j<MAXJOGADORES; j++){
            fscanf(fp, "%s", palavra); //player ou size

            //Se o primeiro caracter for 'p', a quantidade de jogadores nesse tamanho acabou
            if(palavra[0] != 'p') {
                break;
            }   
                
            fgets(palavra, 4, fp); //Lê e elimina os caracteres entre "player" e seu nome
            fgets(palavra, TAMPALAVRA, fp); //Lê o nome do jogador

            palavra[strlen(palavra) - 1] = '\0'; //Adiciona '\0' ao final do nome do jogador
            strcpy(rk.gap[size-3].jogadores[j].nome, palavra); //Salva o nome do jogador na struct
            
            fscanf(fp, "%s", palavra); //Elimina "time"
            fscanf(fp, "%s", palavra); //Elimina "="
            fscanf(fp, "%lf", &rk.gap[size-3].jogadores[j].tempo); //Lê o tempo de jogo
            if(rk.gap[size-3].qtd<5) rk.gap[size-3].qtd++; //Se a quantidade de jogadores daquele tamanho for menor que cinco, é incrementado
        }
    }

    fclose(fp);
    return rk;
}

//Verifica se o formato de arquivo de salvamento é válido
int verificaFormato(char nome[25]){
    char aux[5];
    int tam, cont=3;
    tam = strlen(nome);

    //Captura os últimos quatro caracteres da string
    for(int i=tam-1; i>tam-4-1 && cont >= 0; i--){
        aux[cont] = nome[i];
        cont --;
    }

    aux[4] = '\0'; //Adiciona '\0' ao final da string

    if(strcmp(aux, ".txt")==0) return 1; //Verifica se o formato é válido e retorna 1 caso seja
    else return 0;
}

//Título do jogo
void titulo(){

    printf(MAGENTA(
         "  ███████╗██╗   ██╗███╗   ███╗██████╗ ██╗     ███████╗████████╗███████╗\n"
         "  ██╔════╝██║   ██║████╗ ████║██╔══██╗██║     ██╔════╝╚══██╔══╝██╔════╝\n"
         "  ███████╗██║   ██║██╔████╔██║██████╔╝██║     █████╗     ██║   █████╗  \n"
         "  ╚════██║██║   ██║██║╚██╔╝██║██╔═══╝ ██║     ██╔══╝     ██║   ██╔══╝  \n"
         "  ███████║╚██████╔╝██║ ╚═╝ ██║██║     ███████╗███████╗   ██║   ███████╗\n\n")); 

         sleep(3);
}