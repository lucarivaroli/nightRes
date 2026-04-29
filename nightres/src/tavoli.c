#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tavoli.h"
#include "utils.h"

void inizializzaCatalogoTavoli(CatalogoTavoli *catalogo) {
    catalogo->elementi = NULL;
    catalogo->numeroElementi = 0;
    catalogo->capacita = 0;
    catalogo->prossimoId = 1;// ID univoco per ogni tavolo, incrementato ad ogni aggiunta
}

void espandiCatalogoTavoli(CatalogoTavoli *catalogo) {// Espande la capacità del catalogo se necessario
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

    printf("Nome tavolo: ");
    scanf(" %[^\n]", tavolo->nomeTavolo);

    printf("Zona(VIP, dancefloor, lounge, esterno): ");
    scanf(" %[^\n]", tavolo->zona);

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
    

    printf("Nuovo nome tavolo: ");
    scanf(" %[^\n]", tavolo->nomeTavolo);
    

    printf("Nuova zona: ");
    scanf(" %[^\n]", tavolo->zona);
    

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

    printf("Zona da cercare: ");
    scanf(" %[^\n]", zona);
    pulisciInput();

    printf("Capienza minima: ");
    scanf("%d", &capienzaMinima);

    printf("Prezzo minimo richiesto: ");
    scanf("%f", &prezzoMinimo);
    pulisciInput();

    printf("\n--- RISULTATI RICERCA TAVOLI ---\n");
    for (i = 0; i < catalogo->numeroElementi; i++) {
        Tavolo *tavolo = catalogo->elementi[i];

        if (tavolo->attivo == 1 && strstr(tavolo->zona, zona) != NULL && tavolo->capienza >= capienzaMinima && tavolo->prezzoMinimo >= prezzoMinimo) {
            printf("ID:%d | Numero:%d | Nome:%s | Zona:%s | Capienza:%d | Prezzo:%.2f\n",tavolo->id, tavolo->numeroTavolo, tavolo->nomeTavolo,tavolo->zona, tavolo->capienza, tavolo->prezzoMinimo);
        }
    }
}

void visualizzaTavoli(CatalogoTavoli *catalogo) {
    int i;

    ordinaTavoliPerNumero(catalogo);

    printf("\n--- TAVOLI ---\n");
    for (i = 0; i < catalogo->numeroElementi; i++) {
        Tavolo *tavolo = catalogo->elementi[i];
        if (tavolo->attivo == 1) {
        printf("ID:%d | Numero:%d | Nome:%s | Zona:%s | Capienza:%d | Prezzo:%.2f | Attivo\n",
            tavolo->id,
            tavolo->numeroTavolo,
            tavolo->nomeTavolo,
            tavolo->zona,
            tavolo->capienza,
            tavolo->prezzoMinimo);
    } else {
        printf("ID:%d | Numero:%d | Nome:%s | Zona:%s | Capienza:%d | Prezzo:%.2f | Disattivo\n",
            tavolo->id,
            tavolo->numeroTavolo,
            tavolo->nomeTavolo,
            tavolo->zona,
            tavolo->capienza,
            tavolo->prezzoMinimo);
        }
    }
        
    
}


void liberaCatalogoTavoli(CatalogoTavoli *catalogo) {
    int i;

    for (i = 0; i < catalogo->numeroElementi; i++) {
        free(catalogo->elementi[i]);
    }

    free(catalogo->elementi);
}