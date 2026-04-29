#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "prenotazioni.h"
#include "utils.h"

void inizializzaArchivioPrenotazioni(ArchivioPrenotazioni *archivio) {
    archivio->elementi = NULL;
    archivio->numeroElementi = 0;
    archivio->capacita = 0;
    archivio->prossimoId = 1;
}

void espandiArchivioPrenotazioni(ArchivioPrenotazioni *archivio) {
    if (archivio->numeroElementi >= archivio->capacita) {
        if (archivio->capacita == 0) {
            archivio->capacita = 10;
        } else {
            archivio->capacita = archivio->capacita * 2;
        }

        archivio->elementi = realloc(archivio->elementi, archivio->capacita * sizeof(Prenotazione *));
    }
}

Prenotazione *trovaPrenotazionePerId(ArchivioPrenotazioni *archivio, int id) {
    int i;
    for (i = 0; i < archivio->numeroElementi; i++) {
        if (archivio->elementi[i]->id == id) {
            return archivio->elementi[i];
        }
    }
    return NULL;
}

void inizializzaCodaAttesa(CodaAttesa *coda) {
    coda->inizio = NULL;
    coda->fine = NULL;
}

int codaVuota(CodaAttesa *coda) {
    return coda->inizio == NULL;
}

void inserisciInCoda(CodaAttesa *coda, Cliente *cliente, const char *fasciaOraria) {
    NodoCoda *nuovoNodo = malloc(sizeof(NodoCoda));

    nuovoNodo->cliente = cliente;
    strcpy(nuovoNodo->fasciaOraria, fasciaOraria);
    nuovoNodo->successivo = NULL;

    if (coda->fine == NULL) {
        coda->inizio = nuovoNodo;
        coda->fine = nuovoNodo;
    } else {
        coda->fine->successivo = nuovoNodo;
        coda->fine = nuovoNodo;
    }
}

NodoCoda *estraiDallaCoda(CodaAttesa *coda) {
    NodoCoda *temp;

    if (coda->inizio == NULL) {
        return NULL;
    }

    temp = coda->inizio;
    coda->inizio = coda->inizio->successivo;

    if (coda->inizio == NULL) {
        coda->fine = NULL;
    }

    return temp;
}

void visualizzaCodaAttesa(CodaAttesa *coda) {
    NodoCoda *corrente = coda->inizio;

    printf("\n--- CODA DI ATTESA ---\n");

    if (corrente == NULL) {
        printf("Nessun cliente in attesa.\n");
        return;
    }

    while (corrente != NULL) {
        printf("Cliente ID:%d | Nome:%s | Fascia:%s\n",
               corrente->cliente->id,
               corrente->cliente->nome,
               corrente->fasciaOraria);
        corrente = corrente->successivo;
    }
}

int oraPerFascia(const char *fasciaOraria) {
    if (strcmp(fasciaOraria, "apertura") == 0) {
        return 22;
    }
    if (strcmp(fasciaOraria, "prime_ore") == 0) {
        return 23;
    }
    return 1;
}

time_t creaOrarioTurno(const char *fasciaOraria) {
    time_t adesso = time(NULL);
    struct tm orario = *localtime(&adesso);

    orario.tm_min = 0;
    orario.tm_sec = 0;
    orario.tm_hour = oraPerFascia(fasciaOraria);

    if (strcmp(fasciaOraria, "late_night") == 0 && localtime(&adesso)->tm_hour > 12) {
        orario.tm_mday += 1;
    }

    return mktime(&orario);
}

int tavoloOccupatoInFascia(ArchivioPrenotazioni *archivio, int idTavolo, const char *fasciaOraria) {
    int i;

    for (i = 0; i < archivio->numeroElementi; i++) {
        Prenotazione *prenotazione = archivio->elementi[i];

        if (prenotazione->tavolo != NULL && prenotazione->tavolo->id == idTavolo && strcmp(prenotazione->fasciaOraria, fasciaOraria) == 0 &&
        (strcmp(prenotazione->stato, "attiva") == 0 ||strcmp(prenotazione->stato, "confermata") == 0)) {
            return 1;
        }
    }

    return 0;
}

