#include "tree_dump.h"

void TreeGraphDump(AVLTree* tree)
{
    assert((tree != nullptr) && "Pointer to tree is NULL!!!\n");

    FILE* dot_file = nullptr;

    dot_file = fopen(DOT_FILE, "w");

    //========================================================================================

    fprintf(dot_file, "digraph G\n"
                      "{\n"
                      "\tgraph [dpi = 100];\n\n"
                      "\trankdir = TB;\n\n"
                      "\tedge[minlen = 3, arrowsize = 2, penwidth = 1.5];\n"
                      "\tnode[shape = Mrecord, style = \"rounded, filled\", "
                      "fillcolor = \"yellow\", fontsize = 20, "
                      "penwidth = 3];\n\n");

    //========================================================================================

    NodeGraphDump(tree->root, dot_file);
    fprintf(dot_file, "\n");

    EdgeGraphDump(tree->root, dot_file);
    fprintf(dot_file, "\n");

    //========================================================================================

    fprintf(dot_file, "}\n");

    fclose(dot_file);

    system("dot tree.dot -T png -o tree.png"); //Change!!!

}

void NodeGraphDump(Node* node, FILE* dot_file)
{
    if (node == nullptr)
    {
        return;
    }

    fprintf(dot_file, "\tnode_%p[label = \"%d | %d\"];\n", node, node->key, node->height);

    NodeGraphDump(node->left,  dot_file);

    NodeGraphDump(node->right, dot_file);
}

void EdgeGraphDump(Node* node, FILE* dot_file)
{
    if (node->left != nullptr)
    {
        fprintf(dot_file, "node_%p -> node_%p [color = \"#00FF41\", label = \"L\"];\n",
                           node, node->left);

        EdgeGraphDump(node->left, dot_file);
    }

    if (node->right != nullptr)
    {
        fprintf(dot_file, "node_%p -> node_%p [color = \"#FF4001\", label = \"R\"];\n",
                           node, node->right);

        EdgeGraphDump(node->right, dot_file);
    }
}