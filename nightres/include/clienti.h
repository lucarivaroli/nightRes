#ifndef CLIENTI_H
#define CLIENTI_H

#include "tavoli.h"

struct Prenotazione;

/** Nodo della lista delle prenotazioni di un cliente.
 * Ogni nodo contiene un puntatore a una prenotazione e il puntatore
 * al nodo successivo nella lista.
 */
typedef struct NodoPrenotazione {
    struct Prenotazione *prenotazione;
    struct NodoPrenotazione *successivo;
} NodoPrenotazione;

/** Rappresenta un cliente della discoteca.
 * Contiene informazioni anagrafiche, livello di fedeltà e la lista
 * delle prenotazioni associate al cliente.
 */
typedef struct {
    int id;
    char nome[MAX_STR];
    char telefono[30];
    char livelloFedelta[20];
    NodoPrenotazione *prenotazioni;
} Cliente;

/** Rappresenta un elenco di clienti.
 * Contiene un array di puntatori ai clienti e gestisce capacità e ID successivo.
 */
typedef struct {
    Cliente **elementi;
    int numeroElementi;
    int capacita;
    int prossimoId;
} ElencoClienti;

/** Inizializza un elenco di clienti 
 * @param elenco Puntatore all'elenco da inizializzare
 * @return void
*/
void inizializzaElencoClienti(ElencoClienti *elenco);

/** Espande la capacità dell'elenco di clienti quando necessario
 * @param elenco Puntatore all'elenco da espandere
 * @return void
 */
void espandiElencoClienti(ElencoClienti *elenco);

/** Trova un cliente nell'elenco per ID
 * @param elenco Puntatore all'elenco di clienti
 * @param id ID del cliente da trovare
 * @return Puntatore al cliente trovato, o NULL se non trovato
 */
Cliente *trovaClientePerId(ElencoClienti *elenco, int id);

/** Trova un cliente nell'elenco per nome o telefono
 * @param elenco Puntatore all'elenco di clienti
 * @param chiave Stringa da cercare (nome o telefono)
 * @return Puntatore al cliente trovato, o NULL se non trovato
 */

/** Aggiunge un cliente all'elenco
 * @param elenco Puntatore all'elenco di clienti
 * @return void
 */
void aggiungiCliente(ElencoClienti *elenco);

/** Elimina un cliente dall'elenco
 * @param elenco Puntatore all'elenco di clienti
 * @return void
 */
void eliminaCliente(ElencoClienti *elenco);

/** Visualizza tutti i clienti dell'elenco
 * @param elenco Puntatore all'elenco di clienti
 * @return void
 */
void visualizzaClienti(ElencoClienti *elenco);

/** Visualizza lo storico di un cliente
 * @param elenco Puntatore all'elenco di clienti
 * @return void
 */
void visualizzaStoricoCliente(ElencoClienti *elenco);

/** Aggiunge una prenotazione alla lista di prenotazioni di un cliente
 * @param cliente Puntatore al cliente a cui aggiungere la prenotazione
 * @param prenotazione Puntatore alla prenotazione da aggiungere
 * @return void
 */
void aggiungiPrenotazioneACliente(Cliente *cliente, struct Prenotazione *prenotazione);

/** Libera la memoria allocata per l'elenco di clienti e le relative prenotazioni
 * @param elenco Puntatore all'elenco di clienti da liberare
 * @return void
 */
void liberaElencoClienti(ElencoClienti *elenco);

#endif