void creaPrenotazione(ArchivioPrenotazioni *archivio, CatalogoTavoli *catalogo, ElencoClienti *elenco, CodaAttesa *coda) {
    int idCliente;
    int idTavolo;
    int i;
    char fasciaOraria[20];
    Cliente *cliente;
    Tavolo *tavolo = NULL;

    visualizzaClienti(elenco);

    printf("ID cliente: ");
    scanf("%d", &idCliente);
    pulisciInput();

    cliente = trovaClientePerId(elenco, idCliente);
    if (cliente == NULL) {
        printf("Cliente non trovato.\n");
        return;
    }

    printf("Fascia oraria (apertura, prime_ore, late_night): ");
    scanf(" %[^\n]", fasciaOraria);
    

    printf("\n--- TAVOLI DISPONIBILI ---\n");
    for (i = 0; i < catalogo->numeroElementi; i++) {
        Tavolo *corrente = catalogo->elementi[i];

        if (corrente->attivo == 1 && !tavoloOccupatoInFascia(archivio, corrente->id, fasciaOraria)) {
            printf("ID:%d | Numero:%d | Nome:%s | Zona:%s | Prezzo:%.2f\n",corrente->id, corrente->numeroTavolo, corrente->nomeTavolo,
                   corrente->zona, corrente->prezzoMinimo);
        }
    }

    printf("ID tavolo desiderato (0 per lista attesa): ");
    scanf("%d", &idTavolo);
    pulisciInput();

    if (idTavolo != 0) {
        tavolo = trovaTavoloPerId(catalogo, idTavolo);
    }

    if (tavolo == NULL || tavolo->attivo == 0 || tavoloOccupatoInFascia(archivio, idTavolo, fasciaOraria)) {
        printf("Tavolo non disponibile. Cliente inserito in lista d'attesa.\n");
        inserisciInCoda(coda, cliente, fasciaOraria);
        return;
    }

    {
        Prenotazione *prenotazione = malloc(sizeof(Prenotazione));

        prenotazione->id = archivio->prossimoId++;
        prenotazione->tavolo = tavolo;
        prenotazione->cliente = cliente;
        strcpy(prenotazione->fasciaOraria, fasciaOraria);
        prenotazione->caparra = tavolo->prezzoMinimo * 0.30f;
        strcpy(prenotazione->stato, "attiva");
        prenotazione->creazione = time(NULL);
        prenotazione->inizioTurno = creaOrarioTurno(fasciaOraria);
        prenotazione->scadenzaNoShow = prenotazione->inizioTurno + 30 * 60;

        espandiArchivioPrenotazioni(archivio);
        archivio->elementi[archivio->numeroElementi++] = prenotazione;

        aggiungiPrenotazioneACliente(cliente, prenotazione);

        printf("Prenotazione creata. ID:%d | Caparra: %.2f | Scadenza no-show: ",
               prenotazione->id, prenotazione->caparra);
        stampaDataOra(prenotazione->scadenzaNoShow);
        printf("\n");
    }
}

void assegnaPrimoDellaCoda(ArchivioPrenotazioni *archivio, Tavolo *tavoloLiberato, CodaAttesa *coda) {
    NodoCoda *nodoCoda;

    if (codaVuota(coda)) {
        return;
    }

    nodoCoda = estraiDallaCoda(coda);

    if (nodoCoda != NULL) {
        Prenotazione *prenotazione = malloc(sizeof(Prenotazione));

        prenotazione->id = archivio->prossimoId++;
        prenotazione->tavolo = tavoloLiberato;
        prenotazione->cliente = nodoCoda->cliente;
        strcpy(prenotazione->fasciaOraria, nodoCoda->fasciaOraria);
        prenotazione->caparra = tavoloLiberato->prezzoMinimo * 0.30f;
        strcpy(prenotazione->stato, "attiva");
        prenotazione->creazione = time(NULL);
        prenotazione->inizioTurno = creaOrarioTurno(prenotazione->fasciaOraria);
        prenotazione->scadenzaNoShow = prenotazione->inizioTurno + 30 * 60;

        espandiArchivioPrenotazioni(archivio);
        archivio->elementi[archivio->numeroElementi++] = prenotazione;
        aggiungiPrenotazioneACliente(nodoCoda->cliente, prenotazione);

        printf("[NOTIFICA] Tavolo assegnato automaticamente a %s\n", nodoCoda->cliente->nome);

        free(nodoCoda);
    }
}

void cancellaPrenotazione(ArchivioPrenotazioni *archivio, CodaAttesa *coda) {
    int id;
    Prenotazione *prenotazione;
    float penale;

    printf("ID prenotazione da cancellare: ");
    scanf("%d", &id);
    pulisciInput();

    prenotazione = trovaPrenotazionePerId(archivio, id);

    if (prenotazione == NULL) {
        printf("Prenotazione non trovata.\n");
        return;
    }

    if (strcmp(prenotazione->stato, "attiva") != 0) {
        printf("Prenotazione non cancellabile. Stato corrente: %s\n", prenotazione->stato);
        return;
    }

    penale = prenotazione->caparra * 0.50f;
    strcpy(prenotazione->stato, "cancellata");

    printf("Prenotazione cancellata. Penale: %.2f euro\n", penale);

    assegnaPrimoDellaCoda(archivio, prenotazione->tavolo, coda);
}

void visualizzaPrenotazioniSerata(ArchivioPrenotazioni *archivio) {
    int i;

    printf("\n--- PRENOTAZIONI SERATA ---\n");

    for (i = 0; i < archivio->numeroElementi; i++) {
        Prenotazione *prenotazione = archivio->elementi[i];

        printf("ID:%d | Cliente:%s | Tavolo:%d | Fascia:%s | Stato:%s | Caparra:%.2f\n",
               prenotazione->id,
               prenotazione->cliente ? prenotazione->cliente->nome : "N/D",
               prenotazione->tavolo ? prenotazione->tavolo->numeroTavolo : -1,
               prenotazione->fasciaOraria,
               prenotazione->stato,
               prenotazione->caparra);
    }
}

void controllaNoShow(ArchivioPrenotazioni *archivio) {
    int i;
    time_t adesso = time(NULL);

    for (i = 0; i < archivio->numeroElementi; i++) {
        Prenotazione *prenotazione = archivio->elementi[i];

        if (strcmp(prenotazione->stato, "attiva") == 0 && adesso > prenotazione->scadenzaNoShow) {
            strcpy(prenotazione->stato, "no_show");
        }
    }

    printf("Controllo no-show completato.\n");
}

void liberaArchivioPrenotazioni(ArchivioPrenotazioni *archivio) {
    int i;
    for (i = 0; i < archivio->numeroElementi; i++) {
        free(archivio->elementi[i]);
    }
    free(archivio->elementi);
}

void liberaCodaAttesa(CodaAttesa *coda) {
    NodoCoda *corrente = coda->inizio;

    while (corrente != NULL) {
        NodoCoda *temp = corrente;
        corrente = corrente->successivo;
        free(temp);
    }
}