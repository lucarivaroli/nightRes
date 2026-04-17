#include "Tavoli.h"
#include "Clienti.h"
#include "Prenotazioni.h"

void salvaTavoli(CatalogoTavoli *catalogo);
void salvaClienti(ElencoClienti *elenco);
void salvaPrenotazioni(ArchivioPrenotazioni *archivio);
void salvaStorico(ElencoClienti *elenco);

void caricaTavoli(CatalogoTavoli *catalogo);
void caricaClienti(ElencoClienti *elenco);
void caricaPrenotazioni(ArchivioPrenotazioni *archivio, CatalogoTavoli *catalogo, ElencoClienti *elenco);

void salvaTutto(CatalogoTavoli *catalogo, ElencoClienti *elenco, ArchivioPrenotazioni *archivio);
