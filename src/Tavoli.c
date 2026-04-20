#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tavoli.h"
#include "utils.h"

void inizializzaCatalogoTavoli(CatalogoTavoli *catalogo) {
    catalogo->elementi = NULL;
    catalogo->numeroElementi = 0;
    catalogo->capacita = 0;
    catalogo->prossimoId = 1;
}

void espandiCatalogoTavoli(CatalogoTavoli *catalogo) {
    if (catalogo->numeroElementi >= catalogo->capacita) {
        if (catalogo->capacita == 0) {
            catalogo->capacita = 10;
        } else {
            catalogo->capacita = catalogo->capacita * 2;
        }

        catalogo->elementi = realloc(catalogo->elementi, catalogo->capacita * sizeof(Tavolo *));
    }
}

Tavolo *trovaTavoloPerId(CatalogoTavoli *catalogo, int id) {
    int i;
    for (i = 0; i < catalogo->numeroElementi; i++) {
        if (catalogo->elementi[i]->id == id) {
            return catalogo->elementi[i];
        }
    }
    return NULL;
}

void aggiungiTavolo(CatalogoTavoli *catalogo) {
    Tavolo *tavolo = malloc(sizeof(Tavolo));

    tavolo->id = catalogo->prossimoId++;
    printf("Numero tavolo: ");
    scanf("%d", &tavolo->numeroTavolo);
    pulisciInput();

    leggiStringa("Nome tavolo: ", tavolo->nomeTavolo, sizeof(tavolo->nomeTavolo));
    leggiStringa("Zona (VIP, dancefloor, lounge, esterno): ", tavolo->zona, sizeof(tavolo->zona));

    printf("Capienza: ");
    scanf("%d", &tavolo->capienza);

    printf("Prezzo minimo: ");
    scanf("%f", &tavolo->prezzoMinimo);
    pulisciInput();

    tavolo->attivo = 1;

    espandiCatalogoTavoli(catalogo);
    catalogo->elementi[catalogo->numeroElementi++] = tavolo;

    printf("Tavolo aggiunto correttamente.\n");
}

void modificaTavolo(CatalogoTavoli *catalogo) {
    int id;
    Tavolo *tavolo;

    printf("ID tavolo da modificare: ");
    scanf("%d", &id);
    pulisciInput();

    tavolo = trovaTavoloPerId(catalogo, id);

    if (tavolo == NULL) {
        printf("Tavolo non trovato.\n");
        return;
    }

    printf("Nuovo numero tavolo: ");
    scanf("%d", &tavolo->numeroTavolo);
    pulisciInput();

    leggiStringa("Nuovo nome tavolo: ", tavolo->nomeTavolo, sizeof(tavolo->nomeTavolo));
    leggiStringa("Nuova zona: ", tavolo->zona, sizeof(tavolo->zona));

    printf("Nuova capienza: ");
    scanf("%d", &tavolo->capienza);

    printf("Nuovo prezzo minimo: ");
    scanf("%f", &tavolo->prezzoMinimo);
    pulisciInput();

    printf("Tavolo modificato.\n");
}

void eliminaTavolo(CatalogoTavoli *catalogo) {
    int id;
    Tavolo *tavolo;

    printf("ID tavolo da eliminare: ");
    scanf("%d", &id);
    pulisciInput();

    tavolo = trovaTavoloPerId(catalogo, id);

    if (tavolo == NULL) {
        printf("Tavolo non trovato.\n");
        return;
    }

    tavolo->attivo = 0;
    printf("Tavolo disattivato.\n");
}

void cercaTavoli(CatalogoTavoli *catalogo) {
    char zona[30];
    int capienzaMinima;
    float prezzoMinimo;
    int i;

    leggiStringa("Zona da cercare: ", zona, sizeof(zona));

    printf("Capienza minima: ");
    scanf("%d", &capienzaMinima);

    printf("Prezzo minimo richiesto: ");
    scanf("%f", &prezzoMinimo);
    pulisciInput();

    printf("\n--- RISULTATI RICERCA TAVOLI ---\n");
    for (i = 0; i < catalogo->numeroElementi; i++) {
        Tavolo *tavolo = catalogo->elementi[i];

        if (tavolo->attivo == 1 &&
            strstr(tavolo->zona, zona) != NULL &&
            tavolo->capienza >= capienzaMinima &&
            tavolo->prezzoMinimo >= prezzoMinimo) {
            printf("ID:%d | Numero:%d | Nome:%s | Zona:%s | Capienza:%d | Prezzo:%.2f\n",
                   tavolo->id, tavolo->numeroTavolo, tavolo->nomeTavolo,
                   tavolo->zona, tavolo->capienza, tavolo->prezzoMinimo);
        }
    }
}

void visualizzaTavoli(CatalogoTavoli *catalogo) {
    int i;

    ordinaTavoliPerNumero(catalogo);

    printf("\n--- TAVOLI ---\n");
    for (i = 0; i < catalogo->numeroElementi; i++) {
        Tavolo *tavolo = catalogo->elementi[i];
        printf("ID:%d | Numero:%d | Nome:%s | Zona:%s | Capienza:%d | Prezzo:%.2f | %s\n",
               tavolo->id,
               tavolo->numeroTavolo,
               tavolo->nomeTavolo,
               tavolo->zona,
               tavolo->capienza,
               tavolo->prezzoMinimo,
               tavolo->attivo ? "Attivo" : "Disattivo");
    }
}

void visualizzaMappaTavoli(CatalogoTavoli *catalogo) {
    int i;
    int colonne = 4;

    ordinaTavoliPerNumero(catalogo);

    printf("\n========= MAPPA TAVOLI =========\n\n");

    for (i = 0; i < catalogo->numeroElementi; i++) {
        Tavolo *tavolo = catalogo->elementi[i];
        printf("[T%-2d %-1s] ", tavolo->numeroTavolo, tavolo->attivo ? "A" : "X");

        if ((i + 1) % colonne == 0) {
            printf("\n");
        }
    }

    printf("\n\nLegenda: A = attivo, X = disattivo\n");
}

void liberaCatalogoTavoli(CatalogoTavoli *catalogo) {
    int i;

    for (i = 0; i < catalogo->numeroElementi; i++) {
        free(catalogo->elementi[i]);
    }

    free(catalogo->elementi);
}