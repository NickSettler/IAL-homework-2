/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
    int result = 1;
    int length = strlen(key);
    for (int i = 0; i < length; i++) {
        result += key[i];
    }
    return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
    for (int i = 0; i < HT_SIZE; i++) {
        (*table)[i] = NULL;
    }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
    int index = get_hash(key);
    ht_item_t *item = (*table)[index];
    while (item != NULL) {
        if (strcmp(item->key, key) == 0) {
            return item;
        }
        item = item->next;
    }
    return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    ht_item_t *item = ht_search(table, key);
    if (item != NULL) {
        item->value = value;
        return;
    }

    int index = get_hash(key);
    ht_item_t *new_item = malloc(sizeof(ht_item_t));
    new_item->key = key;
    new_item->value = value;
    new_item->next = (*table)[index];
    (*table)[index] = new_item;
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
    ht_item_t *item = ht_search(table, key);

    return item == NULL ? NULL : &item->value;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
    int index = get_hash(key);
    ht_item_t *item = (*table)[index];
    ht_item_t *prev = NULL;
    while (item != NULL) {
        if (strcmp(item->key, key) == 0) {
            if (prev == NULL) {
                (*table)[index] = item->next;
            } else {
                prev->next = item->next;
            }
            free(item);
            return;
        }
        prev = item;
        item = item->next;
    }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
    for (int i = 0; i < HT_SIZE; i++) {
        ht_item_t *item = (*table)[i];
        while (item != NULL) {
            ht_item_t *next = item->next;
            free(item);
            item = next;
        }
        (*table)[i] = NULL;
    }
}
