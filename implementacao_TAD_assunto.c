#include <stdio.h>
#include <stdlib.h>
#include "TAD.h"

struct assunto {
    int id;
    double ponto;
    struct assunto *next;
};

/**
* Inserção de assunto
*/
Assunto *insereAssunto(Assunto *a, int id, double ponto) {
    Assunto *novo, *aux = a;
	novo = (Assunto *) malloc(sizeof(Assunto));
    if(a == NULL) {
    	novo->id = id;
	    novo->ponto = ponto;
	    novo->next = novo;
	} else {
		while(aux->next != a)
			aux = aux->next;
		novo->id = id;
	    novo->ponto = ponto;
	    novo->next = aux->next;
	    aux->next = novo;
	}
    return novo;
}


