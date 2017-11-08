#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void imprimirTabela ();
void adicionarPalavra (char *palavra);
void removerPalavra (char *palavra);
int buscarPalavra (char *palavra);
void verificarComando (char *palavra);
int calcularHash (int palavra);
int transformarPalavra (char *palavra);
int algoritmos (char *palavra);
int letraMais (char *palavra);
int letraMenos (char *palavra);
int letraTrocada (char *palavra);
int letraErrada (char *palavra);
void realocarTabela ();

typedef struct {
    int chave;
	char palavras[100];
} Tabela;

char linha [100];
char ultimaPalavra [100];
int tamanhoTabela = 50;
int qtdElementos = 0;
int fatorCarga = 0;
Tabela *tabela;


int main() {
    /**
        Aloca mem�ria para a tabela
    **/
    tabela = (Tabela*) calloc (tamanhoTabela, sizeof(Tabela));
    while ((scanf(" %s", linha)) && (strcmp(linha, "*") != 0)) {
        strlwr (linha);
        /**
            Se o tiver apenas um caractere na linha, verifica se � algum comando espec�fico
        **/
        if (strlen(linha) == 1) {
            verificarComando(linha);
        }
        /**
            Reconhecido como palavra, armazena como sendo a �ltima palavra lida
        **/
        if (strlen(linha) > 1) {
            strcpy(ultimaPalavra, linha);
            if(buscarPalavra(ultimaPalavra) != -1) {
                //printf("ok %s\n", ultimaPalavra);
                printf("ok\n");
            }
            else if (algoritmos(ultimaPalavra) == 1) {

            }
            else {
                printf("not found\n");
            }
        }
    }
    free (tabela);
    return 0;
}

void imprimirTabela () {
    int i;
    for (i = 0; i < tamanhoTabela; i++) {
        printf("%2d %10d %s\n", i, tabela[i].chave, tabela[i].palavras);
    }
}

int algoritmos (char *palavra) {
    int retorno = 0;

    if (letraMais(palavra) == 1) {
        retorno = 1;
    }
    if (letraMenos(palavra) == 1) {
        retorno = 1;
    }
    if (letraTrocada(palavra) == 1) {
        retorno = 1;
    }
    if (letraErrada(palavra) == 1) {
        retorno = 1;
    }

    return retorno;
}

int letraMais (char *palavra) {
    return 0;
}

int letraMenos (char *palavra) {
    return 0;
}

int letraTrocada (char *palavra) {
    return 0;
}

int letraErrada (char *palavra) {
    return 0;
}

void verificarComando (char *palavra) {
    if (strcmp(palavra, "+") == 0) {
        adicionarPalavra (ultimaPalavra);
    }
    else if (strcmp(palavra, "-") == 0) {
        removerPalavra (ultimaPalavra);
    }
    else {  //palavra com apenas uma letra
        strcpy(ultimaPalavra, palavra);
        if(buscarPalavra(ultimaPalavra) != -1) {
            //printf("ok %s\n", ultimaPalavra);
            printf("ok\n");
        }
        else {
            printf("not found\n");
        }
    }
}

/**
    Busca se a palavra digitada est� na Tabela, se estiver retorna a posi��o exata da mesma
**/
int buscarPalavra (char *palavra) {
    if (strcmp(palavra, "") == 0) {
        return -1;
    }
    int chave = transformarPalavra(palavra);
    int posicao = calcularHash(chave);
    if (posicao < 0) {  //garante que a posi��o seja v�lida
        posicao *= -1;
    }
    int posicaoInicial = posicao;   //Guarda o valor inicial para o caso de a chave n�o estar na tabela e evitar loop

    //Verifica se na tabela a chave e seu conte�do correspondem
    while ((tabela[posicao].chave != chave)) {
        posicao++;
        if (posicao == tamanhoTabela) {     //Se chegar no final da tabela, dever� ir ao in�cio
            posicao = 0;
        }
        if (tabela[posicao].chave == 0) {
            return -1;
        }
        if (posicao == posicaoInicial) {    //Palavra n�o est� na hash
            return -1;
        }
    }
    return posicao;
}

