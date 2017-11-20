#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void imprimirTabela ();
void adicionarPalavra (char *palavra);
void removerPalavra (char *palavra);
int buscarPalavra (int palavra);
void verificarComando (char *palavra);
int calcularHash (int palavra);
int transformarPalavra (char *palavra);
int algoritmos (char *palavra);
int letraMais (char *palavra);
int letraMenos (char *palavra);
int letraTrocada (char *palavra);
int letraErrada (char *palavra);
void realocarTabela ();
void ordenarVariacoes ();

typedef struct {
    int chave;
	char palavras[100];
} Tabela;

typedef struct {
    int palavra;
} Variacoes;

char linha [100];
char ultimaPalavra [100];
int tamanhoTabela = 50;
int qtdElementos = 0;
int fatorCarga = 0;
int qtdVariacoes = 0;
Tabela *tabela;
Variacoes *variacoes;


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
            if(buscarPalavra(transformarPalavra(ultimaPalavra)) != -1) {
                printf("ok %s\n", ultimaPalavra);
            }
            else if (algoritmos(ultimaPalavra) == 1) { //imprimir lista de varia��es ordenadas
                ordenarVariacoes();
                int i = 0;
                for (i = 0; i < qtdVariacoes; i++) {
                    if (variacoes[i].palavra == variacoes[i+1].palavra) {
                        //faz nada para imprimir s� uma vez
                    }
                    else {
                        printf("ok %s\n", tabela[buscarPalavra(variacoes[i].palavra)].palavras);
                    }
                }
                qtdVariacoes = 0;
            }
            else {
                printf("not found\n");
            }
        }
    }
    imprimirTabela();
    free (tabela);
    return 0;
}

/**
    Ordena em ordem alfab�tica as varia��es encontradas pelos algoritmos de reconhecimento
**/
void ordenarVariacoes () {
    int temp, i, j;
    for (j = 1; j < qtdVariacoes; j++) {
        i = j-1;
        temp = variacoes[j].palavra;
        while ((i >= 0) && (temp < variacoes[i].palavra)) {
            variacoes[i+1].palavra = variacoes[i].palavra;
            i--;
        }
        variacoes[i+1].palavra = temp;
    }
}

/**
    Imprime toda a tabela
**/
void imprimirTabela () {
    int i;
    for (i = 0; i < tamanhoTabela; i++) {
        printf("%2d %10d %s\n", i, tabela[i].chave, tabela[i].palavras);
    }
}

