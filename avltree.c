/**
 * @file avltree.c
 *
 * @brief  
 *
 * @author jinyang.hust@gmail.com
 *
 * @bugs
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "avltree.h"



/**
 * @brief This performs an RR rotation on the tree at the give node.  
 * This is required when an unblanced node is right-heavy and the subtree to
 * its right is right-heavy too (for example after a node was inserted into
 * the right subtree's right subtree).
 *
 * An RR rotation goes anticlockwise - it moves the right sub-node up
 * to the root, and the root node down to the left.
 * 
 * From:   N       To:     R
 *        / \             / \
 *       L   R           N   B
 *          / \         / \
 *         A   B*      L   A
 *
 * Where B is the deepest path to the bottom of the tree.
 *
 * @param root pointer that points to node, i.e. node's parent's child (points to 
 * node).
 * @param node unbalanced node. 
 */
static void 
avl_rotate_rr(avl_node_t **root, avl_node_t *node)
{
    avl_node_t *r = node->right;

    assert(r);
    assert(*root == node);

    node->right = r->left; /* handle A , N: node, R: r */
    if (r->left) r->left->parent = node;
    r->parent = node->parent; /* handle R */
    r->left = node;
    node->parent = r; /* handle N */

    *root = r;
}


/**
 * @brief This performs an LL rotation on the tree at the give node.
 * 
 * This is required when an unblanced node is left-heavy and the subtree to
 * its left is left-heavy too (for example after a node was inserted into
 * the left subtree's left subtree).
 *
 * An LL rotation goes clockwise - it moves the left sub-node up to
 * the root, and the root node down to the right.
 *
 * From:     N       To:     L
 *          / \             / \
 *         L   R           A   N
 *        / \                 / \
 *       A*  B               B   R
 *
 * Where A is the deepest path to the bottom of the tree.
 * @param root pointer that points to node, i.e. node's parent's child (points to 
 * node).
 * @param node unbalanced node. 
 */
static void 
avl_rotate_ll(avl_node_t **root, avl_node_t *node)
{
    avl_node_t *l = node->left;

    assert(l);
    assert(*root == node);

    node->left = l->right; /* handle B */
    if (l->right) l->right->parent = node;
    l->parent = node->parent; /* handle L */
    l->right = node;
    node->parent = l; /* handle N */

    *root = l;
}


/**
 * @brief This performs an LR rotation on the tree at the give node.
 * This is required when an unblanced node is left-heavy and the subtree
 * to its left is right-heavy (for example after a node was inserted into
 * the left subtree's right subtree).
 *
 * An LR rotation consists of an RR rotation on the left subtree
 * followed by an LL rotation on the root.
 *
 * From:     N       To:     N      To:      B
 *          / \             / \            /   \
 *         L   R           B   R          L     N
 *        / \             / \            / \   / \
 *       A   B           L   D          A   C D   R
 *          / \         / \
 *         C   D       A   C
 * @param root pointer that points to node, i.e. node's parent's child (points to 
 * node).
 * @param node unbalanced node. 
 */
static void 
avl_rotate_lr(avl_node_t **root, avl_node_t *node)
{
    assert(node->left);
    assert(node->left->right);
    assert(*root == node);

    avl_rotate_rr(&node->left, node->left); /* RR rotation on left subtree L-B-D */
    avl_rotate_ll(root, node); /* LL rotation on node */
}


/**
 * This performs an RL rotation on the tree at the give node.
 * This is required when an unblanced node is right-heavy and the subtree
 * to its right is left-heavy (for example after a node was inserted into
 * the right subtree's left subtree).
 *
 * An RL rotation consists of an LL rotation on the right subtree
 * followed by an RR rotation on the root.
 *
 * From:   N       To:   N        To:      A
 *        / \           / \              /   \
 *       L   R         L   A            N     R
 *          / \           / \          / \   / \
 *         A   B         C   R        L   C D   B
 *        / \               / \
 *       C   D             D   B
 * @param root pointer that points to node, i.e. node's parent's child (points to 
 * node).
 * @param node unbalanced node. 
 */

static void 
avl_rotate_rl(avl_node_t **root, avl_node_t *node)
{
    assert(node->right);
    assert(node->right->left);
    assert(*root == node);

    avl_rotate_ll(&node->right, node->right); /* LL rotation on right subtree R-A-C */
    avl_rotate_rr(root, node); /* RR rotation on node */
}



