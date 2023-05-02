#include <stdio.h>
#include <stdlib.h>
#include "TAD.h"

struct documento {
    int id;
    Assunto *listaAssunto;
    struct documento *next;
};


/**
* Inicialização da lista de documentos
*/
Doc *iniListaDoc() {
    return NULL;
}
/**
* Inserção de documento
*/
Doc *insereDoc(Doc *d, int id) {
    Doc *novo, *aux = d;
	novo = (Doc *) malloc(sizeof(Doc));
    if(d == NULL) {
    	novo->id = id;
	    novo->listaAssunto = NULL;
	    novo->next = novo;
	} else {
		while(aux->next != d)
			aux = aux->next;
		novo->id = id;
	    novo->listaAssunto = NULL;
	    novo->next = aux->next;
	    aux->next = novo;
	}
    return novo;
}

Doc *removeDoc(Doc *listaDoc, int id) {
    if(listaDoc == NULL) {
        printf("Lista vazia.\n");
        return NULL;
    }
    Doc *current = listaDoc, *previous = NULL;
    while(current->id != id) {
        previous = current;
        current = current->next;
        if(current == listaDoc) {
            printf("Doc inexistente!");
            return NULL;
        }
    }
    if(current == listaDoc && current->next == listaDoc) {
        listaDoc = NULL;
        free(current);
        return NULL;
    }
    if(current == listaDoc) {
        Doc *last = listaDoc;
        while(last->next != listaDoc) {
            last = last->next;
        }
        listaDoc = current->next;
        last->next = listaDoc;
    } else
        previous->next = current->next;
    free(current);
    return listaDoc;
}