/**
    Executa todos os algoritmos de reconhecimento
**/
int algoritmos (char *palavra) {
    int retorno = 0;
    qtdVariacoes = 0;

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

/**
    Se o usu�rio digitar uma letra a mais ele retornar� a(s) suposta(s) palavra(s) correta(s)
    Uma letra a mais: a palavra fornecida possui uma letra qualquer a mais comparada �
original.
    Ex: "carrro"
**/
int letraMais (char *palavra) {
    int multiplicador = 128;
    int i, j = 0;
    int tamanhoPalavra = strlen(palavra);
    int somaPalavra = 0;
    int posicao = 0;
    int retorno = 0;
    for (i = 0; i < tamanhoPalavra; i++) {
        for (j = 0; j < tamanhoPalavra; j++) {
            if (i == j) {
                somaPalavra += 0;
            }
            else {
                somaPalavra += palavra[j] * multiplicador++;
            }
        }
        posicao = buscarPalavra(somaPalavra);
        if (posicao != -1) {
            qtdVariacoes++;
            variacoes = (Variacoes*) realloc (variacoes, qtdVariacoes*sizeof(Variacoes));
            variacoes[qtdVariacoes-1].palavra = somaPalavra;
            retorno = 1;
        }
        multiplicador = 128;
        somaPalavra = 0;
    }
    return retorno;
}

/**
    Se o usu�rio digitar uma letra a menos ele retornar� a(s) suposta(s) palavra(s) correta(s)
    Uma letra a menos: a palavra fornecida possui uma letra qualquer a menos em qualquer
posi��o.
    Ex: "caro�a"
**/
int letraMenos (char *palavra) {
    int multiplicador = 128;
    int i, j, k, somaPalavra, posicao, retorno = 0;
    int tamanhoPalavra = strlen(palavra);
    //Busca por h�fen
    for (i = 0; i < tamanhoPalavra+1; i++) {
        for (j = 0; j < tamanhoPalavra+1; j++) {
            if (i == j) {
					somaPalavra += 45 * multiplicador++;
            }
            else if (j > i){
                somaPalavra += palavra[j-1] * multiplicador++;
            }
            else {
                somaPalavra += palavra[j] * multiplicador++;
            }
        }
        posicao = buscarPalavra(somaPalavra);
        if (posicao != -1) {
            qtdVariacoes++;
            variacoes = (Variacoes*) realloc (variacoes, qtdVariacoes*sizeof(Variacoes));
            variacoes[qtdVariacoes-1].palavra = somaPalavra;
            retorno = 1;
        }
        multiplicador = 128;
        somaPalavra = 0;
    }
    //Outras letras
    for (i = 0; i < tamanhoPalavra+1; i++) {
		for (k = 97; k < 123; k++) {

			for (j = 0; j < tamanhoPalavra+1; j++) {
				if (i == j) {
					somaPalavra += k * multiplicador++;
				}
				else if (j > i){
					somaPalavra += palavra[j-1] * multiplicador++;
				}
				else {
                    somaPalavra += palavra[j] * multiplicador++;
				}
			}
			posicao = buscarPalavra(somaPalavra);
			if (posicao != -1) {
            	qtdVariacoes++;
            	variacoes = (Variacoes*) realloc (variacoes, qtdVariacoes*sizeof(Variacoes));
            	variacoes[qtdVariacoes-1].palavra = somaPalavra;
            	retorno = 1;
        	}
        	multiplicador = 128;
        	somaPalavra = 0;
		}
	}
	return retorno;
}

/**
    Se o usu�rio digitar duas letras trocadas ele retornar� a(s) suposta(s) palavra(s) correta(s)
    Letras trocadas: a palavra fornecida possui duas letras vizinhas em posi��es invertidas.
    Ex: "computdaor"
**/
int letraTrocada (char *palavra) {
    return 0;
}

/**
    Se o usu�rio digitar uma letra errada ele retornar� a(s) suposta(s) palavra(s) correta(s)
    Uma letra errada: a palavra correta pode ser constru�da trocando uma letra da palavra
original por outra.
    Ex: "computafor"
**/
int letraErrada (char *palavra) {
    return 0;
}

/**
    Verifica qual comando deve ser executado
**/
void verificarComando (char *palavra) {
    if (strcmp(palavra, "+") == 0) {
        adicionarPalavra (ultimaPalavra);
    }
    else if (strcmp(palavra, "-") == 0) {
        removerPalavra (ultimaPalavra);
    }
    else {  //palavra com apenas uma letra
        strcpy(ultimaPalavra, palavra);
        if(buscarPalavra(transformarPalavra(ultimaPalavra)) != -1) {
            printf("ok %s\n", ultimaPalavra);
            //printf("ok\n");
        }
        else {
            printf("not found\n");
        }
    }
}

/**
    Busca se a palavra digitada est� na Tabela, se estiver retorna a posi��o exata da mesma
    Parametro palavra j� � a String transformada em um n�mero Natural
**/
int buscarPalavra (int palavra) {
    if (palavra == 0) {
        return -1;
    }
    int posicao = calcularHash(palavra);
    if (posicao < 0) {  //garante que a posi��o seja v�lida
        posicao *= -1;
    }
    int posicaoInicial = posicao;   //Guarda o valor inicial para o caso de a chave n�o estar na tabela e evitar loop

    //Verifica se na tabela a chave e seu conte�do correspondem
    while ((tabela[posicao].chave != palavra)) {
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
    printf ("ok %s\n", palavra);
    //printf ("ok\n");
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
    int posicao = buscarPalavra(transformarPalavra(palavra));
    if (posicao == -1) {
        printf("fail\n");
    }
    else {
        tabela[posicao].chave = 0;
        printf("ok %s\n", palavra);
        //printf("ok\n");
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