/**
 * @brief get balance factor of a tree
 * The balance factor of a tree is the height of left subtree minnus height of right 
 * subtree. If balance factor is -1, 0, or 1, then it is a balanced tree.
 * If the balance factor at a node is > 0 then the subtree at that node
 * is left heavy. If it is < 0 then it is right heavy.
 * @param  node
 * @return balance factor of a node. 
 */
int
avl_get_balance(avl_node_t *node)
{
    int lh, rh;

    lh = (node->left) ? node->left->height : 0;
    rh = (node->right) ? node->right->height : 0;
    return (lh - rh);
}


/** 
 * @brief check if a node is banlanced.
 * @param node.
 * @return
 *      TRUE    balanced
 *      FALSE   not balanced
 */
bool_t
avl_is_balanced(avl_node_t *node)
{
    int bfact = avl_get_balance(node);

    return ((bfact < -1 || bfact > 1) ? FALSE : TRUE);
}



/**
 * @brief calculate height of all nodes in a tree.
 * @param node.
 * @return no
 */
void avl_set_height(avl_node_t *node)
{
    node->height = 1;

    if (node->left) {
        avl_set_height(node->left);
        node->height = node->left->height + 1;
    }
    if (node->right) {
        avl_set_height(node->right);
        if (node->right->height >= node->height)
            node->height = node->right->height + 1;
    }
}


/**
 * @brief do balancing operation for a tree required.
 * Balance the un-balanced tree at the given node by RR, LL, RL, LR
 * rotation.
 * @param tree avl tree
 * @param node do balancing at the node.
 * @return 
 */
bool_t
avl_do_balancing(avl_tree_t *tree, avl_node_t *node)
{
    bool_t ret = TRUE;
    avl_node_t **root;

    if (node->parent)
        root = (node->parent->right == node) ? &node->parent->right 
                    : &node->parent->left;
    else {
        assert(tree->root == node);
        root = &tree->root;
    }

    if (avl_get_balance(node) > 0) {
        /* left heavy */
        assert(node->left);
        if (avl_get_balance(node->left) > 0) {
            /* left subtree also left heavy, LL case */
            avl_rotate_ll(root, node);
        }
        else if (avl_get_balance(node->left) < 0) {
            /* LR case */
            avl_rotate_lr(root, node);
        }
        else {
            /* left right equal, LL needed. Only happen for deletion */
            avl_rotate_ll(root, node);
            ret = FALSE;
        }
    }
    else if (avl_get_balance(node) < 0) {
        /* right heavy */
        assert(node->right);
        if (avl_get_balance(node->right) < 0) {
            /* RR case */
            avl_rotate_rr(root, node);
        }
        else if (avl_get_balance(node->right) > 0) {
            /* RL case */
            avl_rotate_rl(root, node);
        }
        else {
            /* left right equal, RR needed. Only happen for deletion */
            avl_rotate_rr(root, node);
            ret = FALSE;
        }
    }

    avl_set_height(tree->root);
    return ret;
}


/**
 * @brief get maxmum node in a tree.
 * @param node
 * @return avl_node_t
 */
avl_node_t *
avl_get_max_node(avl_node_t *node)
{
    if (!node) return NULL;
    if (!node->right)
        return node;
    return avl_get_max_node(node->right);
}


/**
 * @brief get mimmum node in a tree.
 * @param node
 * @return
 */
avl_node_t *
avl_get_min_node(avl_node_t *node)
{
    if (!node) return NULL;
    if (!node->left)
        return node;
    return avl_get_min_node(node->left);
}



/**
 * @brief genaral binary search tree procedure.
 * @param root tree root
 * @param node 
 * @param cmp compare function
 * @return found node, or NULL if not found.
 */
static avl_node_t *
__avl_find(avl_node_t *root, avl_node_t *node, avl_tree_cmp_fp cmp)
{
    int res = 0;
    if (!root) return NULL;

    res = cmp(node, root);
    if (res > 0) 
        return __avl_find(root->right, node, cmp);
    
    else if (res < 0) 
        return __avl_find(root->left, node, cmp);
    
    return root;
}


/**
 * @brief find a node in avl tree.
 * @param tree avl tree to be searched.
 * @param node 
 * @return the found node, or NULL if not found.
 */
static avl_node_t *
avl_find(avl_tree_t *tree, avl_node_t *node)
{
    assert(tree);
    assert(node);

    return __avl_find(tree->root, node, tree->avl_ops->avl_cmp);
}



/**
 * @brief add a new node into tree root.
 * @param root the right place in which we add the new node.
 * @param root_parent parent of root.
 * @param node new node to be added.
 * @param cmp compare function.
 * @return avl_node_t
 */
