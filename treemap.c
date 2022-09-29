#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
  TreeMap* Map = (TreeMap*)malloc(sizeof(TreeMap));  
  Map->lower_than = lower_than; 
  return Map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  if(tree == NULL) return;
  if(tree->root == NULL){
    tree->root = createTreeNode(key, value);
    tree->current = tree->root;
    return;
  }
  TreeNode *aux = tree->root;
  while(aux){
    if(is_equal(tree, key, aux->pair->key))
      return;
    if(tree->lower_than(key, aux->pair->key)){
      if(aux->left) 
        aux = aux->left;
      else{
        aux->left = createTreeNode(key, value);
        aux->left->parent = aux;
        tree->current = aux->left;
      }
    }
    else{
      if(aux->right)
        aux = aux->right;
      else{
        aux->right = createTreeNode(key, value);
        aux->right->parent = aux;
        tree->current = aux->right;
      }
    }
  }
}
  
TreeNode * minimum(TreeNode * x){
  if(x->left == NULL){
    return x;
  }
  else{
    while (x->left != NULL){
      x = x->left;
    }
    return x;
  }
}


void removeNode(TreeMap * tree, TreeNode* node) {
  if(tree == NULL || node == NULL) return;
  if(node->left == NULL && node->right == NULL){
    if(node != tree->root){
      if(node->parent->left == node)
        node->parent->left = NULL;
      else
        node->parent->right = NULL;
      }
    free(node);
    return;
  }

  if(node->left && node->right){
    TreeNode *aux = minimum(node->right);
    node->pair->key = aux->pair->key;
    node->pair->value = aux->pair->value;
    removeNode(tree, aux);
  }
  else{
    if(node == node->parent->left){
      if(node->left){
        node->parent->left = node->left;
        node->left->parent = node->parent;
      }
      else{
        node->parent->left = node->right;
          node->right->parent = node->parent;
      }
    }
    else{
      if(node->left){
        node->parent->right = node->left;
        node->left->parent = node->parent;
      }
      else{
        node->parent->right = node->right;
        node->right->parent = node->parent;
      }
    }
    free(node);
  }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    tree->current=tree->root;
    while(tree->current!=NULL ){
      if(is_equal(tree,tree->current->pair->key,key)){
        return tree->current->pair;
      }
      else{
        if(tree->lower_than(tree->current->pair->key,key)){
          tree->current = tree->current->right;
        }
        else{
          tree->current = tree->current->left;
        }
      }
    }
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
  if(tree == NULL) return NULL;
  TreeNode *node = tree->root;
  TreeNode *upbound_node = NULL;
  while(node){
    if(is_equal(tree, node->pair->key, key))
      return node->pair;
    if(tree->lower_than(key, node->pair->key)){
      upbound_node = node;
      node = node->left;
    }
    else
      node = node->right;
  }
  if(upbound_node) return upbound_node->pair;
  return NULL;
}


Pair * firstTreeMap(TreeMap * tree) {
  tree->current = tree->root;
  while(tree->current->left != NULL){
    tree->current = tree->current->left;
  }
  return tree->current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  if(tree == NULL || tree->current == NULL)
    return NULL;
  TreeNode *aux;
  if(tree->current->right)
      aux = minimum(tree->current->right);
  else{
    aux = tree->current->parent;
    while(aux && tree->lower_than(aux->pair->key, tree->current->pair->key))
    aux = aux->parent;
  }
  tree->current = aux;
  if(aux) return aux->pair;
  return NULL;
}