/**
    Adiciona palavra na tabela
**/
void adicionarPalavra (char *palavra) {
    int chave = transformarPalavra(palavra);
    int posicao = calcularHash(chave);
    if (posicao < 0) {
        posicao *= -1;
    }
    int posicaoInicial = posicao;   //Guarda o valor inicial para o caso de controle da tabela cheia

    /**
    Identifica se j� existe uma chave na posi��o atual, se for a mesma chave, retorna erro e n�o adiciona
    Se for outra chave, pula uma posi��o at� encontrar uma vaga vazia
    **/
    if (tabela[posicao].chave == chave) {
        printf("fail\n");
        return;
    }
    else {
        while (tabela[posicao].chave != 0) {
            /**
            Identifica se j� existe uma chave na posi��o atual, se for a mesma chave, retorna erro e n�o adiciona
            Se for outra chave, pula uma posi��o at� encontrar uma vaga vazia
            **/
            if (tabela[posicao].chave == chave) {
                printf("fail\n");
            }
            posicao++;
            if (posicao == tamanhoTabela) {    //Se chegar no final da tabela, dever� ir ao in�cio
                posicao = 0;
            }
            else if (posicao == posicaoInicial) {    //tabela cheia :O O fator de carga garante que nunca chegue aqui
                realocarTabela();
            }
        }
    }
    tabela[posicao].chave = chave;
    strcpy(tabela[posicao].palavras, palavra);
    //printf ("ok %s\n", palavra);
    printf ("ok\n");
    qtdElementos++;
    fatorCarga = qtdElementos*100 / tamanhoTabela;
    /**
        Se a tabela estiver mais de 75% completa, vamos realoc�-la.
    **/
    if (fatorCarga > 75) {
        realocarTabela();
    }
}

/**
    Faz a busca pela palavra na tabela, se encontrar remove
**/
void removerPalavra (char *palavra) {
    int posicao = buscarPalavra(palavra);
    if (posicao == -1) {
        printf("fail\n");
    }
    else {
        tabela[posicao].chave = 0;
        //printf("ok %s\n", palavra);
        printf("ok\n");
    }
}

/**
    Fun��o que transforma uma string em um n�mero Natural
    Solu��o proposta por Cormen, p�gina 186
**/
int transformarPalavra (char *palavra) {
    int i;
    int multiplicador = 128;
    int somaAscii = 0;
    somaAscii *= palavra[0];
    for (i = 0; i < strlen(palavra); i++) {
        somaAscii += palavra[i] * multiplicador;
        multiplicador++;
    }
    return somaAscii;
}

/**
    Fun��o que calcula a Hash, ou seja, a posi��o que o c�digo deve ficar no vetor
**/
int calcularHash (int palavra) {
    return palavra%tamanhoTabela;
}

/**
    Rehash
    Primeiro criamos uma c�pia da tabela Hash exatamente com ela est� no momento do Rehash
    Duplicamos o tamanho da tabela
    Na c�pia da tabela, aplicamos hash em todos os valores para que peguem novas posi��es na nova tabela
    Ao final, libera mem�ria da tabela aux�liar
**/
void realocarTabela () {
    int i = 0;
    int posicao = 0;
    Tabela *tabelaAuxiliar;
    tabelaAuxiliar = (Tabela*) calloc (tamanhoTabela, sizeof(Tabela));
    for (i = 0; i < tamanhoTabela; i++) {
        tabelaAuxiliar[i].chave = tabela[i].chave;
        strcpy (tabelaAuxiliar[i].palavras, tabela[i].palavras);
    }
    tamanhoTabela *= 2;
    free (tabela);
    tabela = (Tabela*) calloc (tamanhoTabela, sizeof(Tabela));
    for (i = 0; i < tamanhoTabela/2; i++) {
        if (tabelaAuxiliar[i].chave != 0) {
            posicao = calcularHash(tabelaAuxiliar[i].chave);
            tabela[posicao].chave = tabelaAuxiliar[i].chave;
            strcpy (tabela[posicao].palavras, tabelaAuxiliar[i].palavras);
        }
    }
    free (tabelaAuxiliar);
}
