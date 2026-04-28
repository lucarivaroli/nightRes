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
        fprintf(file, "%d,%d,%s,%s,%d,%.2f,%d\n",
                tavolo->id,
                tavolo->numeroTavolo,
                tavolo->nomeTavolo,
                tavolo->zona,
                tavolo->capienza,
                tavolo->prezzoMinimo,
                tavolo->attivo);
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
        fprintf(file, "%d,%s,%s,%s\n",
                cliente->id,
                cliente->nome,
                cliente->telefono,
                cliente->livelloFedelta);
    }

    fclose(file);
}

void salvaPrenotazioni(ArchivioPrenotazioni *archivio) {
    FILE *file = fopen("data/prenotazioni.csv", "w");
    int i;

    if (file == NULL) {
        return;
    }

    for (i = 0; i < archivio->numeroElementi; i++) {
        Prenotazione *prenotazione = archivio->elementi[i];
        fprintf(file, "%d,%d,%d,%s,%.2f,%s,%lld,%lld,%lld\n",
                prenotazione->id,
                prenotazione->tavolo ? prenotazione->tavolo->id : -1,
                prenotazione->cliente ? prenotazione->cliente->id : -1,
                prenotazione->fasciaOraria,
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
        char *token = strtok(riga, ",");

        if (token == NULL) {
            free(tavolo);
            continue;
        }
        tavolo->id = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            free(tavolo);
            continue;
        }
        tavolo->numeroTavolo = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            free(tavolo);
            continue;
        }
        strcpy(tavolo->nomeTavolo, token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            free(tavolo);
            continue;
        }
        strcpy(tavolo->zona, token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            free(tavolo);
            continue;
        }
        tavolo->capienza = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            free(tavolo);
            continue;
        }
        tavolo->prezzoMinimo = (float)atof(token);

        token = strtok(NULL, ",\n");
        if (token == NULL) {
            free(tavolo);
            continue;
        }
        tavolo->attivo = atoi(token);

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
        char *token = strtok(riga, ",");

        if (token == NULL) {
            free(cliente);
            continue;
        }
        cliente->id = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            free(cliente);
            continue;
        }
        strcpy(cliente->nome, token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            free(cliente);
            continue;
        }
        strcpy(cliente->telefono, token);

        token = strtok(NULL, ",\n");
        if (token == NULL) {
            free(cliente);
            continue;
        }
        strcpy(cliente->livelloFedelta, token);
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
        char *token = strtok(riga, ",");

        if (token == NULL) {
            free(prenotazione);
            continue;
        }
        prenotazione->id = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            free(prenotazione);
            continue;
        }
        idTavolo = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            free(prenotazione);
            continue;
        }
        idCliente = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            free(prenotazione);
            continue;
        }
        strcpy(prenotazione->fasciaOraria, token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            free(prenotazione);
            continue;
        }
        prenotazione->caparra = (float)atof(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            free(prenotazione);
            continue;
        }
        strcpy(prenotazione->stato, token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            free(prenotazione);
            continue;
        }
        prenotazione->creazione = (time_t)atoll(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            free(prenotazione);
            continue;
        }
        prenotazione->inizioTurno = (time_t)atoll(token);

        token = strtok(NULL, ",\n");
        if (token == NULL) {
            free(prenotazione);
            continue;
        }
        prenotazione->scadenzaNoShow = (time_t)atoll(token);

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