#include <stdio.h>
#include "include/tavoli.h"
#include "include/clienti.h"
#include "include/prenotazioni.h"
#include "include/file_io.h"
#include "include/utils.h"

void menuTavoli(CatalogoTavoli *catalogo) {
    int scelta;

    do {
        printf("\n--- MENU TAVOLI ---\n");
        printf("1. Aggiungi tavolo\n");
        printf("2. Modifica tavolo\n");
        printf("3. Elimina tavolo\n");
        printf("4. Cerca tavoli\n");
        printf("5. Visualizza tavoli\n");
        printf("6. Visualizza mappa tavoli\n");
        printf("0. Indietro\n");
        printf("Scelta: ");
        scanf("%d", &scelta);
        pulisciInput();

        switch (scelta) {
            case 1: aggiungiTavolo(catalogo); break;
            case 2: modificaTavolo(catalogo); break;
            case 3: eliminaTavolo(catalogo); break;
            case 4: cercaTavoli(catalogo); break;
            case 5: visualizzaTavoli(catalogo); break;
            case 6: visualizzaMappaTavoli(catalogo); break;
        }
    } while (scelta != 0);
}

void menuClienti(ElencoClienti *elenco) {
    int scelta;

    do {
        printf("\n--- MENU CLIENTI ---\n");
        printf("1. Registra cliente\n");
        printf("2. Elimina cliente\n");
        printf("3. Visualizza clienti\n");
        printf("4. Visualizza storico cliente\n");
        printf("0. Indietro\n");
        printf("Scelta: ");
        scanf("%d", &scelta);
        pulisciInput();

        switch (scelta) {
            case 1: aggiungiCliente(elenco); break;
            case 2: eliminaCliente(elenco); break;
            case 3: visualizzaClienti(elenco); break;
            case 4: visualizzaStoricoCliente(elenco); break;
        }
    } while (scelta != 0);
}

void menuPrenotazioni(ArchivioPrenotazioni *archivio, CatalogoTavoli *catalogo, ElencoClienti *elenco, CodaAttesa *coda) {
    int scelta;

    do {
        printf("\n--- MENU PRENOTAZIONI ---\n");
        printf("1. Crea prenotazione\n");
        printf("2. Cancella prenotazione\n");
        printf("3. Visualizza prenotazioni serata\n");
        printf("4. Controlla no-show\n");
        printf("5. Visualizza lista attesa\n");
        printf("0. Indietro\n");
        printf("Scelta: ");
        scanf("%d", &scelta);
        pulisciInput();

        switch (scelta) {
            case 1: creaPrenotazione(archivio, catalogo, elenco, coda); break;
            case 2: cancellaPrenotazione(archivio, coda); break;
            case 3: visualizzaPrenotazioniSerata(archivio); break;
            case 4: controllaNoShow(archivio); break;
            case 5: visualizzaCodaAttesa(coda); break;
        }
    } while (scelta != 0);
}

int main() {
    CatalogoTavoli catalogoTavoli;
    ElencoClienti elencoClienti;
    ArchivioPrenotazioni archivioPrenotazioni;
    CodaAttesa codaAttesa;
    int scelta;

    inizializzaCatalogoTavoli(&catalogoTavoli);
    inizializzaElencoClienti(&elencoClienti);
    inizializzaArchivioPrenotazioni(&archivioPrenotazioni);
    inizializzaCodaAttesa(&codaAttesa);

    caricaTavoli(&catalogoTavoli);
    caricaClienti(&elencoClienti);
    caricaPrenotazioni(&archivioPrenotazioni, &catalogoTavoli, &elencoClienti);

    do {
        printf("\n=============================\n");
        printf("         NIGHTRES\n");
        printf("=============================\n");
        printf("1. Gestione tavoli\n");
        printf("2. Gestione clienti\n");
        printf("3. Gestione prenotazioni\n");
        printf("4. Statistiche\n");
        printf("5. Salva dati\n");
        printf("0. Esci\n");
        printf("Scelta: ");
        scanf("%d", &scelta);
        pulisciInput();

        switch (scelta) {
            case 1:
                menuTavoli(&catalogoTavoli);
                break;
            case 2:
                menuClienti(&elencoClienti);
                break;
            case 3:
                menuPrenotazioni(&archivioPrenotazioni, &catalogoTavoli, &elencoClienti, &codaAttesa);
                break;
            case 4:
                mostraStatistiche(&catalogoTavoli, &elencoClienti, &archivioPrenotazioni);
                break;
            case 5:
                salvaTutto(&catalogoTavoli, &elencoClienti, &archivioPrenotazioni);
                break;
            case 0:
                salvaTutto(&catalogoTavoli, &elencoClienti, &archivioPrenotazioni);
                printf("Uscita dal programma.\n");
                break;
            default:
                printf("Scelta non valida.\n");
        }
    } while (scelta != 0);

    liberaArchivioPrenotazioni(&archivioPrenotazioni);
    liberaElencoClienti(&elencoClienti);
    liberaCatalogoTavoli(&catalogoTavoli);
    liberaCodaAttesa(&codaAttesa);

    return 0;
}
