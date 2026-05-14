#ifndef TAVOLI_H
#define TAVOLI_H

#define MAX_STR 100


/** Rappresenta un tavolo del ristorante.
 * Contiene informazioni identificative, nome, zona, capienza,
 * prezzo minimo e stato di attivazione.
 */
typedef struct {
    int id;
    int numeroTavolo;
    char nomeTavolo[50];
    char zona[30];
    int capienza;
    float prezzoMinimo;
    int attivo;
} Tavolo;

/** Catalogo dinamico dei tavoli.
 * Gestisce un array di puntatori a tavolo e traccia il numero
 * di elementi, la capacità attuale e l'ID successivo.
 */
typedef struct {
    Tavolo **elementi;
    int numeroElementi;
    int capacita;
    int prossimoId;
} CatalogoTavoli;

/** Inizializza il catalogo dei tavoli.
 * @param catalogo Puntatore al catalogo da inizializzare
 */
void inizializzaCatalogoTavoli(CatalogoTavoli *catalogo);

/** Espande la capacità del catalogo se necessario.
 * @param catalogo Puntatore al catalogo dei tavoli
 */
void espandiCatalogoTavoli(CatalogoTavoli *catalogo);

/** Cerca un tavolo nel catalogo tramite ID.
 * @param catalogo Puntatore al catalogo dei tavoli
 * @param id ID del tavolo da cercare
 * @return Puntatore al tavolo trovato, o NULL se non esiste
 */
Tavolo *trovaTavoloPerId(CatalogoTavoli *catalogo, int id);

/** Aggiunge un nuovo tavolo al catalogo.
 * @param catalogo Puntatore al catalogo dei tavoli
 */
void aggiungiTavolo(CatalogoTavoli *catalogo);

/** Modifica i dati di un tavolo esistente.
 * @param catalogo Puntatore al catalogo dei tavoli
 */
void modificaTavolo(CatalogoTavoli *catalogo);

/** Elimina un tavolo dal catalogo.
 * @param catalogo Puntatore al catalogo dei tavoli
 */
void eliminaTavolo(CatalogoTavoli *catalogo);

/** Cerca tavoli in base a criteri specifici
 * (es. zona, capienza, stato).
 * @param catalogo Puntatore al catalogo dei tavoli
 */
void cercaTavoli(CatalogoTavoli *catalogo);

/** Visualizza l'elenco dei tavoli presenti nel catalogo.
 * @param catalogo Puntatore al catalogo dei tavoli
 */
void visualizzaTavoli(CatalogoTavoli *catalogo);

/** Libera la memoria usata dal catalogo dei tavoli.
 * @param catalogo Puntatore al catalogo dei tavoli
 */
void liberaCatalogoTavoli(CatalogoTavoli *catalogo);


/**
 * Visualizza una mappa dei tavoli divisi per zona.
 *
 * @param catalogo Puntatore al catalogo dei tavoli.
 * @return void
 */
void visualizzaMappaTavoli(CatalogoTavoli *catalogo);

#endif