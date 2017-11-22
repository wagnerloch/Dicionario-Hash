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
int primos [] = {1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101,
103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373,
379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521,
523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673,
677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839,
853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991};
Tabela *tabela;
Variacoes *variacoes;


int main() {
    /**
        Aloca memória para a tabela
    **/
    tabela = (Tabela*) calloc (tamanhoTabela, sizeof(Tabela));
    while ((scanf(" %s", linha)) && (strcmp(linha, "*") != 0)) {
        strlwr (linha);
        /**
            Se o tiver apenas um caractere na linha, verifica se é algum comando específico
        **/
        if (strlen(linha) == 1) {
            verificarComando(linha);
        }
        /**
            Reconhecido como palavra, armazena como sendo a última palavra lida
        **/
        if (strlen(linha) > 1) {
            strcpy(ultimaPalavra, linha);
            if(buscarPalavra(transformarPalavra(ultimaPalavra)) != -1) {
                printf("ok %s\n", ultimaPalavra);
            }
            else if (algoritmos(ultimaPalavra) == 1) { //imprimir lista de variações ordenadas
                ordenarVariacoes();
                int i = 0;
                for (i = 0; i < qtdVariacoes; i++) {
                    if (variacoes[i].palavra == variacoes[i+1].palavra) {
                        //faz nada para imprimir só uma vez
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
    free (variacoes);
    return 0;
}

/**
    Ordena em ordem alfabética as variações encontradas pelos algoritmos de reconhecimento
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
    Se o usuário digitar uma letra a mais ele retornará a(s) suposta(s) palavra(s) correta(s)
    Uma letra a mais: a palavra fornecida possui uma letra qualquer a mais comparada à
original.
    Ex: "carrro"
**/
int letraMais (char *palavra) {
    int multiplicador = 0;
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
                somaPalavra += palavra[j] * primos[multiplicador++];
            }
        }
        posicao = buscarPalavra(somaPalavra);
        if (posicao != -1) {
            qtdVariacoes++;
            variacoes = (Variacoes*) realloc (variacoes, qtdVariacoes*sizeof(Variacoes));
            variacoes[qtdVariacoes-1].palavra = somaPalavra;
            retorno = 1;
        }
        multiplicador = 0;
        somaPalavra = 0;
    }
    return retorno;
}

/**
    Se o usuário digitar uma letra a menos ele retornará a(s) suposta(s) palavra(s) correta(s)
    Uma letra a menos: a palavra fornecida possui uma letra qualquer a menos em qualquer
posição.
    Ex: "caroça"
**/
int letraMenos (char *palavra) {
    int multiplicador = 0;
    int i, j, k, somaPalavra, posicao, retorno = 0;
    int tamanhoPalavra = strlen(palavra);
    //Busca por hífen
    for (i = 0; i < tamanhoPalavra+1; i++) {
        for (j = 0; j < tamanhoPalavra+1; j++) {
            if (i == j) {
					somaPalavra += 45 * primos[multiplicador++];
            }
            else if (j > i){
                somaPalavra += palavra[j-1] * primos[multiplicador++];
            }
            else {
                somaPalavra += palavra[j] * primos[multiplicador++];
            }
        }
        posicao = buscarPalavra(somaPalavra);
        if (posicao != -1) {
            qtdVariacoes++;
            variacoes = (Variacoes*) realloc (variacoes, qtdVariacoes*sizeof(Variacoes));
            variacoes[qtdVariacoes-1].palavra = somaPalavra;
            retorno = 1;
        }
        multiplicador = 0;
        somaPalavra = 0;
    }
    //Outras letras
    multiplicador = 0;
    somaPalavra = 0;
    for (i = 0; i < tamanhoPalavra+1; i++) {
		for (k = 97; k < 123; k++) {

			for (j = 0; j < tamanhoPalavra+1; j++) {
				if (i == j) {
					somaPalavra += k * primos[multiplicador++];
				}
				else if (j > i){
					somaPalavra += palavra[j-1] * primos[multiplicador++];
				}
				else {
                    somaPalavra += palavra[j] * primos[multiplicador++];
				}
			}
			posicao = buscarPalavra(somaPalavra);
			if (posicao != -1) {
            	qtdVariacoes++;
            	variacoes = (Variacoes*) realloc (variacoes, qtdVariacoes*sizeof(Variacoes));
            	variacoes[qtdVariacoes-1].palavra = somaPalavra;
            	retorno = 1;
        	}
        	multiplicador = 0;
        	somaPalavra = 0;
		}
	}
	return retorno;
}

/**
    Se o usuário digitar duas letras trocadas ele retornará a(s) suposta(s) palavra(s) correta(s)
    Letras trocadas: a palavra fornecida possui duas letras vizinhas em posições invertidas.
    Ex: "computdaor"
**/
int letraTrocada (char *palavra) {
    int multiplicador = 0;
    int i, j, somaPalavra, posicao, retorno = 0;
    int tamanhoPalavra = strlen(palavra);
    for (i = 0; i < tamanhoPalavra-1; i++) {
        somaPalavra = 0;
        for (j = 0; j < tamanhoPalavra; j++) {
            if (i == j) {
                somaPalavra += palavra[j+1] * primos[multiplicador++];
                somaPalavra += palavra[j++] * primos[multiplicador++];
            }
            else {
                somaPalavra += palavra[j] * primos[multiplicador++];
            }
        }
        posicao = buscarPalavra(somaPalavra);
        if (posicao != -1) {
            qtdVariacoes++;
            variacoes = (Variacoes*) realloc (variacoes, qtdVariacoes*sizeof(Variacoes));
            variacoes[qtdVariacoes-1].palavra = somaPalavra;
            retorno = 1;
        }
        multiplicador = 0;
    }

    return retorno;
}

/**
    Se o usuário digitar uma letra errada ele retornará a(s) suposta(s) palavra(s) correta(s)
    Uma letra errada: a palavra correta pode ser construída trocando uma letra da palavra
original por outra.
    Ex: "computafor"
**/
int letraErrada (char *palavra) {
    int multiplicador = 0;
    int i, j, k, somaPalavra, posicao, retorno = 0;
    int tamanhoPalavra = strlen(palavra);

    //Busca por hífen
    for (i = 0; i < tamanhoPalavra; i++) {
        for (j = 0; j < tamanhoPalavra; j++) {
            if (i == j) {
					somaPalavra += 45 * primos[multiplicador++];
            }
            else {
                somaPalavra += palavra[j] * primos[multiplicador++];
            }
        }
        posicao = buscarPalavra(somaPalavra);
        if (posicao != -1) {
            qtdVariacoes++;
            variacoes = (Variacoes*) realloc (variacoes, qtdVariacoes*sizeof(Variacoes));
            variacoes[qtdVariacoes-1].palavra = somaPalavra;
            retorno = 1;
        }
        multiplicador = 0;
        somaPalavra = 0;
    }

    for (i = 0; i < tamanhoPalavra; i++) {
		for (k = 97; k < 123; k++) {

			for (j = 0; j < tamanhoPalavra; j++) {
				if (i == j) {
					somaPalavra += k * primos[multiplicador++];
				}
				else {
                    somaPalavra += palavra[j] * primos[multiplicador++];
				}
			}
			posicao = buscarPalavra(somaPalavra);
			if (posicao != -1) {
            	qtdVariacoes++;
            	variacoes = (Variacoes*) realloc (variacoes, qtdVariacoes*sizeof(Variacoes));
            	variacoes[qtdVariacoes-1].palavra = somaPalavra;
            	retorno = 1;
        	}
        	multiplicador = 0;
        	somaPalavra = 0;
		}
	}
    return retorno;
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
    Busca se a palavra digitada está na Tabela, se estiver retorna a posição exata da mesma
    Parametro palavra já é a String transformada em um número Natural
**/
int buscarPalavra (int palavra) {
    if (palavra == 0) {
        return -1;
    }
    int posicao = calcularHash(palavra);
    if (posicao < 0) {  //garante que a posição seja válida
        posicao *= -1;
    }
    int posicaoInicial = posicao;   //Guarda o valor inicial para o caso de a chave não estar na tabela e evitar loop

    //Verifica se na tabela a chave e seu conteúdo correspondem
    while ((tabela[posicao].chave != palavra)) {
        posicao++;
        if (posicao == tamanhoTabela) {     //Se chegar no final da tabela, deverá ir ao início
            posicao = 0;
        }
        if (tabela[posicao].chave == 0) {
            return -1;
        }
        if (posicao == posicaoInicial) {    //Palavra não está na hash
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
    Identifica se já existe uma chave na posição atual, se for a mesma chave, retorna erro e não adiciona
    Se for outra chave, pula uma posição até encontrar uma vaga vazia
    **/
    if (tabela[posicao].chave == chave) {
        printf("fail\n");
        return;
    }
    else {
        while (tabela[posicao].chave != 0) {
            /**
            Identifica se já existe uma chave na posição atual, se for a mesma chave, retorna erro e não adiciona
            Se for outra chave, pula uma posição até encontrar uma vaga vazia
            **/
            if (tabela[posicao].chave == chave) {
                printf("fail\n");
            }
            posicao++;
            if (posicao == tamanhoTabela) {    //Se chegar no final da tabela, deverá ir ao início
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
        Se a tabela estiver mais de 75% completa, vamos realocá-la.
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
    Função que transforma uma string em um número Natural
    Solução proposta por Cormen, página 186
**/
int transformarPalavra (char *palavra) {
    int i;
    int multiplicador = 0;
    int somaAscii = 0;
    somaAscii *= palavra[0];
    for (i = 0; i < strlen(palavra); i++) {
        somaAscii += palavra[i] * primos[multiplicador];
        multiplicador++;
    }
    return somaAscii;
}

/**
    Função que calcula a Hash, ou seja, a posição que o código deve ficar no vetor
**/
int calcularHash (int palavra) {
    return palavra%tamanhoTabela;
}

/**
    Rehash
    Primeiro criamos uma cópia da tabela Hash exatamente com ela está no momento do Rehash
    Duplicamos o tamanho da tabela
    Na cópia da tabela, aplicamos hash em todos os valores para que peguem novas posições na nova tabela
    Ao final, libera memória da tabela auxíliar
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
