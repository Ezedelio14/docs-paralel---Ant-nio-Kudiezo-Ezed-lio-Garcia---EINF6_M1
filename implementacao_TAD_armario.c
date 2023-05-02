#include <stdio.h>
#include <stdlib.h>
#include "TAD.h"

struct armario {
    int id;
    Doc *listaDoc;
    double *mediaAssuntos;
    struct armario *next;
};

    /**
* Inicializa��o da lista de arm�rios
*/
Armario *iniListaArmario() {
    return NULL;
}
/**
* Inser��o de arm�rio
*/
Armario *insereArmario(Armario *a, int id) {
    Armario *novo, *aux = a;
	novo = (Armario *) malloc(sizeof(Armario));
    if(a == NULL) {
    	novo->id = id;
	    novo->listaDoc = NULL;
	    novo->next = novo;
	} else {
		while(aux->next != a)
			aux = aux->next;
		novo->id = id;
	    novo->listaDoc = NULL;
	    novo->next = aux->next;
	    aux->next = novo;
	}
    return novo;
}

