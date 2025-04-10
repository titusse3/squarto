//  ktree.h : partie interface d'un module polymorphe pour la spécification
//    KTREE du TDA d'un arbre à k enfant.

//  AUCUNE MODIFICATION DE CE SOURCE N'EST AUTORISÉE.

#ifndef KTREE__H
#define KTREE__H

#include <stdlib.h>

// Fonctionnement général :
// - la structure de données ne stocke pas d'objets mais des références vers
//      ces objets. Les références sont du type générique « void * » ;
// - si des opérations d'allocation dynamique sont effectuées, elles le sont
//      pour la gestion propre de la structure de données, et en aucun cas pour
//      réaliser des copies ou des destructions d'objets ;
// - les fonctions qui possèdent un paramètre de type « ktree_t * » ou
//      « ktree_t ** » ont un comportement indéterminé lorsque ce paramètre ou
//      sa déréférence n'est pas l'adresse d'un contrôleur préalablement
//      renvoyée avec succès par la fonction ktree_empty et non révoquée depuis
//      par la fonction ktree_dispose ;
// - aucune fonction ne peut ajouter nullptr à la structure de données ;
// - les fonctions de type de retour « void * » renvoient nullptr en cas
//      d'échec. En cas de succès, elles renvoient une référence de valeur
//      actuellement ou auparavant stockée par la structure de données ;
// - l'implantation des fonctions dont la spécification ne précise pas qu'elles
//      doivent gérer les cas de dépassement de capacité ne doivent avoir
//      affaire avec aucun problème de la sorte.
// - la structure de données associé à chaque noeud de l'arbre un numéro unique
//      compris entre 0 et la taille de l'arbre - 1.

// struct ktree_t ktree_t: type et nom de type d'un contrôleur regroupant les
//    informations nécessaires pour gérer un arbre k-aire.
typedef struct ktree_t ktree_t;

// ktree_empty : tente d'allouer les ressources nécessaires pour gérer un
//    nouvel arbre k-aire initialement vide.
//    Renvoie nullptr en cas de dépassement de capacité, sinon un pointeur vers
//    le contrôleur associé à l'arbre.
extern ktree_t *ktree_empty(void);

// ktree_dispose : sans effet si *kptr vaut nullptr, Libère sinon les ressources
//  allouées à la gestion de l'arbre k-aire associé à *kptr puis affecte nullptr
//  à *kptr.
extern void ktree_dispose(ktree_t **kptr);

// ktree_insert : renvoie nullptr si ref vaut nullptr. Insère ref dans l'arbre
//    k-aire pointé par k en tant qu'enfant du noeud de numéro n. Si n vaut
//    SIZE_MAX, insère ref en tant que racine de l'arbre k-aire pointé par k.
//    Renvoie SIZE_MAX en cas de dépassement de capacité et/ou que n soit pas
//    valide ; renvoie sinon le numéro associé au noeud ajouté.
extern size_t ktree_insert(ktree_t *k, size_t n, const void *ref);

// ktree_get_neighbor : renvoie SIZE_MAX si n n'est pas un numéro valide.
//    Renvoie sinon le numéro associé à l'enfant du noeud n de l'arbre k-aire
//    pointé par k, SIZE_MAX si ce noeud n'admet pas d'enfant.
extern size_t ktree_get_neighbor(ktree_t *k, size_t n);

// ktree_get_ref_by_num : renvoie SIZE_MAX si n n'est pas un numéro valide
//    Renvoie sinon la référence de l'enfant n de l'arbre k-aire pointé par k.
extern void *ktree_get_ref_by_num(ktree_t *k, size_t n);

// ktree_get_num_by_ref : renvoie SIZE_MAX si n n'est pas un numéro valide
//    Renvoie sinon le numéro associé au première enfant du noeud de l'arbre de
//    numéro n. Si n n'a pas d'enfant, renvoie SIZE_MAX.
extern size_t ktree_get_first_child(ktree_t *k, size_t n);

// ktree_get_parent_by_ref : renvoie SIZE_MAX si n n'est pas un numéro valide
//    Renvoie sinon le numéro associé au parent du noeud de l'arbre de numéro n.
//    Si n désigne la racine alors renvoie SIZE_MAX.
extern size_t ktree_get_parent_by_num(ktree_t *k, size_t n);

// ktree_get_num_by_ref : renvoie SIZE_MAX si l'arbre est vide.
//    Renvoie sinon le numéro associé à la racine de l'arbre.
extern size_t ktree_get_root_num(ktree_t *k);

// ktree_is_descendant : renvoie false si n1 ou n2 ne sont pas des numéros
//    valide.
//    Renvoie true si n2 est un descendant de n1, false sinon.
extern bool ktree_is_descendant(ktree_t *k, size_t n1, size_t n2);

#endif
