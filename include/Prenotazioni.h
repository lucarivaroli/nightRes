#include <time.h>
#include "Clienti.h"
#include "Tavoli.h"

typedef struct Prenotazione {
    int id;
    Tavolo *tavolo;
    Cliente *cliente;
    char fasciaOraria[20];
    float caparra;
    char stato[20];
    time_t creazione;
    time_t inizioTurno;
    time_t scadenzaNoShow;
} Prenotazione;

typedef struct {
    Prenotazione **elementi;
    int numeroElementi;
    int capacita;
    int prossimoId;
} ArchivioPrenotazioni;

typedef struct NodoCoda {
    Cliente *cliente;
    char fasciaOraria[20];
    struct NodoCoda *successivo;
} NodoCoda;

typedef struct {
    NodoCoda *inizio;
    NodoCoda *fine;
} CodaAttesa;

void inizializzaArchivioPrenotazioni(ArchivioPrenotazioni *archivio);
void espandiArchivioPrenotazioni(ArchivioPrenotazioni *archivio);
Prenotazione *trovaPrenotazionePerId(ArchivioPrenotazioni *archivio, int id);

void inizializzaCodaAttesa(CodaAttesa *coda);
int codaVuota(CodaAttesa *coda);
void inserisciInCoda(CodaAttesa *coda, Cliente *cliente, const char *fasciaOraria);
NodoCoda *estraiDallaCoda(CodaAttesa *coda);
void visualizzaCodaAttesa(CodaAttesa *coda);

time_t creaOrarioTurno(const char *fasciaOraria);
int tavoloOccupatoInFascia(ArchivioPrenotazioni *archivio, int idTavolo, const char *fasciaOraria);

void creaPrenotazione(ArchivioPrenotazioni *archivio, CatalogoTavoli *catalogo, ElencoClienti *elenco, CodaAttesa *coda);
void cancellaPrenotazione(ArchivioPrenotazioni *archivio, CodaAttesa *coda);
void visualizzaPrenotazioniSerata(ArchivioPrenotazioni *archivio);
void controllaNoShow(ArchivioPrenotazioni *archivio);
void assegnaPrimoDellaCoda(ArchivioPrenotazioni *archivio, Tavolo *tavoloLiberato, CodaAttesa *coda);

void liberaArchivioPrenotazioni(ArchivioPrenotazioni *archivio);
void liberaCodaAttesa(CodaAttesa *coda);