#ifndef PRENOTAZIONI_H
#define PRENOTAZIONI_H

#include <time.h>
#include "tavoli.h"
#include "clienti.h"

/** Struttura che rappresenta una prenotazione.
 * Contiene riferimenti al tavolo e al cliente, la fascia oraria,
 * l'importo della caparra, lo stato della prenotazione e i timestamp
 * per creazione, inizio turno e scadenza del no-show.
 */
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

/** Archivio dinamico delle prenotazioni.
 * Memorizza un array di puntatori e gestisce capacità e ID successivo.
 */
typedef struct {
    Prenotazione **elementi;
    int numeroElementi;
    int capacita;
    int prossimoId;
} ArchivioPrenotazioni;

/** Nodo della coda di attesa.
 * Ogni nodo contiene un cliente, la fascia oraria richiesta e il puntatore
 * al nodo successivo in coda.
 */
typedef struct NodoCoda {
    Cliente *cliente;
    char fasciaOraria[20];
    struct NodoCoda *successivo;
} NodoCoda;

/** Struttura per gestire la coda di attesa.
 * Mantiene i riferimenti all'inizio e alla fine della coda.
 */
typedef struct {
    NodoCoda *inizio;
    NodoCoda *fine;
} CodaAttesa;

/** Inizializza l'archivio delle prenotazioni.
 * @param archivio Puntatore all'archivio delle prenotazioni
 */
void inizializzaArchivioPrenotazioni(ArchivioPrenotazioni *archivio);

/** Espande la capacità dell'archivio delle prenotazioni quando necessario.
 * @param archivio Puntatore all'archivio delle prenotazioni
 */
void espandiArchivioPrenotazioni(ArchivioPrenotazioni *archivio);

/** Trova una prenotazione nell'archivio per ID.
 * @param archivio Puntatore all'archivio delle prenotazioni
 * @param id ID della prenotazione da trovare
 * @return Puntatore alla prenotazione trovata, o NULL se non trovata
 */
Prenotazione *trovaPrenotazionePerId(ArchivioPrenotazioni *archivio, int id);

/** Inizializza la coda di attesa.
 * @param coda Puntatore alla coda di attesa
 */
void inizializzaCodaAttesa(CodaAttesa *coda);

/** Verifica se la coda di attesa è vuota.
 * @param coda Puntatore alla coda di attesa
 * @return 1 se vuota, 0 altrimenti
 */
int codaVuota(CodaAttesa *coda);

/** Inserisce un cliente in coda per una data fascia oraria.
 * @param coda Puntatore alla coda di attesa
 * @param cliente Puntatore al cliente da inserire
 * @param fasciaOraria Fascia oraria richiesta
 */
void inserisciInCoda(CodaAttesa *coda, Cliente *cliente, const char *fasciaOraria);

/** Estrae il primo cliente dalla coda di attesa.
 * @param coda Puntatore alla coda di attesa
 * @return Puntatore al nodo estratto, o NULL se la coda è vuota
 */
NodoCoda *estraiDallaCoda(CodaAttesa *coda);

/** Visualizza la lista dei clienti in coda di attesa.
 * @param coda Puntatore alla coda di attesa
 */
void visualizzaCodaAttesa(CodaAttesa *coda);

/** Converte una fascia oraria in un timestamp per il turno.
 * @param fasciaOraria Stringa con la fascia oraria
 * @return Timestamp corrispondente al turno
 */
time_t creaOrarioTurno(const char *fasciaOraria);

/** Verifica se un tavolo è occupato in una data fascia oraria.
 * @param archivio Puntatore all'archivio delle prenotazioni
 * @param idTavolo ID del tavolo da controllare
 * @param fasciaOraria Fascia oraria da verificare
 * @return 1 se occupato, 0 se libero
 */
int tavoloOccupatoInFascia(ArchivioPrenotazioni *archivio, int idTavolo, const char *fasciaOraria);

/** Crea una nuova prenotazione e la aggiunge all'archivio.
 * @param archivio Archivio delle prenotazioni
 * @param catalogo Catalogo dei tavoli
 * @param elenco Elenco clienti
 * @param coda Coda di attesa
 */
void creaPrenotazione(ArchivioPrenotazioni *archivio, CatalogoTavoli *catalogo, ElencoClienti *elenco, CodaAttesa *coda);

/** Cancella una prenotazione esistente e gestisce la coda di attesa.
 * @param archivio Archivio delle prenotazioni
 * @param coda Coda di attesa
 */
void cancellaPrenotazione(ArchivioPrenotazioni *archivio, CodaAttesa *coda);

/** Visualizza tutte le prenotazioni della serata.
 * @param archivio Archivio delle prenotazioni
 */
void visualizzaPrenotazioniSerata(ArchivioPrenotazioni *archivio);

/** Controlla i no-show e aggiorna lo stato delle prenotazioni.
 * @param archivio Archivio delle prenotazioni
 */
void controllaNoShow(ArchivioPrenotazioni *archivio);

/** Assegna il primo cliente in coda a un tavolo liberato.
 * @param archivio Archivio delle prenotazioni
 * @param tavoloLiberato Tavolo appena liberato
 * @param coda Coda di attesa
 */
void assegnaPrimoDellaCoda(ArchivioPrenotazioni *archivio, Tavolo *tavoloLiberato, CodaAttesa *coda);

/** Libera la memoria allocata per l'archivio delle prenotazioni.
 * @param archivio Archivio delle prenotazioni
 */
void liberaArchivioPrenotazioni(ArchivioPrenotazioni *archivio);

/** Libera la memoria allocata per la coda di attesa.
 * @param coda Coda di attesa
 */
void liberaCodaAttesa(CodaAttesa *coda);

#endif