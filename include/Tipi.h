
#include <time.h>

#define MAX_STR 100

// STRUCT TAVOLO

typedef struct {
    int id;
    char zona[30];      // VIP, lounge, dancefloor
    int capienza;
    float prezzoMin;
    int attivo;
} Tavolo;

struct Cliente;
struct Prenotazione;

typedef struct NodoPren {
    struct Prenotazione *pren;
    struct NodoPren *next;
} NodoPren;

typedef struct Cliente {
    int id;
    char nome[MAX_STR];
    char telefono[30];
    char livello[20];   // standard, gold, VIP
    NodoPren *prenotazioni;
} Cliente;

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
//array dinamici
typedef struct {
    Tavolo **elementi;
    int numeroElementi;
    int capacita;
    int prossimoId;
} CatalogoTavoli;

typedef struct {
    Cliente **elementi;
    int numeroElementi;
    int capacita;
    int prossimoId;
} ElencoClienti;

typedef struct {
    Prenotazione **elementi;
    int numeroElementi;
    int capacita;
    int prossimoId;
} ArchivioPrenotazioni;

// CODA FIFO (LISTA ATTESA)

typedef struct NodoCoda {
    Cliente *cliente;
    char fascia[20];
    struct NodoCoda *next;
} NodoCoda;

typedef struct {
    NodoCoda *head;
    NodoCoda *tail;
} CodaAttesa;
