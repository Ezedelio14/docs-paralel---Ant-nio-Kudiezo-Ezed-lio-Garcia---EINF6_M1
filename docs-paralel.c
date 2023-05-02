#include "implementacao_TAD_armario.c"
#include "implementacao_TAD_doc.c"
#include "implementacao_TAD_assunto.c"
#include <math.h>
#include <omp.h>

void insertDocsByRoundRobin(int d, int s, int c, Armario *a);
void inserirPontuacaoNoAssunto(int idDoc, int idAssunto, double ponto, Armario *armarios, int d);
int armarioMenordistancia(int idDoc, Armario *armarios, int s, int d);
void calcularMedias(Armario *armarios, int s);


void main(int argc, char argv[]){
    /**
    * L�GICA DO PROGRAMA ####?
    * L� do ficheiro "doc.in"
    * Atribui os documentos em ar�rios com o algoritmo Round Robin
    * Faz c�lculo de m�dias dos arm�rios
    * Faz c�lculo da menor dist�ncia de cada doc em rela��o a arm�rios
    * Movimenta os documentos
    * Escreve o resultado da sa�da no "doc.out"
    *
    **/
omp_set_num_threads(2);
        int c, d, s;
        Armario *armarios = iniListaArmario();
        FILE *ficheiro;
        ficheiro = fopen("doc.in", "r");
        float valorLido;
        int linhaLeitura = 1, controlador = 0, i, idDoc;
        while(fscanf(ficheiro, "%f", &valorLido) != EOF) {
            if(linhaLeitura == 1){
                c = valorLido;
                for(i = c-1; i >= 0; i--) {//Inserindo "c" arm�rios
                    armarios = insereArmario(armarios, i);
                }
            }
            else if(linhaLeitura == 2) {
                d = valorLido;
            }
            else if(linhaLeitura == 3) {
                s = valorLido;
                insertDocsByRoundRobin(d, s, c, armarios);//Insere documentos aos arm�rios com Round Robin
            }
            else {
                if(controlador == 0) {
                    idDoc = valorLido;
                } else if(controlador <= s) {
                    inserirPontuacaoNoAssunto(idDoc, controlador-1, valorLido, armarios, d);//Insere ponto em cada assunto dos docs, ao ler cada linha de docs do "doc.in"
                }
                if(controlador == s) controlador = 0;
                else controlador++;
            }
            linhaLeitura++;
        }
        fclose(ficheiro);
    /**
    * Fazer c�lculo das m�dias de assuntos em cada arm�rio
    */
        calcularMedias(armarios, s);
    /**
    * Fazendo c�lculo da menor dist�ncia dos docs em rela��o a arm�rios,
    * Movimenta docs
    * C�lcula nova m�dia dos arm�rios
    * Repete o processo at� quando todos os docs n�o se movimentam mais
    *
    */
        controlador = 0;
        int armMenorD, armDoc;
        for(i = 0; controlador != d; i++) {
            armDoc = qualArmarioDoDoc(i, armarios, d)->id;
            armMenorD = armarioMenordistancia(i, armarios, s, d);
            if(armDoc == armMenorD){
                controlador++;
            } else {
                armarios = moverDoc(i, armMenorD, armarios, d);
                controlador = 0;
            }
            if(i == d-1) {
                i = -1;
                calcularMedias(armarios, s);
            }
        }

    /**
    * Escrevendo o resultado do estado dos arm�rios no "doc.out"
    */
        ficheiro = fopen("doc.out", "w");
        for(i = 0; i < d; i++) {
            fprintf(ficheiro, "%d %d \n", i, qualArmarioDoDoc(i, armarios, d)->id);
            printf("%d %d\n", i, qualArmarioDoDoc(i, armarios, d)->id);
        }
        fclose(ficheiro);
}


/**
* Distribui os docs nos arm�rios, usando o Round Robin
*/
    void insertDocsByRoundRobin(int d, int s, int c, Armario *a) {
         int i, j, k = 0;
    #pragma omp parallel for private(i,j,k) shared(a)
    for(i = 0; i < d; i++) {
        k = i % c;
        Armario *aux = a;
        for(int l = 0; l < k; l++)
            aux = aux->next;
        #pragma omp critical
        {
            aux->listaDoc = insereDoc(aux->listaDoc, i);
        }
        for(j = 0; j < s; j++) {
            #pragma omp critical
            {
                aux->listaDoc->listaAssunto = insereAssunto(aux->listaDoc->listaAssunto, j, 0);
            }
        }
    }
    }
    /**
    * Insere as pontua��es de cada assunto
    */
        void inserirPontuacaoNoAssunto(int idDoc, int idAssunto, double ponto, Armario *armarios, int d) {
             #pragma omp parallel shared(armarios,d,idDoc,idAssunto,ponto)
    {
        Armario *armAux;
        Doc *docAux;
        Assunto *assuntoAux;
        int i;
        #pragma omp for
        for(int j=0; j<d; j++){
            armAux = armarios;
            for(i = 0; i < j; i++) armAux = armAux->next;

            docAux = armAux->listaDoc;
            while(docAux != NULL && docAux->id != idDoc) docAux = docAux->next;
            if(docAux != NULL){
                assuntoAux = docAux->listaAssunto;
                while(assuntoAux != NULL && assuntoAux->id != idAssunto) assuntoAux = assuntoAux->next;
                if(assuntoAux != NULL) assuntoAux->ponto = ponto;
            }
        }
    }
    return;
        }
    /**
    * Calcula as m�dias de assuntos de todos os arm�rios
    */
        void calcularMedias(Armario *armarios, int s) {
            Armario *aux = armarios;
            do {
                aux->mediaAssuntos = mediaArmario(aux->listaDoc, s);
                aux = aux->next;
            } while(aux != armarios);
        }
        /**
        * Auxiliar da fun��o "calcularMedias"
        */
            double *mediaArmario(Doc *listaDocs, int s) {
                 Doc *d = listaDocs;
    double *v = malloc(s * sizeof(double));
    int i, contadorDoc = 0;

    #pragma omp parallel for private(d, contadorDoc)
    for(i = 0; i < s; i++) {
        double soma = 0.0;
        d = listaDocs;
        contadorDoc = 0;

        do {
            while(d->listaAssunto->id != i)
                d->listaAssunto = d->listaAssunto->next;

            soma += d->listaAssunto->ponto;
            d = d->next;
            contadorDoc++;
        } while(d != listaDocs);

        v[i] = soma / contadorDoc;
    }

    return v;
            }


