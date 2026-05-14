#ifndef TIPI_H
#define TIPI_H

#include <time.h>

#define MAX_STR 100

/** Rappresenta un tavolo nel locale.
 * Contiene ID, zona di appartenenza, capienza massima,
 * prezzo minimo e stato di attivazione.
 */
typedef struct {
    int id;
    char zona[30];      // VIP, lounge, dancefloor
    int capienza;
    float prezzoMin;
    int attivo;
} Tavolo;

struct Cliente;
struct Prenotazione;

/** Nodo singolo della lista delle prenotazioni di un cliente.
 * Contiene il puntatore alla prenotazione e il puntatore
 * al nodo successivo.
 */
typedef struct NodoPren {
    struct Prenotazione *pren;
    struct NodoPren *next;
} NodoPren;

/** Rappresenta un cliente.
 * Contiene informazioni anagrafiche, livello fedeltà
 * e la lista delle prenotazioni associate.
 */
typedef struct Cliente {
    int id;
    char nome[MAX_STR];
    char telefono[30];
    char livello[20];   // standard, gold, VIP
    NodoPren *prenotazioni;
} Cliente;

/** Rappresenta una prenotazione.
 * Contiene riferimenti a tavolo e cliente, fascia oraria,
 * caparra, stato e timestamp di creazione e validità.
 */
typedef struct Prenotazione {
    int id;
    Tavolo *tavolo;
    Cliente *cliente;
    char fascia[20];     // apertura, prime_ore, late_night
    float caparra;
    char stato[20];      // attiva, cancellata, no_show
    time_t creazione;
    time_t inizioTurno;
    time_t scadenzaNoShow;
} Prenotazione;

/** Catalogo dinamico dei tavoli.
 * Gestisce array dinamico, numero di elementi e ID successivo.
 */
typedef struct {
    Tavolo **elementi;
    int numeroElementi;
    int capacita;
    int prossimoId;
} CatalogoTavoli;

/** Elenco dinamico dei clienti.
 * Utilizzato per gestire la lista dei clienti registrati.
 */
typedef struct {
    Cliente **elementi;
    int numeroElementi;
    int capacita;
    int prossimoId;
} ElencoClienti;

/** Archivio dinamico delle prenotazioni.
 * Tiene traccia di tutte le prenotazioni create.
 */
typedef struct {
    Prenotazione **elementi;
    int numeroElementi;
    int capacita;
    int prossimoId;
} ArchivioPrenotazioni;

/** Nodo della coda di attesa.
 * Contiene cliente e fascia oraria richiesta.
 */
typedef struct NodoCoda {
    Cliente *cliente;
    char fascia[20];
    struct NodoCoda *next;
} NodoCoda;

/** Struttura della coda di attesa.
 * Mantiene puntatori a testa e coda per accesso FIFO.
 */
typedef struct {
    NodoCoda *head;
    NodoCoda *tail;
} CodaAttesa;

#endif // TIPI_H
