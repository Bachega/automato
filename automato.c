#include "automato.h"

Automato* criarAutomato() {
	Automato* a = (Automato*)malloc(sizeof(Automato));
	a->alf = NULL;
	a->s = NULL;
	return a;	
}

void inserirEstado(Automato* a, char* nome, int inicial, int final) {
	Estado* e = (Estado*)malloc(sizeof(Estado));
	e->nome = (char*)malloc(sizeof(nome)+1);
	strcpy(e->nome, nome);
	e->inicial = inicial;
	e->final = final;
	e->t = NULL;
	e->prox = NULL;
	
	if(a->s==NULL)
		a->s = e;
	else {
		Estado* temp = a->s;
		while(temp->prox)
			temp = temp->prox;
		temp->prox = e;
		temp = NULL;
	}
	
	if(e->final) inserirTransicao(a, e->nome, e->nome, '@');
}

void inserirTransicao(Automato* a, char* fonte, char* destino, char elem) {
	Estado* e = NULL;
	Estado* d = NULL;
	
	Estado* temp = a->s;
	while(temp!=NULL) {
		if(strcmp(temp->nome, fonte) == 0) {
			e = temp;
			break;
		}
		temp = temp->prox;
	}
	
	temp = a->s;
	while(temp!=NULL) {
		if(strcmp(temp->nome, destino) == 0) {
			d = temp;
			break;
		}
		temp = temp->prox;
	}
	temp = NULL;
	
	if(e && d) {
		Transicao* t = (Transicao*)malloc(sizeof(Transicao));
		t->elem = elem;
		t->e = d;
		t->prox = NULL;
		if(e->t==NULL)
			e->t = t;
		else {
			Transicao* trans = e->t;
			while(trans->prox)
				trans = trans->prox;
			trans->prox = t;
			trans = NULL;
		}
	}
}

void deletarTransicao(Transicao* t) {
	if(t==NULL) return;
	t->e = NULL;
	deletarTransicao(t->prox);
	t->prox = NULL;
	free(t);
	//printf("\nApaguei transicao\n");
}

void deletarEstado(Estado* e) {
	if(e==NULL) return;
	free(e->nome);
	e->nome = NULL;
	deletarTransicao(e->t);
	deletarEstado(e->prox);
	e->t = NULL;
	e->prox = NULL;
	free(e);
	//printf("\nApaguei estado\n");
}

void deletarAutomato(Automato* a) {
	free(a->alf);
	a->alf = NULL;
	deletarEstado(a->s);
	free(a);
	//printf("\nApaguei automato\n");
}

void imprimirEstado(Estado* e) {
	Transicao* temp = e->t;
	printf("Estado: %s\nInicial: %d\nFinal: %d\nTransicoes:\n", e->nome, e->inicial, e->final);
	
	while(temp) {
		printf("(%s, %c) = %s\n", e->nome, temp->elem, temp->e->nome);
		temp = temp->prox;
	}
	
	printf("\n");
}

void imprimirAutomato(Automato* a) {
	int i;
	int t = strlen(a->alf);
	Estado* temp = a->s;
	
	printf("AUTOMATO\n\n");
	printf("Alfabeto = {");
	
	for(i=0; i<t; i++) {
		if(i!=(t-1)) printf("%c, ", *(a->alf+i));
		else printf("%c}\n\n", *(a->alf+i));
	}
	
	while(temp) {
		imprimirEstado(temp);
		temp = temp->prox;
	}
}

void processarFita(char* f, Automato* a) {
	int i, x=1;
	Estado* e = a->s;
	Transicao* t = NULL;
	
	printf("\n");
	for(i=0; x; i++) {
		if(!f[i]) x=0;
		printf("[%s]%s\n", e->nome, (f+i));
		t = e->t;
		while(t!=NULL) {
			if(t->elem == f[i]) {
				e = t->e;
				break;
			}
			t = t->prox;
		}	
	}
	
	if(e->final) printf("\nACEITA\n\n");
	else printf("\nREJEITA\n\n");
}

void imprimirGramaticaEstado(Estado* e) {
	int nul=0;
	Transicao* t = e->t;

	printf("%s -> ", e->nome);	
	if(t->elem=='@'){ nul = 1; t = t->prox;}
	while(t) {
		if(t->prox==NULL && !nul) printf("%c(%s)", t->elem, t->e->nome);
		else printf("%c(%s) | ", t->elem, t->e->nome);
		t = t->prox;
	}
	if(nul) printf("@");
	printf("\n");
}

void imprimirGramatica(Automato* a) {
	Estado* e = a->s;
	printf("GRAMATICA\n\n");
	while(e) {
		imprimirGramaticaEstado(e);
		e = e->prox;
	}
	printf("\n");
}

Estado* buscaEstado(char* nome, Automato* a) {
	Estado* e = a->s;
	while(e!=NULL) {
		if(strcmp(e->nome, nome) == 0) {
			return e;
		}
		e = e->prox;
	}
	return NULL;
}

