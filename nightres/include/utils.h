#ifndef UTILS_H
#define UTILS_H

#include <time.h>
#include "tavoli.h"
#include "clienti.h"
#include "prenotazioni.h"

/** Pulisce il buffer di input standard.
 * Viene usata dopo una lettura con scanf per evitare
 * che i caratteri residui interferiscano con input successivi.
 */
void pulisciInput();

/** Legge una stringa dall'utente.
 * @param messaggio Messaggio mostrato all'utente
 * @param destinazione Buffer dove salvare la stringa
 * @param dimensione Dimensione massima del buffer
 */
void leggiStringa(const char *messaggio, char *destinazione, int dimensione);

/** Stampa una data/ora formattata a partire da un timestamp.
 * @param valoreTempo Timestamp da visualizzare
 */
void stampaDataOra(time_t valoreTempo);

/** Stampa una barra di progresso o una linea di stato.
 * @param valore Valore corrente
 * @param massimo Valore massimo
 */
void stampaBarra(int valore, int massimo);

/** Ordina l'elenco dei tavoli in base al numero del tavolo.
 * @param catalogo Catalogo dei tavoli da ordinare
 */
void ordinaTavoliPerNumero(CatalogoTavoli *catalogo);

/** Mostra statistiche generali sul sistema.
 * @param catalogo Catalogo dei tavoli
 * @param elenco Elenco dei clienti
 * @param archivio Archivio delle prenotazioni
 */
void mostraStatistiche(CatalogoTavoli *catalogo, ElencoClienti *elenco, ArchivioPrenotazioni *archivio);

#endif