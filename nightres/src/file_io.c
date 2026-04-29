#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"

void salvaTavoli(CatalogoTavoli *catalogo) {
    FILE *file = fopen("data/tavoli.csv", "w");
    int i;

    if (file == NULL) {
        return;
    }

    for (i = 0; i < catalogo->numeroElementi; i++) {
        Tavolo *tavolo = catalogo->elementi[i];
        fprintf(file, "%d,%d,%s,%s,%d,%.2f,%d\n",tavolo->id,tavolo->numeroTavolo,tavolo->nomeTavolo,tavolo->zona,tavolo->capienza,tavolo->prezzoMinimo,tavolo->attivo);
    }

    fclose(file);
}

void salvaClienti(ElencoClienti *elenco) {
    FILE *file = fopen("data/clienti.csv", "w");
    int i;

    if (file == NULL) {
        return;
    }

    for (i = 0; i < elenco->numeroElementi; i++) {
        Cliente *cliente = elenco->elementi[i];
        fprintf(file, "%d,%s,%s,%s\n",cliente->id,cliente->nome,cliente->telefono,cliente->livelloFedelta);
    }

    fclose(file);
}

void salvaPrenotazioni(ArchivioPrenotazioni *archivio) {
    FILE *file = fopen("data/prenotazioni.csv", "w");
    int i, idTavolo, idCliente;

    if (file == NULL) {
        return;
    }

    for (i = 0; i < archivio->numeroElementi; i++) {

    Prenotazione *prenotazione = archivio->elementi[i];

    if (prenotazione->tavolo != NULL) {
        idTavolo = prenotazione->tavolo->id;
    } else {
        idTavolo = -1;// Valore speciale per indicare nessun tavolo associato
    }

    if (prenotazione->cliente != NULL) {
        idCliente = prenotazione->cliente->id;
    } else {
        idCliente = -1;// Valore speciale per indicare nessun cliente associato
    }

    fprintf(file, "%d,%d,%d,%s,%.2f,%s,%lld,%lld,%lld\n",prenotazione->id,idTavolo,idCliente,prenotazione->fasciaOraria,
            prenotazione->caparra,
            prenotazione->stato,
            (long long)prenotazione->creazione,
            (long long)prenotazione->inizioTurno,
            (long long)prenotazione->scadenzaNoShow);
}

  

    fclose(file);
}

void salvaStorico(ElencoClienti *elenco) {
    FILE *file = fopen("data/storico.csv", "w");
    int i;

    if (file == NULL) {
        return;
    }

    for (i = 0; i < elenco->numeroElementi; i++) {
        NodoPrenotazione *corrente = elenco->elementi[i]->prenotazioni;

        while (corrente != NULL) {
            fprintf(file, "%d,%d\n",
                    elenco->elementi[i]->id,
                    corrente->prenotazione->id);
            corrente = corrente->successivo;
        }
    }

    fclose(file);
}

void caricaTavoli(CatalogoTavoli *catalogo) {
    FILE *file = fopen("data/tavoli.csv", "r");
    char riga[256];

    if (file == NULL) {
        return;
    }

    while (fgets(riga, sizeof(riga), file) != NULL) {

        Tavolo *tavolo = malloc(sizeof(Tavolo));

        int letti = sscanf(riga,
            "%d,%d,%49[^,],%29[^,],%d,%f,%d",       //sscanf: legge un intero, un intero, una stringa fino a 49 caratteri o fino a virgola, una stringa fino a 29 caratteri o fino a virgola, un intero, un float, un intero
            &tavolo->id,
            &tavolo->numeroTavolo,
            tavolo->nomeTavolo,
            tavolo->zona,
            &tavolo->capienza,
            &tavolo->prezzoMinimo,
            &tavolo->attivo
        );

        if (letti != 7) {       // Controlla se sono stati letti tutti i campi necessari
            free(tavolo);
            continue;
        }

        espandiCatalogoTavoli(catalogo);
        catalogo->elementi[catalogo->numeroElementi++] = tavolo;

        if (tavolo->id >= catalogo->prossimoId) {
            catalogo->prossimoId = tavolo->id + 1;
        }
    }

    fclose(file);
}

void caricaClienti(ElencoClienti *elenco) {
    FILE *file = fopen("data/clienti.csv", "r");
    char riga[256];

    if (file == NULL) {
        return;
    }

    while (fgets(riga, sizeof(riga), file) != NULL) {

        Cliente *cliente = malloc(sizeof(Cliente));

        int letti = sscanf(riga,
            "%d,%99[^,],%29[^,],%19[^\n]",
            &cliente->id,
            cliente->nome,
            cliente->telefono,
            cliente->livelloFedelta
        );

        if (letti != 4) {
            free(cliente);
            continue;
        }

        cliente->prenotazioni = NULL;

        espandiElencoClienti(elenco);
        elenco->elementi[elenco->numeroElementi++] = cliente;

        if (cliente->id >= elenco->prossimoId) {
            elenco->prossimoId = cliente->id + 1;
        }
    }

    fclose(file);
}

void caricaPrenotazioni(ArchivioPrenotazioni *archivio, CatalogoTavoli *catalogo, ElencoClienti *elenco) {
    FILE *file = fopen("data/prenotazioni.csv", "r");
    char riga[512];

    if (file == NULL) {
        return;
    }

    while (fgets(riga, sizeof(riga), file) != NULL) {

        Prenotazione *prenotazione = malloc(sizeof(Prenotazione));

        int idTavolo;
        int idCliente;
        long long creazione;
        long long inizioTurno;
        long long scadenzaNoShow;

        int letti = sscanf(riga,
            "%d,%d,%d,%19[^,],%f,%19[^,],%lld,%lld,%lld",
            &prenotazione->id,
            &idTavolo,
            &idCliente,
            prenotazione->fasciaOraria,
            &prenotazione->caparra,
            prenotazione->stato,
            &creazione,
            &inizioTurno,
            &scadenzaNoShow
        );

        if (letti != 9) {
            free(prenotazione);
            continue;
        }

        prenotazione->creazione = (time_t)creazione;
        prenotazione->inizioTurno = (time_t)inizioTurno;
        prenotazione->scadenzaNoShow = (time_t)scadenzaNoShow;

        prenotazione->tavolo = trovaTavoloPerId(catalogo, idTavolo);
        prenotazione->cliente = trovaClientePerId(elenco, idCliente);

        espandiArchivioPrenotazioni(archivio);
        archivio->elementi[archivio->numeroElementi++] = prenotazione;

        if (prenotazione->id >= archivio->prossimoId) {
            archivio->prossimoId = prenotazione->id + 1;
        }

        if (prenotazione->cliente != NULL) {
            aggiungiPrenotazioneACliente(prenotazione->cliente, prenotazione);
        }
    }

    fclose(file);
}

void salvaTutto(CatalogoTavoli *catalogo, ElencoClienti *elenco, ArchivioPrenotazioni *archivio) {
    salvaTavoli(catalogo);
    salvaClienti(elenco);
    salvaPrenotazioni(archivio);
    salvaStorico(elenco);
    printf("Dati salvati correttamente nei file CSV.\n");
}