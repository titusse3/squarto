#include "ktree.h"

#include <stdint.h>

typedef struct node_t node_t;

struct node_t {
  size_t num;
  const void *ref;
  node_t *neightbor;
  node_t *child;
  node_t *parent;
};

struct ktree_t {
  size_t size;
  node_t *root;
};

// === NODE ====================================================================

// node__dispose : libère récursivement la mémoire allouée à l'arbre
//    k-aire pointé par node.
void node__dispose(node_t *node) {
  while (node != nullptr) {
    node__dispose(node->child);
    node_t *next = node->neightbor;
    free(node);
    node = next;
  }
}

// node__get_by_num : renvoie le noeud de l'arbre k-aire pointé par k
//    dont le numéro est n. Renvoie nullptr si n n'est pas valide comme numéro.
node_t *node__get_by_num(node_t *k, size_t n) {
  if (k == nullptr) {
    return nullptr;
  }
  if (k->num == n) {
    return k;
  }
  node_t *result = node__get_by_num(k->child, n);
  if (result != nullptr) {
    return result;
  }
  return node__get_by_num(k->neightbor, n);
}

// === KTREE ===================================================================

ktree_t *ktree_empty(void) {
  ktree_t *k = malloc(sizeof *k);
  if (k == nullptr) {
    return nullptr;
  }
  k->size = 0;
  k->root = nullptr;
  return k;
}

void ktree_dispose(ktree_t **kptr) {
  if (*kptr == nullptr) {
    return;
  }
  node__dispose((*kptr)->root);
  free(*kptr);
  *kptr = nullptr;
}

size_t ktree_insert(ktree_t *k, size_t n, const void *ref) {
  node_t *node = malloc(sizeof *node);
  if (node == nullptr) {
    return SIZE_MAX;
  }
  node->num = k->size;
  ++k->size;
  node->ref = ref;
  node->neightbor = nullptr;
  node->child = nullptr;
  if (n == SIZE_MAX) {
    if (k->root != nullptr) {
      free(node);
      return SIZE_MAX;
    }
    k->root = node;
    node->parent = nullptr;
    return node->num;
  }
  node_t *parent = node__get_by_num(k->root, n);
  if (parent == nullptr) {
    free(node);
    return SIZE_MAX;
  }
  node->parent = parent;
  node->neightbor = parent->child;
  parent->child = node;
  return node->num;
}

size_t ktree_get_neighbor(ktree_t *k, size_t n) {
  node_t *node = node__get_by_num(k->root, n);
  if (node == nullptr) {
    return SIZE_MAX;
  }
  return node->neightbor == nullptr ? SIZE_MAX : node->neightbor->num;
}

void *ktree_get_ref_by_num(ktree_t *k, size_t n) {
  node_t *node = node__get_by_num(k->root, n);
  if (node == nullptr) {
    return nullptr;
  }
  return (void *) node->ref;
}

size_t ktree_get_first_child(ktree_t *k, size_t n) {
  node_t *node = node__get_by_num(k->root, n);
  if (node == nullptr) {
    return SIZE_MAX;
  }
  return node->child == nullptr ? SIZE_MAX : node->child->num;
}

size_t ktree_get_parent_by_num(ktree_t *k, size_t n) {
  node_t *node = node__get_by_num(k->root, n);
  if (node == nullptr) {
    return SIZE_MAX;
  }
  return node->parent == nullptr ? SIZE_MAX : node->parent->num;
}

size_t ktree_get_root_num(ktree_t *k) {
  if (k->root == nullptr) {
    return SIZE_MAX;
  }
  return k->root->num;
}

bool ktree_is_descendant(ktree_t *k, size_t n1, size_t n2) {
  node_t *node = node__get_by_num(k->root, n1);
  if (node == nullptr) {
    return false;
  }
  return node__get_by_num(node, n2) != nullptr;
}