/**
* Busca e retorna o "id" do arm�rio menos distante do doc
*/
    int armarioMenordistancia(int idDoc, Armario *armarios, int s, int d) {
        Doc *doc = buscarDoc(idDoc, armarios);
    int num_threads = omp_get_max_threads();
    int block_size = (int) ceil((double) num_armarios / num_threads);
    double *menorDistancia = malloc(num_threads * sizeof(double));
    int *idArmarioMenor = malloc(num_threads * sizeof(int));

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int start = tid * block_size;
        int end = start + block_size;
        if (end > num_armarios) end = num_armarios;

        Armario *aux = armarios;
        for (int i = 0; i < start; i++) {
            aux = aux->next;
        }

        double distancia = 0;
        Assunto *a;
        for (int i = start; i < end; i++) {
            distancia = 0;
            for (int j = 0; j < s; j++) {
                a = buscarAssunto(j, doc, d);
                distancia += pow((a->ponto - aux->mediaAssuntos[j]), 2);
            }
            if (i == start || distancia < menorDistancia[tid]) {
                menorDistancia[tid] = distancia;
                idArmarioMenor[tid] = aux->id;
            }
            aux = aux->next;
        }
    }

    double menorDist = menorDistancia[0];
    int idArmarioMenor = idArmarioMenor[0];
    for (int i = 1; i < num_threads; i++) {
        if (menorDistancia[i] < menorDist) {
            menorDist = menorDistancia[i];
            idArmarioMenor = idArmarioMenor[i];
        }
    }

    free(menorDistancia);
    free(idArmarioMenor);
    return idArmarioMenor;
    }

/**
* Movimenta um doc para um outro arm�rio
*/
    Armario *moverDoc(int idDoc, int idArmario, Armario *armarios, int d) {
        Doc *doc = buscarDoc(idDoc, armarios);
        Assunto *ass = doc->listaAssunto;
        armarios = qualArmarioDoDoc(idDoc, armarios, d);
        armarios->listaDoc = removeDoc(armarios->listaDoc, idDoc);

        armarios = buscarArmario(idArmario, armarios);
        armarios->listaDoc = insereDoc(armarios->listaDoc, idDoc);
        armarios->listaDoc->listaAssunto = ass;
        return armarios;
    }

/**
* Fun��o que retorna a estrutura do doc solicitado pelo seu "id"
*/
    Doc *buscarDoc(int idDoc, Armario *armarios) {
        Armario *aux = armarios;
        do {
            Doc *d = aux->listaDoc;
            do {
                if(d->id == idDoc)
                    return d;
                d = d->next;
            } while(d != aux->listaDoc);
            aux = aux->next;
        } while(aux != armarios);
    }

/**
* Fun��o que retorna a estrutura do assunto solicitado pelo seu "id"
*/
    Assunto *buscarAssunto(int idAssunto, Doc *doc, int d) {
      Doc* aux = doc;
    Assunto* resultado = NULL;
    #pragma omp parallel shared(resultado)
    {
        int num_threads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();
        int chunk_size = d / num_threads;
        int start_index = thread_id * chunk_size;
        int end_index = (thread_id == num_threads - 1) ? d : (thread_id + 1) * chunk_size;
        int i;
        for (i = start_index; i < end_index; i++) {
            Assunto* a = aux->listaAssunto;
            do {
                if (a->id == idAssunto) {
                    #pragma omp critical
                    {
                        resultado = a;
                    }
                }
                a = a->next;
            } while (a != aux->listaAssunto);
            aux = aux->next;
        }
    }
    return resultado;
    }

/**
* Fun��o que retorna a estrutura do arm�rio solicitado pelo "id" seu id
*/
    Armario *buscarArmario(int idArmario, Armario *armarios) {
        Armario *aux = armarios;
        do {
            if(aux->id == idArmario)
                return aux;
            aux = aux->next;
        } while(aux != armarios);
    }

/**
* Fun��o que retorna o arm�rio onde se encontra um determinado doc
*/
    Armario *qualArmarioDoDoc(int idDoc, Armario *armarios, int d) {
        Armario *aux = armarios;
        int i;
        do {
            Doc *doc = aux->listaDoc;
            for(i = 0; i < d; i++) {
                if(doc->id == idDoc)
                    return aux;
                doc = doc->next;
            }
            aux = aux->next;
        } while(aux != armarios);
    }
    
