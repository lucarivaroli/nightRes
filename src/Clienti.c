#include <stdio.h>
#include <stdlib.h>
#include "Clienti.h"
#include "Utils.h"

void inizializzaElencoClienti(ElencoClienti *elenco) {
    elenco->elementi = NULL;
    elenco->numeroElementi = 0;
    elenco->capacita = 0;
    elenco->prossimoId = 1;
}

void espandiElencoClienti(ElencoClienti *elenco) {
    if (elenco->numeroElementi >= elenco->capacita) {
        if (elenco->capacita == 0) {
            elenco->capacita = 10;
        } else {
            elenco->capacita = elenco->capacita * 2;
        }

        elenco->elementi = realloc(elenco->elementi, elenco->capacita * sizeof(Cliente *));
    }
}

Cliente *trovaClientePerId(ElencoClienti *elenco, int id) {
    int i;
    for (i = 0; i < elenco->numeroElementi; i++) {
        if (elenco->elementi[i]->id == id) {
            return elenco->elementi[i];
        }
    }
    return NULL;
}

void aggiungiCliente(ElencoClienti *elenco) {
    Cliente *cliente = malloc(sizeof(Cliente));

    cliente->id = elenco->prossimoId++;
    cliente->prenotazioni = NULL;

    leggiStringa("Nome cliente: ", cliente->nome, sizeof(cliente->nome));
    leggiStringa("Telefono: ", cliente->telefono, sizeof(cliente->telefono));
    leggiStringa("Livello fedelta' (standard, gold, VIP): ", cliente->livelloFedelta, sizeof(cliente->livelloFedelta));

    espandiElencoClienti(elenco);
    elenco->elementi[elenco->numeroElementi++] = cliente;

    printf("Cliente aggiunto correttamente.\n");
}

void eliminaCliente(ElencoClienti *elenco) {
    int id;
    int i;
    int posizione = -1;

    printf("ID cliente da eliminare: ");
    scanf("%d", &id);
    pulisciInput();

    for (i = 0; i < elenco->numeroElementi; i++) {
        if (elenco->elementi[i]->id == id) {
            posizione = i;
            break;
        }
    }

    if (posizione == -1) {
        printf("Cliente non trovato.\n");
        return;
    }

    free(elenco->elementi[posizione]);

    for (i = posizione; i < elenco->numeroElementi - 1; i++) {
        elenco->elementi[i] = elenco->elementi[i + 1];
    }

    elenco->numeroElementi--;
    printf("Cliente eliminato.\n");
}

void visualizzaClienti(ElencoClienti *elenco) {
    int i;

    printf("\n--- CLIENTI ---\n");
    for (i = 0; i < elenco->numeroElementi; i++) {
        Cliente *cliente = elenco->elementi[i];
        printf("ID:%d | Nome:%s | Telefono:%s | Livello:%s\n",
               cliente->id, cliente->nome, cliente->telefono, cliente->livelloFedelta);
    }
}

void aggiungiPrenotazioneACliente(Cliente *cliente, struct Prenotazione *prenotazione) {
    NodoPrenotazione *nuovoNodo = malloc(sizeof(NodoPrenotazione));

    if (nuovoNodo == NULL) {
        printf("Errore allocazione memoria.\n");
        return;
    }

    nuovoNodo->prenotazione = prenotazione;
    nuovoNodo->successivo = cliente->prenotazioni;
    cliente->prenotazioni = nuovoNodo;
}

void visualizzaStoricoCliente(ElencoClienti *elenco) {
    int id;
    Cliente *cliente;
    NodoPrenotazione *corrente;

    printf("ID cliente: ");
    scanf("%d", &id);
    pulisciInput();

    cliente = trovaClientePerId(elenco, id);

    if (cliente == NULL) {
        printf("Cliente non trovato.\n");
        return;
    }

    printf("\n--- STORICO PRENOTAZIONI DI %s ---\n", cliente->nome);

    corrente = cliente->prenotazioni;

    if (corrente == NULL) {
        printf("Nessuna prenotazione presente.\n");
        return;
    }

    while (corrente != NULL) {
        printf("Prenotazione ID:%d | Tavolo:%d | Fascia:%s | Stato:%s | Caparra:%.2f\n",
               corrente->prenotazione->id,
               corrente->prenotazione->tavolo ? corrente->prenotazione->tavolo->numeroTavolo : -1,
               corrente->prenotazione->fasciaOraria,
               corrente->prenotazione->stato,
               corrente->prenotazione->caparra);
        corrente = corrente->successivo;
    }
}

void liberaElencoClienti(ElencoClienti *elenco) {
    int i;

    for (i = 0; i < elenco->numeroElementi; i++) {
        NodoPrenotazione *corrente = elenco->elementi[i]->prenotazioni;

        while (corrente != NULL) {
            NodoPrenotazione *temp = corrente;
            corrente = corrente->successivo;
            free(temp);
        }

        free(elenco->elementi[i]);
    }

    free(elenco->elementi);
}