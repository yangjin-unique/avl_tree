/**
 * @file avltree.h
 *
 * @brief  
 *
 * @author jinyang.hust@gmail.com
 *
 * @bugs
 */

#ifndef _AVL_TREE_
#define _AVL_TREE_


typedef enum {
    FALSE = 0,
    TRUE,
} bool_t;


/**
 * @brief avl node structure define
 */
typedef struct _avl_node {
    struct _avl_node    *parent;
    struct _avl_node    *left, *right;
    int     height;
} avl_node_t;


/**
 * @brief prototype of user defined compare function.
 * @return 
 *      0: equal
 *      1: a > b
 *      -1: a < b
 */
typedef int (*avl_tree_cmp_fp)(const avl_node_t *a, const avl_node_t *b);

/**
 * @brief callback function table tobe registered by users. 
 */
typedef struct _avl_ops {
    avl_tree_cmp_fp avl_cmp;
    /* add more callback if needed */
} avl_ops_t;

typedef struct _avl_tree {
    avl_node_t  *root;
    avl_ops_t   *avl_ops;   
} avl_tree_t;



/**
 * @brief find a node in avl tree.
 * @param tree avl tree to be searched.
 * @param node 
 * @return the found node, or NULL if not found.
 */
static avl_node_t *
avl_find(avl_tree_t *tree, avl_node_t *node);

/**
 * @brief add a node to the tree.
 * @param tree
 * @node node to be added.
 * @return no
 */
void
avl_add(avl_tree_t *tree, avl_node_t *node);

/**
 * @brief 
 * When deleting a node, we need to move a new node in the tree to the empty 
 * place, there are two candidates: the maximum node of its left subtree, or 
 * the minmum node of its right subtree. Here, the former is used.
 *
 */
void
avl_del(avl_tree_t *tree, avl_node_t *node);



#endif
