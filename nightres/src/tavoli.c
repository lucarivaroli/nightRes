#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tavoli.h"
#include "utils.h"
// Gestisce il menu per la gestione dei tavoli
void inizializzaCatalogoTavoli(CatalogoTavoli *catalogo) {
    catalogo->elementi = NULL;
    catalogo->numeroElementi = 0;
    catalogo->capacita = 0;
    catalogo->prossimoId = 1;// ID univoco per ogni tavolo, incrementato ad ogni aggiunta
}
// Espande la capacità del catalogo se necessario
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
// Trova un tavolo nel catalogo in base al suo ID
Tavolo *trovaTavoloPerId(CatalogoTavoli *catalogo, int id) {
    int i;
    for (i = 0; i < catalogo->numeroElementi; i++) { // Scorre tutti i tavoli nel catalogo
        if (catalogo->elementi[i]->id == id) { // Se trova un tavolo con l'ID corrispondente, lo restituisce
            return catalogo->elementi[i]; // Restituisce il tavolo trovato
        }
    }
    return NULL; // Se non trova nessun tavolo con l'ID specificato, restituisce NULL
}
// Aggiunge un nuovo tavolo al catalogo
void aggiungiTavolo(CatalogoTavoli *catalogo) {
    Tavolo *tavolo = (Tavolo *)malloc(sizeof(Tavolo));

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

    espandiCatalogoTavoli(catalogo); // assicura che ci sia spazio extra per il nuovo tavolo
    catalogo->elementi[catalogo->numeroElementi++] = tavolo; // aggiunge il nuovo tavolo al catalogo e incrementa il numero di elementi

    printf("Tavolo aggiunto correttamente.\n");
}
// Modifica i dettagli di un tavolo esistente
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
// Rimuove un tavolo dal catalogo (disattivandolo)
//il tavolo rimane nel catalogo ma viene marcato come disattivo, quindi con attivo=0
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
// Permette di cercare tavoli in base a zona, capienza e prezzo minimo
void cercaTavoli(CatalogoTavoli *catalogo) {
    char zona[30]; // Zona da cercare (VIP, lounge, dancefloor, esterno)
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

        // Controlla se il tavolo è attivo, se la zona corrisponde a quella cercata, se la capienza è sufficiente e se il prezzo minimo è soddisfatto
        if (tavolo->attivo == 1 && strstr(tavolo->zona, zona) != NULL && tavolo->capienza >= capienzaMinima && tavolo->prezzoMinimo >= prezzoMinimo) {
            printf("ID:%d | Numero:%d | Nome:%s | Zona:%s | Capienza:%d | Prezzo:%.2f\n",tavolo->id, tavolo->numeroTavolo, tavolo->nomeTavolo,tavolo->zona, tavolo->capienza, tavolo->prezzoMinimo);
        }
    }
}
// Visualizza la lista completa dei tavoli
void visualizzaTavoli(CatalogoTavoli *catalogo) {
    int i;

    ordinaTavoliPerNumero(catalogo);

    printf("\n--- TAVOLI ---\n");
    for (i = 0; i < catalogo->numeroElementi; i++) { // Scorre tutti i tavoli nel catalogo
        Tavolo *tavolo = catalogo->elementi[i];
        if (tavolo->attivo == 1) { // Se il tavolo è attivo, stampa le sue informazioni con l'indicazione "Attivo"
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


// Libera la memoria allocata per il catalogo dei tavoli
void liberaCatalogoTavoli(CatalogoTavoli *catalogo) {
    int i;

    for (i = 0; i < catalogo->numeroElementi; i++) {
        free(catalogo->elementi[i]);
    }

    free(catalogo->elementi);
}

void visualizzaMappaTavoli(CatalogoTavoli *catalogo) {
    int i;

    printf("\n========== MAPPA TAVOLI ==========\n");

    printf("\n--- ZONA VIP ---\n");
    for (i = 0; i < catalogo->numeroElementi; i++) {
        Tavolo *tavolo = catalogo->elementi[i]; 

        if (strcmp(tavolo->zona, "VIP") == 0) {
            if (tavolo->attivo == 1) {
                printf("[Tavolo %d - Attivo] ", tavolo->numeroTavolo);
            } else {
                printf("[Tavolo %d - Disattivo] ", tavolo->numeroTavolo);
            }
        }
    }

    printf("\n\n--- ZONA DANCEFLOOR ---\n");
    for (i = 0; i < catalogo->numeroElementi; i++) {
        Tavolo *tavolo = catalogo->elementi[i];

        if (strcmp(tavolo->zona, "dancefloor") == 0) {
            if (tavolo->attivo == 1) {
                printf("[Tavolo %d - Attivo] ", tavolo->numeroTavolo);
            } else {
                printf("[Tavolo %d - Disattivo] ", tavolo->numeroTavolo);
            }
        }
    }

    printf("\n\n--- ZONA LOUNGE ---\n");
    for (i = 0; i < catalogo->numeroElementi; i++) {
        Tavolo *tavolo = catalogo->elementi[i];

        if (strcmp(tavolo->zona, "lounge") == 0) {
            if (tavolo->attivo == 1) {
                printf("[Tavolo %d - Attivo] ", tavolo->numeroTavolo);
            } else {
                printf("[Tavolo %d - Disattivo] ", tavolo->numeroTavolo);
            }
        }
    }

    printf("\n\n--- ZONA ESTERNO ---\n");
    for (i = 0; i < catalogo->numeroElementi; i++) {
        Tavolo *tavolo = catalogo->elementi[i];

        if (strcmp(tavolo->zona, "esterno") == 0) {
            if (tavolo->attivo == 1) {
                printf("[Tavolo %d - Attivo] ", tavolo->numeroTavolo);
            } else {
                printf("[Tavolo %d - Disattivo] ", tavolo->numeroTavolo);
            }
        }
    }

    printf("\n==================================\n");
}