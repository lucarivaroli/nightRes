#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> //al suo interno ci sono funzioni per gestire il tempo, come time() e localtime()
#include "prenotazioni.h"
#include "utils.h"

void inizializzaArchivioPrenotazioni(ArchivioPrenotazioni *archivio) {
    archivio->elementi = NULL;
    archivio->numeroElementi = 0;
    archivio->capacita = 0;
    archivio->prossimoId = 1;// ID univoco per ogni prenotazione, incrementato ad ogni aggiunta
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
        if (archivio->elementi[i]->id == id) { // controlla se ce una prenotazione con id uguale a quello cercato
            return archivio->elementi[i]; //restituisce la prenotazione trovata
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

// Funzione per inserire un cliente nella coda di attesa.
// creando un nuovo nodo e aggiornando i puntatori di inizio e fine della coda
void inserisciInCoda(CodaAttesa *coda, Cliente *cliente, const char *fasciaOraria) {
    NodoCoda *nuovoNodo = malloc(sizeof(NodoCoda));// crea un nuovo nodo per la coda di attesa

    nuovoNodo->cliente = cliente; // assegna il cliente al nodo
    strcpy(nuovoNodo->fasciaOraria, fasciaOraria); // copia la fascia oraria richiesta nel nodo
    nuovoNodo->successivo = NULL; // il nuovo nodo sarà l'ultimo, quindi il successivo è NULL

    if (coda->fine == NULL) {
        coda->inizio = nuovoNodo;
        coda->fine = nuovoNodo;
    } else {
        coda->fine->successivo = nuovoNodo;
        coda->fine = nuovoNodo;
    }
}

// Funzione per estrarre un cliente dalla coda di attesa.
// Rimuove il nodo in testa alla coda e restituisce un puntatore a quel nodo
NodoCoda *estraiDallaCoda(CodaAttesa *coda) {
    NodoCoda *temp; // variabile temporanea per memorizzare il nodo estratto

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

    // Scorre la coda di attesa e stampa le informazioni di ogni cliente in attesa, inclusa la fascia oraria richiesta
    while (corrente != NULL) { //continua finche ci sono nodi nella coda
        printf("Cliente ID:%d | Nome:%s | Fascia:%s\n",corrente->cliente->id,corrente->cliente->nome,corrente->fasciaOraria);
        corrente = corrente->successivo; // passa al nodo successivo nella coda
    }
}

int oraPerFascia(const char *fasciaOraria) {
    //trasforma la fascia oraria in un'ora specifica per creare l'orario di inizio del turno
    if (strcmp(fasciaOraria, "apertura") == 0) {
        return 22;// La fascia "apertura" inizia alle 22:00
    }
    if (strcmp(fasciaOraria, "prime_ore") == 0) {
        return 23;// La fascia "prime_ore" inizia alle 23:00
    }
    return 1;
}

time_t creaOrarioTurno(const char *fasciaOraria) {
    time_t adesso = time(NULL); //funzione della time.h che restituisce il tempo attuale in secondi dal 1 gennaio 1970
    struct tm orario = *localtime(&adesso);// Converte il tempo attuale in una struttura tm in modo da poterlo modificare piu facilmente
                                            // Imposta i minuti e i secondi a zero per creare un orario pulito basato sull'ora della fascia oraria

    orario.tm_min = 0; //azzera i minuti
    orario.tm_sec = 0;//azzera i secondi
    orario.tm_hour = oraPerFascia(fasciaOraria); //imposta l'ora in base alla fascia oraria

    // Se la fascia oraria è "late_night" e l'ora attuale è già passata, aggiunge un giorno per indicare la notte successiva
    if (strcmp(fasciaOraria, "late_night") == 0 && localtime(&adesso)->tm_hour > 12) {
        orario.tm_mday += 1; 
    }

    return mktime(&orario); //trasforma il valore in modo che il programma riesce a gestirlo
}

// Funzione per verificare se un tavolo è già occupato in una data fascia oraria.
int tavoloOccupatoInFascia(ArchivioPrenotazioni *archivio, int idTavolo, const char *fasciaOraria) {
    int i;

    for (i = 0; i < archivio->numeroElementi; i++) { // Scorre tutte le prenotazioni nell'archivio
        Prenotazione *prenotazione = archivio->elementi[i]; //prende una prenotazione alla volta

        // Controlla se la prenotazione è per il tavolo specificato, nella fascia oraria richiesta, e se lo stato è "attiva" o "confermata"
        if (prenotazione->tavolo != NULL && prenotazione->tavolo->id == idTavolo && strcmp(prenotazione->fasciaOraria, fasciaOraria) == 0 &&
        (strcmp(prenotazione->stato, "attiva") == 0 ||strcmp(prenotazione->stato, "confermata") == 0)) {
            return 1; // Il tavolo è occupato in quella fascia oraria
        }
    }

    return 0; // Il tavolo è libero in quella fascia oraria
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
        Tavolo *corrente = catalogo->elementi[i]; //legge tutti i tavoli nell'elenco

        if (corrente->attivo == 1 && !tavoloOccupatoInFascia(archivio, corrente->id, fasciaOraria)) { //! serve per invertire un valore logico
            printf("ID:%d | Numero:%d | Nome:%s | Zona:%s | Prezzo:%.2f\n",corrente->id, corrente->numeroTavolo, corrente->nomeTavolo,corrente->zona, corrente->prezzoMinimo);
        }
    }

    // Chiede all'utente di inserire l'ID del tavolo desiderato o 0 per essere inseriti in lista d'attesa
    printf("ID tavolo desiderato (0 per lista attesa): ");
    scanf("%d", &idTavolo);
    pulisciInput();

    // Se l'utente ha scelto un tavolo specifico, cerca quel tavolo nel catalogo
    if (idTavolo != 0) {
        tavolo = trovaTavoloPerId(catalogo, idTavolo);
    }

    // Se il tavolo non esiste, non è attivo, o è già occupato nella fascia oraria richiesta, inserisce il cliente in coda di attesa
    if (tavolo == NULL || tavolo->attivo == 0 || tavoloOccupatoInFascia(archivio, idTavolo, fasciaOraria)) {
        printf("Tavolo non disponibile. Cliente inserito in lista d'attesa.\n");
        inserisciInCoda(coda, cliente, fasciaOraria);
        return;
    }

    //le parentesi servono per raggruppare tutte le operazioni di creazione della prenotazione in un unico blocco, rendendo il codice piu leggibile e organizzato
    {
        Prenotazione *prenotazione = malloc(sizeof(Prenotazione)); //alloca dinamicamente memoria per una vuova prenotazione

        prenotazione->id = archivio->prossimoId++; //assegna un ID univoco alla prenotazione e incrementa il contatore per la prossima prenotazione
        prenotazione->tavolo = tavolo; // associa il tavolo scelto alla prenotazione
        prenotazione->cliente = cliente; // associa il cliente alla prenotazione
        strcpy(prenotazione->fasciaOraria, fasciaOraria); //copia la fascia oraria scelta nella struttura della prenotazione
        prenotazione->caparra = tavolo->prezzoMinimo * 0.30f; //calcola la caparra come il 30% del prezzo minimo del tavolo e la assegna alla prenotazione
        strcpy(prenotazione->stato, "attiva");
        prenotazione->creazione = time(NULL); //assegna alla prenotazione il tempo attuale come momento di creazione
        prenotazione->inizioTurno = creaOrarioTurno(fasciaOraria);//calcola l'orario dell'inizio turno e lo assegna alla prenotazione
        prenotazione->scadenzaNoShow = prenotazione->inizioTurno + 30 * 60; //calcola la scadenza per il no-show come 30 minuti dopo l'inizio del turno e la assegna alla prenotazione

        // Aggiunge la prenotazione all'archivio, espandendo l'archivio se necessario
        espandiArchivioPrenotazioni(archivio);
        archivio->elementi[archivio->numeroElementi++] = prenotazione;

        // Aggiunge la prenotazione alla lista delle prenotazioni del cliente
        aggiungiPrenotazioneACliente(cliente, prenotazione);

        printf("Prenotazione creata. ID:%d | Caparra: %.2f | Scadenza no-show: ",prenotazione->id, prenotazione->caparra);
        stampaDataOra(prenotazione->scadenzaNoShow); //stampa la data e ora della scadenza del no-show in un formato leggibile per l'utente
        printf("\n");
    }
}

void assegnaPrimoDellaCoda(ArchivioPrenotazioni *archivio, Tavolo *tavoloLiberato, CodaAttesa *coda) {
    NodoCoda *nodoCoda;

    if (codaVuota(coda)) {
        return;
    }

    nodoCoda = estraiDallaCoda(coda); //estrae il primo cliente dalla coda di attesa e lo memorizza in nodoCoda


    if (nodoCoda != NULL) { //se c'è un cliente in attesa, crea una nuova prenotazione per quel cliente e il tavolo appena liberato
        Prenotazione *prenotazione = malloc(sizeof(Prenotazione));

        prenotazione->id = archivio->prossimoId++; //assegna un ID univoco alla prenotazione e incrementa il contatore per la prossima prenotazione
        prenotazione->tavolo = tavoloLiberato;// associa il tavolo appena liberato alla prenotazione
        prenotazione->cliente = nodoCoda->cliente;// associa il cliente estratto dalla coda alla prenotazione
        strcpy(prenotazione->fasciaOraria, nodoCoda->fasciaOraria);//copia la fascia oraria richiesta dal cliente nella struttura della prenotazione
        prenotazione->caparra = tavoloLiberato->prezzoMinimo * 0.30f;//calcola la caparra come il 30% del prezzo minimo del tavolo e la assegna alla prenotazione
        strcpy(prenotazione->stato, "attiva");
        prenotazione->creazione = time(NULL);
        prenotazione->inizioTurno = creaOrarioTurno(prenotazione->fasciaOraria); //calcola l'orario dell'inizio turno e lo assegna alla prenotazione
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

    prenotazione = trovaPrenotazionePerId(archivio, id); //trova la prenotazione tramite id

    if (prenotazione == NULL) {
        printf("Prenotazione non trovata.\n");
        return;
    }

    
    if (strcmp(prenotazione->stato, "attiva") != 0) {
        printf("Prenotazione non cancellabile. Stato corrente: %s\n", prenotazione->stato);
        return;
    }

    penale = prenotazione->caparra * 0.50f; //calcola la penale e prende il 50% della caparra 
    strcpy(prenotazione->stato, "cancellata"); //aggiorna lo stato della prenotazione a "cancellata"

    printf("Prenotazione cancellata. Penale: %.2f euro\n", penale);

    assegnaPrimoDellaCoda(archivio, prenotazione->tavolo, coda); 
    //dopo aver cancellato la prenotazione, chiama la funzione assegnaPrimoDellaCoda per assegnare il tavolo liberato al primo cliente in coda di attesa
}

void visualizzaPrenotazioniSerata(ArchivioPrenotazioni *archivio) {
    int i;

    printf("\n--- PRENOTAZIONI SERATA ---\n");

   for (i = 0; i < archivio->numeroElementi; i++) {
    Prenotazione *prenotazione = archivio->elementi[i];

    if (prenotazione->cliente != NULL) { //controlla se ce un cliente associato alla prenotazione

        // Se il tavolo associato alla prenotazione non è NULL, stampa le informazioni della prenotazione
        if (prenotazione->tavolo != NULL) {
            // Stampa le informazioni della prenotazione, inclusi ID, nome del cliente, numero del tavolo, fascia oraria, stato e caparra
            printf("ID:%d | Cliente:%s | Tavolo:%d | Fascia:%s | Stato:%s | Caparra:%.2f\n",prenotazione->id,prenotazione->cliente->nome,prenotazione->tavolo->numeroTavolo,prenotazione->fasciaOraria,prenotazione->stato,prenotazione->caparra);

        } else {
            // Se il tavolo è NULL, stampa "N/D" al posto del numero del tavolo
            printf("ID:%d | Cliente:%s | Tavolo:N/D | Fascia:%s | Stato:%s | Caparra:%.2f\n",prenotazione->id,prenotazione->cliente->nome,prenotazione->fasciaOraria,prenotazione->stato,prenotazione->caparra);
        }

    } else {
        // Se il cliente associato alla prenotazione è NULL, stampa "N/D" al posto del nome del cliente e del numero del tavolo
        printf("ID:%d | Cliente:N/D | Tavolo:N/D | Fascia:%s | Stato:%s | Caparra:%.2f\n",prenotazione->id,prenotazione->fasciaOraria,prenotazione->stato,prenotazione->caparra);
    }
}
}

// Funzione per controllare le prenotazioni attive e aggiornare lo stato a "no_show" se la scadenza del no-show è passata
void controllaNoShow(ArchivioPrenotazioni *archivio) {
    int i;
    time_t adesso = time(NULL); // Ottiene il tempo attuale in secondi dal 1 gennaio 1970

    for (i = 0; i < archivio->numeroElementi; i++) {
        Prenotazione *prenotazione = archivio->elementi[i];

        // Controlla se la prenotazione è attiva e se la scadenza del no-show è passata
        if (strcmp(prenotazione->stato, "attiva") == 0 && adesso > prenotazione->scadenzaNoShow) {
            strcpy(prenotazione->stato, "no_show"); // Aggiorna lo stato della prenotazione a "no_show" se la scadenza è passata
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