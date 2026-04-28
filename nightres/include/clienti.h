#ifndef CLIENTI_H
#define CLIENTI_H

#include "tavoli.h"

struct Prenotazione;

typedef struct NodoPrenotazione {
    struct Prenotazione *prenotazione;
    struct NodoPrenotazione *successivo;
} NodoPrenotazione;

typedef struct {
    int id;
    char nome[MAX_STR];
    char telefono[30];
    char livelloFedelta[20];
    NodoPrenotazione *prenotazioni;
} Cliente;

typedef struct {
    Cliente **elementi;
    int numeroElementi;
    int capacita;
    int prossimoId;
} ElencoClienti;

void inizializzaElencoClienti(ElencoClienti *elenco);
void espandiElencoClienti(ElencoClienti *elenco);
Cliente *trovaClientePerId(ElencoClienti *elenco, int id);

void aggiungiCliente(ElencoClienti *elenco);
void eliminaCliente(ElencoClienti *elenco);
void visualizzaClienti(ElencoClienti *elenco);
void visualizzaStoricoCliente(ElencoClienti *elenco);

void aggiungiPrenotazioneACliente(Cliente *cliente, struct Prenotazione *prenotazione);

void liberaElencoClienti(ElencoClienti *elenco);

#endif