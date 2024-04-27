#ifndef TREE_DUMP_H_INCLUDED
#define TREE_DUMP_H_INCLUDED

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef int tree_elem_t;

#define tree_format "%d"

const tree_elem_t TREE_POISON_VALUE = -1;

const char* const DOT_FILE      = "tree.dot";

const char* const PICTURE_FILE  = "tree.png";

const char* const TXT_DUMP_FILE = "tree.dmp";

enum Dump_mode : unsigned int
{
    PRE_ORDER   = 0,
    POST_ORDER  = 1,
    IN_ORDER    = 2,
};

typedef struct Node
{
    int             key;
    size_t          height;
    struct Node*    left;
    struct Node*    right;
} Node;

typedef struct AVLTree
{
    Node*   root;
    size_t  size;
} AVLTree;

void TreeGraphDump(AVLTree* tree);

void NodeGraphDump(Node* node, FILE* dot_file);

void EdgeGraphDump(Node* node, FILE* dot_file);

#endif