static avl_node_t *
__avl_add(avl_node_t *root, avl_node_t *root_parent, avl_node_t *node, avl_tree_cmp_fp cmp)  
{
    int res;

    if (!root) { 
        /* It is the right place in which we add the new node */
        node->parent = root_parent;
        return node;
    }

    res = cmp(node, root);
    if (res < 0)
        root->left = __avl_add(root->left, root, node, cmp);
    else if (res > 0)
        return root->right = __avl_add(root->right, root, node, cmp);

    /* node is already in the tree, i.e. root is the node */
    return root;
}


/**
 * @brief add a node to the tree.
 * @param tree
 * @node node to be added.
 * @return no
 */
void
avl_add(avl_tree_t *tree, avl_node_t *node) 
{
    assert(tree);
    assert(node);

    /* add new node */
    tree->root = __avl_add(tree->root, NULL, node, tree->avl_ops->avl_cmp);
    /* recalculate height of all nodes */
    avl_set_height(tree->root);
    /* rebalance */
    for (; node != NULL; node = node->parent) {
        if (!avl_is_balanced(node)) {
            avl_do_balancing(tree, node);
            break;
        }
    }
}



/**
 * @brief swap positions of node1 and node2.
 * Devide this into 3 cases: 
 *          1. node1 is root;
 *          2. node2 is root;
 *          3. Both node1 and node2 are not root.
 */
void
avl_swap_nodes(avl_node_t *node1, avl_node_t *node2, avl_tree_t *tree)
{
    avl_node_t tmp;
    avl_node_t **parent;
    size_t len = sizeof(avl_node_t); 

    assert(node1 && node2);
    if (node1 == node2) return;

    memcpy(&tmp, node1, len);
    memcpy(node1, node2, len);
    memcpy(node2, &tmp, len);

    /* Now we have swapped pointers (i.e. parent, left, right) of both two node.
     * Next, need to modify child pointers of parents of two nodes. 
     */
    /* handle node1 */
    if (node1->parent) {
        if (node1->parent == node1) node1->parent = node2; /* node1 and node2 link directly */
        else if (node1->parent->left == node2) node1->parent->left = node1;
        else if (node1->parent->right == node2) node1->parent->right = node1;
        else assert(FALSE);
    }  
    else {
        assert(tree->root == node2);
        tree->root = node1;
    }
    if (node1->left) {
        if (node1->left == node1) node1->left = node2;  /* node1 and node2 link directly */
        node1->left->parent = node1;
    }
    if (node1->right) {
        if (node1->right == node1) node1->right = node2;  /* node1 and node2 link directly */
        node1->right->parent = node1;
    }

    /* handle node2 */
    if (node2->parent) {
        if (node2->parent == node2) node2->parent = node1; /* node1 and node2 link directly */
        else if (node2->parent->left == node1) node2->parent->left = node2;
        else if (node2->parent->right == node1) node2->parent->right = node2;
        else assert(FALSE);
    }  
    else {
        assert(tree->root == node1);
        tree->root = node2;
    }
    if (node2->left) {
        if (node2->left == node2) node2->left = node1;  /* node1 and node2 link directly */
        node2->left->parent = node2;
    }
    if (node2->right) {
        if (node2->right == node2) node2->right = node1;  /* node1 and node2 link directly */
        node2->right->parent = node2;
    }
    
}


/**
 * @brief delete a node from a tree.
 * When deleting a node, we need to move a new node in the tree to the empty 
 * place, there are two candidates: the maximum node of its left subtree, or 
 * the minmum node of its right subtree. Here, the former is used.
 * @param tree
 * @param node to be deleted.
 * @return none
 */
void
avl_del(avl_tree_t *tree, avl_node_t *node)
{
    avl_node_t *swap;

    assert(tree);
    assert(node);
    if (node->left && node->right) {
        swap = avl_get_max_node(node->left);
        avl_swap_nodes(node, swap, tree);
    } 

    /* now the node should have at most one child */
    assert(!(node->left && node->right));

    avl_node_t **parent;
    if (node->parent) 
        parent = (node->parent->left == node) ? &node->parent->left
                        : &node->parent->right;
    else
        parent = &tree->root;
    /* delete node now */
    *parent = (node->left) ? node->left : node->right;
    if (*parent) 
        (*parent)->parent = node->parent;
    if (node->parent) {
        node = node->parent;
        avl_set_height(tree->root);
        bool_t cont = FALSE;
        for (; node != NULL; node = node->parent) {
            if (!avl_is_balanced(node)) {
                cont = avl_do_balancing(tree, node);
                if (!cont) break;
                node = node->parent;
            }
        }
    
    }
}



