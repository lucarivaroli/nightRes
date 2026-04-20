#include <stdio.h>
#include <string.h>
#include <time.h>
#include "utils.h"

void pulisciInput() {
    int carattere;
    while ((carattere = getchar()) != '\n' && carattere != EOF) {
    }
}

void leggiStringa(const char *messaggio, char *destinazione, int dimensione) {
    printf("%s", messaggio);
    if (fgets(destinazione, dimensione, stdin) != NULL) {
        destinazione[strcspn(destinazione, "\n")] = '\0';
    }
}

void stampaDataOra(time_t valoreTempo) {
    char buffer[64];
    struct tm *infoTempo = localtime(&valoreTempo);
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M", infoTempo);
    printf("%s", buffer);
}

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

void ordinaTavoliPerNumero(CatalogoTavoli *catalogo) {
    int i, j;
    Tavolo *temp;

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

void mostraStatistiche(CatalogoTavoli *catalogo, ElencoClienti *elenco, ArchivioPrenotazioni *archivio) {
    int i, j;
    int massimoPrenotazioniTavolo = 0;
    int idTavoloTop = -1;
    int massimoPrenotazioniCliente = 0;
    int idClienteTop = -1;
    int numeroNoShow = 0;
    int richiesteVip = 0, richiesteDancefloor = 0, richiesteLounge = 0, richiesteEsterno = 0;
    float incassoTotale = 0.0f;

    for (i = 0; i < catalogo->numeroElementi; i++) {
        int conteggio = 0;
        for (j = 0; j < archivio->numeroElementi; j++) {
            if (archivio->elementi[j]->tavolo != NULL &&
                archivio->elementi[j]->tavolo->id == catalogo->elementi[i]->id) {
                conteggio++;
            }
        }
        if (conteggio > massimoPrenotazioniTavolo) {
            massimoPrenotazioniTavolo = conteggio;
            idTavoloTop = catalogo->elementi[i]->id;
        }
    }

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

            if (strcmp(prenotazione->stato, "attiva") == 0 ||
                strcmp(prenotazione->stato, "confermata") == 0 ||
                strcmp(prenotazione->stato, "no_show") == 0) {
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
        if (richiesteDancefloor > massimoZona) massimoZona = richiesteDancefloor;
        if (richiesteLounge > massimoZona) massimoZona = richiesteLounge;
        if (richiesteEsterno > massimoZona) massimoZona = richiesteEsterno;

        printf("Zona piu' richiesta:\n");
        printf("VIP        "); stampaBarra(richiesteVip, massimoZona); printf("\n");
        printf("dancefloor "); stampaBarra(richiesteDancefloor, massimoZona); printf("\n");
        printf("lounge     "); stampaBarra(richiesteLounge, massimoZona); printf("\n");
        printf("esterno    "); stampaBarra(richiesteEsterno, massimoZona); printf("\n");
    }

    if (archivio->numeroElementi > 0) {
        printf("Incasso medio per serata: %.2f\n", incassoTotale / archivio->numeroElementi);
    } else {
        printf("Incasso medio per serata: 0.00\n");
    }
}