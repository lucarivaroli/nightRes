#ifndef FILE_IO_H 
#define FILE_IO_H

#include "tavoli.h"
#include "clienti.h"
#include "prenotazioni.h"

/** Salva i tavoli su file
 * @param catalogo Puntatore al catalogo dei tavoli
 * @return void
 */
void salvaTavoli(CatalogoTavoli *catalogo);

/** Salva i clienti su file
 * @param elenco Puntatore all'elenco dei clienti
 * @return void
 */
void salvaClienti(ElencoClienti *elenco);

/** Salva le prenotazioni su file
 * @param archivio Puntatore all'archivio delle prenotazioni
 * @return void
 */
void salvaPrenotazioni(ArchivioPrenotazioni *archivio);

/** Salva lo storico dei clienti su file
 * @param elenco Puntatore all'elenco dei clienti
 * @return void
 */
void salvaStorico(ElencoClienti *elenco);

/** Carica i tavoli da file
 * @param catalogo Puntatore al catalogo dei tavoli
 * @return void
 */
void caricaTavoli(CatalogoTavoli *catalogo);

/** Carica i clienti da file
 * @param elenco Puntatore all'elenco dei clienti
 * @return void
 */
void caricaClienti(ElencoClienti *elenco);

/** Carica le prenotazioni da file
 * @param archivio Puntatore all'archivio delle prenotazioni
 * @param catalogo Puntatore al catalogo dei tavoli
 * @param elenco Puntatore all'elenco dei clienti
 * @return void
 */
void caricaPrenotazioni(ArchivioPrenotazioni *archivio, CatalogoTavoli *catalogo, ElencoClienti *elenco);

/** Salva tutto su file
 * @param catalogo Puntatore al catalogo dei tavoli
 * @param elenco Puntatore all'elenco dei clienti
 * @param archivio Puntatore all'archivio delle prenotazioni
 * @return void
 */
void salvaTutto(CatalogoTavoli *catalogo, ElencoClienti *elenco, ArchivioPrenotazioni *archivio);

#endif