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
  tree->current=node->parent;
  if(node->left==NULL && node->right==NULL){
    if(tree->current->left==node){
      tree->current->left=NULL;
      node = NULL;
    }
    else{
      tree->current->right=NULL;
      node = NULL;
    }
  }
  
  if(node->left!=NULL && node->right==NULL){
    if(tree->current->left==node)
      tree->current->left=node->left;
    else
      tree->current->right=node->left;
  
  }
  if(node->left==NULL && node->right!=NULL){
    if(tree->current->left==node)
      tree->current->left=node->right;
    else
      tree->current->right=node->right;
  }
  else{
    TreeNode * x = minimum(node->right);
    node->pair->key=x->pair->key;
    node->pair->value=x->pair->value;
    removeNode(tree,x);
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
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
  return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