void carregar(char* f, Automato* a) {
	int i=0;
	char c = '\0';
	char elem = '\0';
	char* nome = (char*)malloc(sizeof(char)*12 + 1); *nome = '\0';
	char* alf = (char*)malloc(sizeof(char)*37 + 1); *alf = '\0';
	char* destino = (char*)malloc(sizeof(char)*12 + 1); *destino = '\0';
	Estado* temp = NULL;
	FILE* arq = NULL;
	
	arq = fopen(f, "r");
	
	if(!arq) {
		printf("Nao foi possivel carregar\n\n");
        return;
	}
	
	while((c=fgetc(arq))!='\n' && i<36) { // CARREGA O ALFABETO
		*(alf+i) = c;
		c=fgetc(arq);
		i++;
	}
	*(alf+i) = '@'; i++;
	*(alf+i) = '\0';
	
	a->alf = (char*)malloc(sizeof(strlen(alf))+1);
	strcpy(a->alf, alf);
	
	free(alf);
	*nome='\0'; i=0;
	
	while((c=fgetc(arq))!='\n') { // PEGA OS NOMES DOS ESTADOS E OS CRIA
		do {
			*(nome+i) = c;
			i++;
		} while((c=fgetc(arq))!=';' && i<12);
		*(nome+i) = '\0';
		
		inserirEstado(a, nome, 0, 0);
		
		*nome='\0'; i=0;
	}
	
	while((c=fgetc(arq))!=';' && i<12) { // PEGA O NOME DO ESTADO INCIAL E O SETA COMO ESTE
		*(nome+i) = c;
		i++;
	}
	c = fgetc(arq);
	*(nome+i) = '\0';
	
	temp = buscaEstado(nome, a);
	temp->inicial = 1;
	temp = NULL;
	
	*nome='\0'; i=0;
	
	while((c=fgetc(arq))!='\n') { // PEGA O NOME DOS ESTADOS FINAIS, SETA COMO ESTES E INSERE UMA TRANSICAO '@'
		do {
			*(nome+i) = c;
			i++;
		} while((c=fgetc(arq))!=';' && i<12);
		*(nome+i) = '\0';
		
		temp = buscaEstado(nome, a);
		temp->final = 1;
		inserirTransicao(a, temp->nome, temp->nome, '@');
		temp = NULL;
		
		*nome='\0'; i=0;
	}
	
	while(!feof(arq)) { // INSERE AS TRANSICOES
		
		while((c=fgetc(arq))!=';' && i<12) {
			*(nome+i) = c;
			i++;
		}
		*(nome+i) = '\0';
		
		i=0;
		
		elem = fgetc(arq);
		fgetc(arq);
		
		while((c=fgetc(arq))!=';' && i<12) {
			*(destino+i) = c;
			i++;
		}
		*(destino+i) = '\0';
		
		inserirTransicao(a, nome, destino, elem);
		
		*(nome) = '\0';
		*(destino) = '\0';
		i = 0;		
		c = fgetc(arq);
	}
		
	free(nome);
	free(destino);
	fclose(arq);
}

int pertenceAlfabeto(char* cad, Automato* a) {
	int i=0, j, v;
	while(cad[i]) {
		v=0;
		j=0;
		while(*(a->alf+j)) { 
			if(cad[i]==*(a->alf+j)) v = 1;
			j++;
		}
		if(!v) return 0;
		i++;
	}
	return 1;
}


int main() {
	int x=1, m;
	int op=0;
	Automato* a = NULL;
	char f[257];
	char c[257];
	
	
	while(x) {
		m=1;
		system("cls");
		printf("Digite o caminho do arquivo para carregar o automato\nou digite zero para sair\n\n");
		scanf("%s", f); fflush(stdin);
		if(!strcmp(f, "0")) break;
		
		a = criarAutomato();
		carregar(f, a);
		f[0]='\0';
		
		if(a->alf==NULL) {
			deletarAutomato(a);
			printf("Algum erro ocorreu ao carregar o automato\n\n");
			system("pause");
		} else {
			while(m) {
				system("cls");
				printf("MENU\n\n");
				printf("1- Imprimir o automato\n");
				printf("2- Imprimir gramatica\n");
				printf("3- Testar uma cadeia\n");
				printf("0- Sair/Carregar outro automato\n\n");
				scanf("%d", &op); fflush(stdin);
				system("cls");
				switch(op) {
					case 1:
						imprimirAutomato(a); system("pause"); break;
					case 2:
						imprimirGramatica(a); system("pause"); break;
					case 3:
						printf("Digite uma cadeia:\n\n");
						scanf("%s", c); fflush(stdin);
						if(pertenceAlfabeto(c, a)) {
							processarFita(c, a); 
						} else {
							printf("\nA cadeia possui elementos que nao pertencem ao alfabeto\n\n");
						}
						c[0]='\0';
						system("pause");
						break;
					case 0:
						m=0; break;
				}
			}
			deletarAutomato(a);
			a = NULL;
		}
	}
}
