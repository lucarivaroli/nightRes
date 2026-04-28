#ifndef TAVOLI_H
#define TAVOLI_H

#define MAX_STR 100

typedef struct {
    int id;
    int numeroTavolo;
    char nomeTavolo[50];
    char zona[30];
    int capienza;
    float prezzoMinimo;
    int attivo;
} Tavolo;

typedef struct {
    Tavolo **elementi;
    int numeroElementi;
    int capacita;
    int prossimoId;
} CatalogoTavoli;

void inizializzaCatalogoTavoli(CatalogoTavoli *catalogo);
void espandiCatalogoTavoli(CatalogoTavoli *catalogo);
Tavolo *trovaTavoloPerId(CatalogoTavoli *catalogo, int id);

void aggiungiTavolo(CatalogoTavoli *catalogo);
void modificaTavolo(CatalogoTavoli *catalogo);
void eliminaTavolo(CatalogoTavoli *catalogo);
void cercaTavoli(CatalogoTavoli *catalogo);
void visualizzaTavoli(CatalogoTavoli *catalogo);
void visualizzaMappaTavoli(CatalogoTavoli *catalogo);

void liberaCatalogoTavoli(CatalogoTavoli *catalogo);

#endif