#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"
// Salva i tavoli in un file CSV, includendo il loro ID, numero, nome, zona, capienza, prezzo minimo e stato di attivazione
void salvaTavoli(CatalogoTavoli *catalogo) {
    FILE *file = fopen("csv/Tavoli.csv", "w");
    int i;

    if (file == NULL) {
        return;
    }

    for (i = 0; i < catalogo->numeroElementi; i++) {
        Tavolo *tavolo = catalogo->elementi[i];
        fprintf(file, "id tavolo: %d,   numero: %d, nome: %s,   zona: %s,   capienza: %d,   prezzoMinimo: %.2f, attivo: %d\n",tavolo->id,tavolo->numeroTavolo,tavolo->nomeTavolo,tavolo->zona,tavolo->capienza,tavolo->prezzoMinimo,tavolo->attivo);
    }

    fclose(file);
}
// Salva i clienti in un file CSV, includendo il loro ID, nome, telefono e livello di fedeltà
void salvaClienti(ElencoClienti *elenco) {
    FILE *file = fopen("csv/Clienti.csv", "w");
    int i;

    if (file == NULL) {
        return;
    }

    for (i = 0; i < elenco->numeroElementi; i++) {
        Cliente *cliente = elenco->elementi[i];
        fprintf(file, "id: %d,nome: %s,telefono: %s,livelloFedelta: %s\n",cliente->id,cliente->nome,cliente->telefono,cliente->livelloFedelta);
    }

    fclose(file);
}
// Salva le prenotazioni in un file CSV, associando gli ID dei tavoli e clienti
void salvaPrenotazioni(ArchivioPrenotazioni *archivio) {
    FILE *file = fopen("csv/Prenotazioni.csv", "w");
    int i, idTavolo, idCliente;

    if (file == NULL) {
        return;
    }

    for (i = 0; i < archivio->numeroElementi; i++) {

    Prenotazione *prenotazione = archivio->elementi[i]; //prende una prenotazione alla volta

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

    fprintf(file, "id: %d,idTavolo: %d,idCliente: %d,fasciaOraria: %s,caparra: %.2f,stato: %s,creazione: %lld,inizioTurno: %lld,scadenzaNoShow: %lld\n",prenotazione->id,idTavolo,idCliente,prenotazione->fasciaOraria,prenotazione->caparra,prenotazione->stato,
            (long long)prenotazione->creazione,//long long perche e un numero intero di grandi dimensioni
            (long long)prenotazione->inizioTurno,
            (long long)prenotazione->scadenzaNoShow);
}

  

    fclose(file);
}
// Salva lo storico delle prenotazioni per ogni cliente in un file CSV
void salvaStorico(ElencoClienti *elenco) {
    FILE *file = fopen("csv/Storico.csv", "w");
    int i;

    if (file == NULL) {
        return;
    }

    for (i = 0; i < elenco->numeroElementi; i++) {
        NodoPrenotazione *corrente = elenco->elementi[i]->prenotazioni; //prende la lista delle prenotazioni del cliente corrente

        while (corrente != NULL) {
            fprintf(file, "id cliente: %d,id prenotazione: %d\n",elenco->elementi[i]->id,corrente->prenotazione->id); // Salva l'ID del cliente e l'ID della prenotazione associata al cliente
            corrente = corrente->successivo;
        }
    }

    fclose(file);
}
// Carica i tavoli dal file CSV e li aggiunge al catalogo
void caricaTavoli(CatalogoTavoli *catalogo) {
    FILE *file = fopen("csv/Tavoli.csv", "r");
    char riga[256];

    if (file == NULL) {
        return;
    }

    while (fgets(riga, sizeof(riga), file) != NULL) {

        Tavolo *tavolo = malloc(sizeof(Tavolo));

        // Utilizza sscanf per leggere i campi dalla riga, assicurandosi di gestire correttamente le stringhe con spazi
        int letti = 0;
        letti = sscanf(riga,"id: %d,numero: %d,nome: %49[^,],zona: %29[^,],capienza: %d,prezzoMinimo: %f,attivo: %d",&tavolo->id,&tavolo->numeroTavolo,tavolo->nomeTavolo,tavolo->zona,&tavolo->capienza,&tavolo->prezzoMinimo,&tavolo->attivo);

        if (letti != 7) {       // Controlla se sono stati letti tutti i campi necessari
            free(tavolo);
            continue; // Se la riga non è formattata correttamente, libera la memoria allocata per il tavolo e passa alla riga successiva
        }

        espandiCatalogoTavoli(catalogo);
        catalogo->elementi[catalogo->numeroElementi++] = tavolo; // Aggiunge il tavolo al catalogo e incrementa il numero di elementi

        if (tavolo->id >= catalogo->prossimoId) {
            catalogo->prossimoId = tavolo->id + 1;
        }
    }

    fclose(file);
}
// Carica i clienti dal file CSV e inizializza le loro prenotazioni come NULL
void caricaClienti(ElencoClienti *elenco) {
    FILE *file = fopen("csv/Clienti.csv", "r");
    char riga[256];

    if (file == NULL) {
        return;
    }

    while (fgets(riga, sizeof(riga), file) != NULL) {

        Cliente *cliente = malloc(sizeof(Cliente));

        int letti = 0;

        letti = sscanf(riga,"id: %d,nome: %99[^,],telefono: %29[^,],livelloFedelta: %19[^\n]",&cliente->id,cliente->nome,cliente->telefono,cliente->livelloFedelta);

        if (letti != 4) {
            free(cliente);
            continue; // Se la riga non è formattata correttamente, libera la memoria allocata per il cliente e passa alla riga successiva
        }

        cliente->prenotazioni = NULL; 

        espandiElencoClienti(elenco); // Assicura che ci sia spazio extra per il nuovo cliente grazie alla reallocazione dinamica
        elenco->elementi[elenco->numeroElementi++] = cliente; // Aggiunge il cliente all'elenco e incrementa il numero di elementi

        // Aggiorna il prossimo ID disponibile se l'ID del cliente appena caricato è maggiore o uguale al prossimo ID disponibile
        if (cliente->id >= elenco->prossimoId) {
            elenco->prossimoId = cliente->id + 1; 
        }
    }

    fclose(file);
}
// Carica le prenotazioni dal file CSV e le associa ai tavoli e clienti corrispondenti
void caricaPrenotazioni(ArchivioPrenotazioni *archivio, CatalogoTavoli *catalogo, ElencoClienti *elenco) {
    FILE *file = fopen("csv/Prenotazioni.csv", "r");
    char riga[512];

    if (file == NULL) {
        return;
    }

    while (fgets(riga, sizeof(riga), file) != NULL) {

        Prenotazione *prenotazione = malloc(sizeof(Prenotazione));

        int idTavolo;
        int idCliente;
        long long creazione; //si utilizza la long long per rappresentare il tempo in secondi
        long long inizioTurno; 
        long long scadenzaNoShow;

        int letti = 0;
        letti = sscanf(riga,"id: %d,idTavolo: %d,idCliente: %d,fasciaOraria: %19[^,],caparra: %f,stato: %19[^,]",&prenotazione->id,&idTavolo,&idCliente,prenotazione->fasciaOraria,&prenotazione->caparra,prenotazione->stato);

        if (letti != 9) {
            free(prenotazione);
            continue;
        }

        //i valori di tempo sono stati salvati come long long (rappresentando il numero di secondi) e devono essere convertiti in time_t per essere utilizzati correttamente nel programma
        prenotazione->creazione = (time_t)creazione; //converte il valore letto da long long a time_t
        prenotazione->inizioTurno = (time_t)inizioTurno;
        prenotazione->scadenzaNoShow = (time_t)scadenzaNoShow;

        prenotazione->tavolo = trovaTavoloPerId(catalogo, idTavolo);// Associa il tavolo alla prenotazione cercandolo nel catalogo tramite l'ID del tavolo
        prenotazione->cliente = trovaClientePerId(elenco, idCliente);// Associa il cliente alla prenotazione cercandolo nell'elenco tramite l'ID del cliente

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
// Salva tutti i dati (tavoli, clienti, prenotazioni e storico) nei rispettivi file CSV
void salvaTutto(CatalogoTavoli *catalogo, ElencoClienti *elenco, ArchivioPrenotazioni *archivio) {
    salvaTavoli(catalogo);
    salvaClienti(elenco);
    salvaPrenotazioni(archivio);
    salvaStorico(elenco);
    printf("Dati salvati correttamente nei file CSV.\n");
}