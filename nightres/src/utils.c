#include <stdio.h>
#include <string.h>
#include <time.h>
#include "utils.h"

// Funzione per pulire il buffer di input dopo una lettura
void pulisciInput() {
    int carattere;
    while ((carattere = getchar()) != '\n' && carattere != EOF) {
    }
}

//Stampa la data e l'ora in un formato leggibile
void stampaDataOra(time_t valoreTempo) {
    char buffer[64];
    struct tm *infoTempo = localtime(&valoreTempo); // Converte il tempo in una struttura tm per formattarlo
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M", infoTempo); // Formato: giorno/mese/anno ora:minuti
    printf("%s", buffer);
}

// Stampa una barra di progresso testuale proporzionale al valore rispetto al massimo
void stampaBarra(int valore, int massimo) { 
    int numeroBarre;                        
    int i;

    if (massimo <= 0) {
        massimo = 1;
    }

    numeroBarre = (valore * 20) / massimo;

    for (i = 0; i < numeroBarre; i++) {
        printf("|");
    }
    printf(" (%d)", valore);
}

// Ordina i tavoli in base al numero del tavolo usando il bubble sort
void ordinaTavoliPerNumero(CatalogoTavoli *catalogo) {
    int i, j;
    Tavolo *temp;

    //bubble sort per ordinare i tavoli in base al numero del tavolo
    for (i = 0; i < catalogo->numeroElementi - 1; i++) {
        for (j = 0; j < catalogo->numeroElementi - i - 1; j++) {
            if (catalogo->elementi[j]->numeroTavolo > catalogo->elementi[j + 1]->numeroTavolo) {
                temp = catalogo->elementi[j];
                catalogo->elementi[j] = catalogo->elementi[j + 1];
                catalogo->elementi[j + 1] = temp;
            }
        }
    }
}

// Mostra statistiche dettagliate sui tavoli, clienti e prenotazioni
void mostraStatistiche(CatalogoTavoli *catalogo, ElencoClienti *elenco, ArchivioPrenotazioni *archivio) {
    int i, j;
    int massimoPrenotazioniTavolo = 0; // non ce nessun tavolo prenotato
    int idTavoloTop = -1; // valore speciale perche non ce nessun tavolo prenotato
    int massimoPrenotazioniCliente = 0; // non ce nessun cliente con prenotazioni
    int idClienteTop = -1;// valore speciale perche non ce nessun cliente con prenotazioni
    int numeroNoShow = 0; // contatore per il numero di no-show
    int richiesteVip = 0, richiesteDancefloor = 0, richiesteLounge = 0, richiesteEsterno = 0; // contatori per le richieste di zona
    float incassoTotale = 0.0f; // incasso totale da tutte le prenotazioni

    for (i = 0; i < catalogo->numeroElementi; i++) { // continua fino a quando non ha controllato tutti i tavoli
        int conteggio = 0;
        for (j = 0; j < archivio->numeroElementi; j++) { // continua fino a quando non ha controllato tutte le prenotazioni
            if (archivio->elementi[j]->tavolo != NULL && archivio->elementi[j]->tavolo->id == catalogo->elementi[i]->id) {
                // se la prenotazione è associata al tavolo corrente, incrementa il conteggio
                conteggio++;
            }
        }
        if (conteggio > massimoPrenotazioniTavolo) { // se il conteggio per questo tavolo è maggiore del massimo attuale, aggiorna il massimo e l'id del tavolo top
            massimoPrenotazioniTavolo = conteggio; 
            idTavoloTop = catalogo->elementi[i]->id; // aggiorna l'id del tavolo più prenotato
        }
    }

    
    // Questo ciclo for itera attraverso tutti i clienti nell'elenco dei clienti. 
    //Per ogni cliente, conta quante prenotazioni ha effettuato scorrendo l'elenco delle prenotazioni. 
    //Se il numero di prenotazioni per un cliente supera il massimo attuale, aggiorna il massimo e memorizza l'id del cliente con più prenotazioni.

    for (i = 0; i < elenco->numeroElementi; i++) { 
        int conteggio = 0;
        NodoPrenotazione *corrente = elenco->elementi[i]->prenotazioni;
        while (corrente != NULL) {
            conteggio++;
            corrente = corrente->successivo;
        }
        if (conteggio > massimoPrenotazioniCliente) {
            massimoPrenotazioniCliente = conteggio;
            idClienteTop = elenco->elementi[i]->id;
        }
    }

    for (i = 0; i < archivio->numeroElementi; i++) {
        Prenotazione *prenotazione = archivio->elementi[i];

        if (strcmp(prenotazione->stato, "no_show") == 0) {
            numeroNoShow++;
        }

        if (prenotazione->tavolo != NULL) {
            if (strcmp(prenotazione->tavolo->zona, "VIP") == 0) {
                richiesteVip++;
            } else if (strcmp(prenotazione->tavolo->zona, "dancefloor") == 0) {
                richiesteDancefloor++;
            } else if (strcmp(prenotazione->tavolo->zona, "lounge") == 0) {
                richiesteLounge++;
            } else if (strcmp(prenotazione->tavolo->zona, "esterno") == 0) {
                richiesteEsterno++;
            }


            // Se la prenotazione è attiva, confermata o no-show, aggiunge il prezzo minimo del tavolo all'incasso totale
            if (strcmp(prenotazione->stato, "attiva") == 0 || strcmp(prenotazione->stato, "confermata") == 0 ||strcmp(prenotazione->stato, "no_show") == 0) {
                incassoTotale += prenotazione->tavolo->prezzoMinimo;
            }
        }
    }

    printf("\n--- STATISTICHE ---\n");
    printf("Tavolo piu' prenotato: %d con ", idTavoloTop);
    stampaBarra(massimoPrenotazioniTavolo, massimoPrenotazioniTavolo);
    printf("\n");

    printf("Cliente con piu' serate: %d con ", idClienteTop);
    stampaBarra(massimoPrenotazioniCliente, massimoPrenotazioniCliente);
    printf("\n");

    printf("Tasso di no-show: %d/%d\n", numeroNoShow, archivio->numeroElementi);

    {
        int massimoZona = richiesteVip;

        // Determina quale zona è la più richiesta confrontando le richieste per ciascuna zona e aggiornando il massimoZona di conseguenza
        if (richiesteDancefloor > massimoZona) massimoZona = richiesteDancefloor;
        if (richiesteLounge > massimoZona) massimoZona = richiesteLounge;
        if (richiesteEsterno > massimoZona) massimoZona = richiesteEsterno;

        printf("Zona piu' richiesta:\n");
        printf("VIP        "); 
        stampaBarra(richiesteVip, massimoZona); 
        printf("\n");
        printf("dancefloor "); 
        stampaBarra(richiesteDancefloor, massimoZona); 
        printf("\n");
        printf("lounge     "); 
        stampaBarra(richiesteLounge, massimoZona); 
        printf("\n");
        printf("esterno    "); 
        stampaBarra(richiesteEsterno, massimoZona); 
        printf("\n");
    }

    if (archivio->numeroElementi > 0) {
        printf("Incasso medio per serata: %.2f\n", incassoTotale / archivio->numeroElementi);
    } else {
        printf("Incasso medio per serata: 0.00\n");
    }
}