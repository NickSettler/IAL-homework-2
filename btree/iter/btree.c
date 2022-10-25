/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
//    if(tree == NULL) return;

    *tree = malloc(sizeof(bst_node_t));
    (*tree)->left = NULL;
    (*tree)->right = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    while (tree != NULL) {
        if (tree->key == key) {
            *value = tree->value;
            return true;
        }

        if (tree->key > key) {
            tree = tree->left;
        } else {
            tree = tree->right;
        }
    }

    return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    bst_node_t *new_node = malloc(sizeof(bst_node_t));
    new_node->key = key;
    new_node->value = value;
    new_node->left = NULL;
    new_node->right = NULL;

    if (*tree == NULL) {
        *tree = new_node;
        return;
    }

    bst_node_t *current = *tree;
    bst_node_t *parent = NULL;

    while (current != NULL) {
        parent = current;

        if (current->key == key) {
            current->value = value;
            free(new_node);
            return;
        }

        if (current->key > key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (parent->key > key) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    bst_node_t *current = *tree;
    bst_node_t *parent = NULL;

    while (current->right != NULL) {
        parent = current;
        current = current->right;
    }

    target->key = current->key;
    target->value = current->value;

    if (parent == NULL) {
        *tree = current->left;
    } else {
        parent->right = current->left;
    }

    free(current);
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    bst_node_t *current = *tree;
    bst_node_t *parent = NULL;

    while (current != NULL) {
        if (current->key == key) {
            if (current->left == NULL && current->right == NULL) {
                if (parent == NULL) {
                    *tree = NULL;
                } else {
                    if (parent->left == current) {
                        parent->left = NULL;
                    } else {
                        parent->right = NULL;
                    }
                }

                free(current);
            } else if (current->left == NULL) {
                if (parent == NULL) {
                    *tree = current->right;
                } else {
                    if (parent->left == current) {
                        parent->left = current->right;
                    } else {
                        parent->right = current->right;
                    }
                }

                free(current);
            } else if (current->right == NULL) {
                if (parent == NULL) {
                    *tree = current->left;
                } else {
                    if (parent->left == current) {
                        parent->left = current->left;
                    } else {
                        parent->right = current->left;
                    }
                }

                free(current);
            } else {
                bst_replace_by_rightmost(current, &current->left);
            }

            return;
        }

        parent = current;

        if (current->key > key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    bst_node_t *current = *tree;
    bst_node_t *parent = NULL;

    while (current != NULL) {
        if (current->left == NULL && current->right == NULL) {
            if (parent == NULL) {
                *tree = NULL;
            } else {
                if (parent->left == current) {
                    parent->left = NULL;
                } else {
                    parent->right = NULL;
                }
            }

            free(current);
            current = parent;
        } else if (current->left == NULL) {
            if (parent == NULL) {
                *tree = current->right;
            } else {
                if (parent->left == current) {
                    parent->left = current->right;
                } else {
                    parent->right = current->right;
                }
            }

            free(current);
            current = parent;
        } else if (current->right == NULL) {
            if (parent == NULL) {
                *tree = current->left;
            } else {
                if (parent->left == current) {
                    parent->left = current->left;
                } else {
                    parent->right = current->left;
                }
            }

            free(current);
            current = parent;
        } else {
            bst_replace_by_rightmost(current, &current->left);
        }

        if (current != NULL) {
            parent = current;
            current = current->right;
        }
    }
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
    bst_node_t *current = tree;

    while (current != NULL) {
        bst_print_node(current);
        stack_bst_push(to_visit, current);
        current = current->left;
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    stack_bst_t to_visit;
    stack_bst_init(&to_visit);

    bst_leftmost_preorder(tree, &to_visit);

    while (!stack_bst_empty(&to_visit)) {
        bst_node_t *current = stack_bst_top(&to_visit);
        stack_bst_pop(&to_visit);

        bst_leftmost_preorder(current->right, &to_visit);
    }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    bst_node_t *current = tree;

    while (current != NULL) {
        stack_bst_push(to_visit, current);
        current = current->left;
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
    stack_bst_t to_visit;
    stack_bst_init(&to_visit);

    bst_leftmost_inorder(tree, &to_visit);

    while (!stack_bst_empty(&to_visit)) {
        bst_node_t *current = stack_bst_top(&to_visit);
        stack_bst_pop(&to_visit);

        bst_print_node(current);
        bst_leftmost_inorder(current->right, &to_visit);
    }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
    bst_node_t *current = tree;
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
    stack_bst_t to_visit;
    stack_bst_init(&to_visit);

    stack_bool_t first_visit;
    stack_bool_init(&first_visit);

    bst_leftmost_postorder(tree, &to_visit, &first_visit);

    while (!stack_bst_empty(&to_visit)) {
        bst_node_t *current = stack_bst_top(&to_visit);
        stack_bst_pop(&to_visit);

        bool first = stack_bool_top(&first_visit);
        stack_bool_pop(&first_visit);

        if (first) {
            stack_bst_push(&to_visit, current);
            stack_bool_push(&first_visit, false);
            bst_leftmost_postorder(current->right, &to_visit, &first_visit);
        } else {
            bst_print_node(current);
        }
    }
}
