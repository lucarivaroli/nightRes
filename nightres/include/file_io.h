#ifndef FILE_IO_H
#define FILE_IO_H

#include "tavoli.h"
#include "clienti.h"
#include "prenotazioni.h"

void salvaTavoli(CatalogoTavoli *catalogo);
void salvaClienti(ElencoClienti *elenco);
void salvaPrenotazioni(ArchivioPrenotazioni *archivio);
void salvaStorico(ElencoClienti *elenco);

void caricaTavoli(CatalogoTavoli *catalogo);
void caricaClienti(ElencoClienti *elenco);
void caricaPrenotazioni(ArchivioPrenotazioni *archivio, CatalogoTavoli *catalogo, ElencoClienti *elenco);

void salvaTutto(CatalogoTavoli *catalogo, ElencoClienti *elenco, ArchivioPrenotazioni *archivio);

#endif