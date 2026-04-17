#include "Tavoli.h"
#include "Clienti.h"
#include "Prenotazioni.h"

void pulisciInput();
void leggiStringa(const char *messaggio, char *destinazione, int dimensione);
void stampaDataOra(time_t valoreTempo);
void stampaBarra(int valore, int massimo);

void ordinaTavoliPerNumero(CatalogoTavoli *catalogo);
void mostraStatistiche(CatalogoTavoli *catalogo, ElencoClienti *elenco, ArchivioPrenotazioni *archivio);