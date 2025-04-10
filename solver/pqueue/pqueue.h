//  pqueue.h : partie interface d'un module polymorphe pour la spécification
//    PQUEUE du TDA d'une file de priorité.

//  AUCUNE MODIFICATION DE CE SOURCE N'EST AUTORISÉE.

#ifndef PQUEUE__H
#define PQUEUE__H

#include <stdlib.h>

//  Fonctionnement général :
//  - la structure de données ne stocke pas d'objets mais des références vers
//      ces objets. Les références sont du type générique « void * » ;
//  - si des opérations d'allocation dynamique sont effectuées, elles le sont
//      pour la gestion propre de la structure de données, et en aucun cas pour
//      réaliser des copies ou des destructions d'objets ;
//  - les fonctions qui possèdent un paramètre de type « pqueue_t * » ou
//      « pqueue_t ** » ont un comportement indéterminé lorsque ce paramètre ou
//      sa déréférence n'est pas l'adresse d'un contrôleur préalablement
//      renvoyée avec succès par la fonction pqueue_empty et non révoquée depuis
//      par la fonction pqueue_dispose ;
//  - aucune fonction ne peut ajouter nullptr à la structure de données ;
//  - les fonctions de type de retour « void * » renvoient nullptr en cas
//      d'échec. En cas de succès, elles renvoient une référence de valeur
//      actuellement ou auparavant stockée par la structure de données ;
//  - l'implantation des fonctions dont la spécification ne précise pas qu'elles
//      doivent gérer les cas de dépassement de capacité ne doivent avoir
//      affaire avec aucun problème de la sorte.

// struct pqueue_t pqueue_t: type et nom de type d'un contrôleur regroupant les
//    informations nécessaires pour gérer une file de priorité.
typedef struct pqueue_t pqueue_t;

// pqueue_empty : tente d'allouer les ressources nécessaires pour gérer une
//    nouvelle file de priorité initialement vide. La fonction de comparaison
//    des objets via leurs références est pointée par compar. Renvoie nullptr en
//    cas de dépassement de capacité. Renvoie sinon un pointeur vers le
//    contrôleur associé à l'arbre.
extern pqueue_t *pqueue_empty(int (*compar)(const void *, const void *));

// pqueue_dispose : sans effet si *pptr vaut nullptr, Libère sinon les
//    ressources allouées à la gestion de la file de priorité associé à *pptr
//    puis affecte nullptr à *pptr.
extern pqueue_t *pqueue_dispose(pqueue_t **pptr);

// pqueue_insert : renvoie nullptr si ref vaut nullptr. Insère ref dans la file
// de
//    priorité pointé par p.
//    Renvoie nullptr en cas de dépassement de capacité ; renvoie sinon ref.
extern void *pqueue_enqueue(pqueue_t *p, const void *ref);

// pqueue_dequeue : renvoie nullptr si la file de priorité pointée par p est
//    vide.
//    Renvoie sinon la référence de l'objet en tête de file et le retire de
//    celle-ci.
extern void *pqueue_dequeue(pqueue_t *p);

// pqueue_filter : modifie la file de priorité pointée par p en supprimant tous
//    les objets pour lesquels la fonction de filtrage filter prenant en
//    paramettre le context cntxt, renvoie false.
extern void pqueue_filter_cntxt(pqueue_t *p,
    bool (*filter)(const void *ref, void *cntxt), void *cntxt);

#endif
