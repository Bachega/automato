#ifndef AUTOMATO_H
#define AUTOMATO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//ESTRUTURA

typedef struct transicao {
	char elem;
	struct estado* e;
	struct transicao* prox;	
}Transicao;

typedef struct estado {
	char* nome;
	int inicial;
	int final;
	struct transicao* t;
	struct estado* prox;
}Estado;

typedef struct automato {
	char* alf;
	struct estado* s;
}Automato;


//FUNCOES

Automato* criarAutomato();

void inserirEstado(Automato*, char*, int, int);

void inserirTransicao(Automato*, char*, char*, char);

void deletarTransicao(Transicao*);

void deletarEstado(Estado*);

void deletarAutomato(Automato*);

void imprimirEstado(Estado*);

void imprimirAutomato(Automato*);

void processarFita(char*, Automato*);

void imprimirGramaticaEstado(Estado*);

void imprimirGramatica(Automato*);

Estado* buscaEstado(char*, Automato*);

void carregar(char*, Automato*);

int pertenceAlfabeto(char*, Automato*);

#endif
