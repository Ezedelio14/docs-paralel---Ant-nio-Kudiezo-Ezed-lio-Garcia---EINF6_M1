#ifndef TAD_INCLUDED
#define TAD_INCLUDED

typedef struct armario Armario;
typedef struct documento Doc;
typedef struct assunto Assunto;

extern double *mediaArmario(Doc *listaDocs, int s);
extern Armario *buscarArmario(int idArmario, Armario *armarios);
extern Assunto *buscarAssunto(int idAssunto, Doc *doc, int d);
extern Armario *qualArmarioDoDoc(int idDoc, Armario *armarios, int d);
extern Armario *moverDoc(int idDoc, int idArmario, Armario *armarios, int d);
extern Doc *buscarDoc(int idDoc, Armario *armarios);
extern Armario *iniListaArmario();
extern Doc *iniListaDoc();
extern Assunto *iniListaAssunto();
extern Armario *insereArmario(Armario *a, int id);
extern Doc *insereDoc(Doc *d, int id);
extern Assunto *insereAssunto(Assunto *a, int id, double pontuacao);
extern Doc *removeDoc(Doc *listaDoc, int id);




#endif // TAD_INCLUDED
