/*
 * This file is where you should implement your binary search tree.  It already
 * contains skeletons of the functions you need to implement (along with
 * documentation for each function).  Feel free to implement any additional
 * functions you might need.  Also, don't forget to include your name and
 * @oregonstate.edu email address below.
 *
 * Name: Vivek Bhagavatula
 * Email: bhagavav@oregonstate.edu
 */

#include <stdlib.h>

#include "bst.h"

/*
 * This structure represents a single node in a BST.  In addition to containing
 * pointers to its two child nodes (i.e. `left` and `right`), it contains two
 * fields representing the data stored at this node.  The `key` field is an
 * integer value that should be used as an identifier for the data in this
 * node.  Nodes in the BST should be ordered based on this `key` field.  The
 * `value` field stores data associated with the key.
 *
 * You should not modify this structure.
 */
struct bst_node {
  int key;
  void* value;
  struct bst_node* left;
  struct bst_node* right;
};


/*
 * This structure represents an entire BST.  It specifically contains a
 * reference to the root node of the tree.
 *
 * You should not modify this structure.
 */
struct bst {
  struct bst_node* root;
};

/*
 * This function calculates the size of a tree by going to the bottom of the tree and building it up by adding 1's which represent the current node to whatever the total currently is and keeps updating
 */

int FIND_SIZE(struct bst_node* node);


/*
 * This function finds the height of the tree by identifiying the max length subtree of each subtree and adding one because the height is increasing by one until you get to the top
 */


int FIND_HEIGHT(struct bst_node* node);

/*
 * This function finds the path sum of the tree by only returning a 1 or true if there has been a leaf node that has hit the path sum. 
 */


int FIND_PATH_SUM(struct bst_node* node, int sum);


/*
 * This function finds the range sum by incrementing a sum if it falls in between the interval and then keeps searching in the intervals by going left and right until you cant anymore
 */

int FIND_RANGE_SUM(struct bst_node* node, int lower, int upper);

/*
 * This function frees the nodes in the tree using postorder traversal
 */

void FREE_NODES(struct bst_node* node);

/*
 * This function should allocate and initialize a new, empty, BST and return
 * a pointer to it.
 */
struct bst* bst_create() {
  struct bst* tree = malloc(sizeof(struct bst));
  tree->root = NULL;
  return tree;
}

/*
 * This function should free the memory associated with a BST.  While this
 * function should up all memory used in the BST itself, it should not free
 * any memory allocated to the pointer values stored in the BST.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   bst - the BST to be destroyed.  May not be NULL.
 */
void bst_free(struct bst* bst) {
  FREE_NODES(bst->root);	
  free(bst);
  return;
}

/*
 * This function should return the total number of elements stored in a given
 * BST.
 *
 * Params:
 *   bst - the BST whose elements are to be counted.  May not be NULL.
 */
int bst_size(struct bst* bst) {
  return FIND_SIZE(bst->root);
}

/*
 * This function should insert a new key/value pair into the BST.  The key
 * should be used to order the key/value pair with respect to the other data
 * stored in the BST.  The value should be stored along with the key, once the
 * right location in the tree is found.
 *
 * Params:
 *   bst - the BST into which a new key/value pair is to be inserted.  May not
 *     be NULL.
 *   key - an integer value that should be used to order the key/value pair
 *     being inserted with respect to the other data in the BST.
 *   value - the value being inserted into the BST.  This should be stored in
 *     the BST alongside the key.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */

  void bst_insert(struct bst* bst, int key, void* value) {
    struct bst_node* current = bst->root;
    struct bst_node* parent = NULL;

    while (current != NULL) {
        parent = current;
        if (key < current->key) {
            current = current->left;
        } else if (key > current->key) {
            current = current->right;
        } else {
            // Key already exists, update the value and return so you dont allocate memory for no reason
            current->value = value;
            return;
        }
    }


    //if the key does not exist then allocate
    struct bst_node* new_node = malloc(sizeof(struct bst_node));
    new_node->key = key;
    new_node->value = value;
    new_node->left = NULL;
    new_node->right = NULL;

    if (parent == NULL) {
        bst->root = new_node;
    } else if (key < parent->key) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }
}

/*
 * This function should remove a key/value pair with a specified key from a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should remove the first one it encounters (i.e. the one closest to
 * the root of the tree).
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair to be removed from the BST.
 */
void bst_remove(struct bst* bst, int key) {
    struct bst_node* current = bst->root;
    struct bst_node* parent = NULL;

    // search through the tree itereativley to see if you can find the key
    while (current != NULL && current->key != key) {
        parent = current;
        if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    // key is not found
    if (current == NULL) {
        return; 
    }

    // node is a leaf node
    if (current->left == NULL && current->right == NULL) {
        if (parent == NULL) { // Node to be deleted is the root node
            bst->root = NULL;
	    free(current);
        } else if (parent->left == current) {
            parent->left = NULL;
	    free(current);
        } else {
            parent->right = NULL;
	    free(current);
        }
        
    }

    // Node has 1 child
    else if (current->left == NULL || current->right == NULL) {
        struct bst_node* child = (current->left != NULL) ? current->left : current->right;
        
        if (parent == NULL) { // Node to be deleted is the root node
            bst->root = child;
        } else if (parent->left == current) {
            parent->left = child;
        } else {
            parent->right = child;
        }
        free(current);
    }

    // node has 2 children
    else {
        struct bst_node* successor_parent = current;
        struct bst_node* successor = current->right;
        while (successor->left != NULL) {
            successor_parent = successor;
            successor = successor->left;
        }
        current->key = successor->key;
        current->value = successor->value;
        if (successor_parent->left == successor) {
            successor_parent->left = successor->right;
        } else {
            successor_parent->right = successor->right;
        }
       free(successor);
    }
}


/*
 * This function should return the value associated with a specified key in a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should return the first one it encounters (i.e. the one closest to
 * the root of the tree).  If the BST does not contain the specified key, this
 * function should return NULL.
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair whose value is to be returned.
 *
 * Return:
 *   Should return the value associated with the key `key` in `bst` or NULL,
 *   if the key `key` was not found in `bst`.
 */
void* bst_get(struct bst* bst, int key) {
  struct bst_node* current = bst->root;
    while (current != NULL) {
        if (key < current->key) {
            current = current->left;  
        } else if (key > current->key) {
            current = current->right; 
        } else {
            return current->value;  
        }
    }

    return NULL;
}

/*****************************************************************************
 **
 ** BST puzzle functions
 **
 *****************************************************************************/

/*:w
 * This function should return the height of a given BST, which is the maximum
 * depth of any node in the tree (i.e. the number of edges in the path from
 * the root to that node).  Note that the height of an empty tree is -1 by
 * convention.
 *
 * Params:
 *   bst - the BST whose height is to be computed
 *
 * Return:
 *   Should return the height of bst.
 */
 int bst_height(struct bst* bst) {
   return FIND_HEIGHT(bst->root);
 }

/*
 * This function should determine whether a specified value is a valid path
 * sum within a given BST.  In other words, this function should check whether
 * the given BST contains any path from the root to a leaf in which the keys
 * sum to the specified value.
 *
 * Params:
 *   bst - the BST whose paths sums to search
 *   sum - the value to search for among the path sums of `bst`
 *
 * Return:
 *   Should return 1 if `bst` contains any path from the root to a leaf in
 *   which the keys add up to `sum`.  Should return 0 otherwise.
 */
int bst_path_sum(struct bst* bst, int sum) {
  return FIND_PATH_SUM(bst->root, sum);
}

/*
 * This function should compute a range sum in a given BST.  Specifically, it
 * should compute the sum of all keys in the BST between a given lower bound
 * and a given upper bound.  For full credit, you should not process any subtree
 * whose keys cannot be included in the range sum.
 *
 * Params:
 *   bst - the BST within which to compute a range sum
 *   lower - the lower bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *   upper - the upper bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *
 * Return:
 *   Should return the sum of all keys in `bst` between `lower` and `upper`.
 */
int bst_range_sum(struct bst* bst, int lower, int upper) {
  return FIND_RANGE_SUM(bst->root, lower, upper);
}

int FIND_SIZE(struct bst_node* node) {
    if (node == NULL) {
        return 0; 
    }

    int left_size = FIND_SIZE(node->left);
    int right_size = FIND_SIZE(node->right);

    return 1 + left_size + right_size;
}

int FIND_HEIGHT(struct bst_node* node) {
    if (node == NULL) {
        return -1; // Base case: null node, height is -1
    }

    // Recursively find the height of the left and right subtrees
    int left_height = FIND_HEIGHT(node->left);
    int right_height = FIND_HEIGHT(node->right);

    // Return the maximum of the two heights plus 1 
    int MAX = 0;
    if(left_height>right_height) MAX = left_height;
    else MAX = right_height;
    return 1 + MAX;
}

int FIND_PATH_SUM(struct bst_node* node, int sum) {
    if (node == NULL) {
        return 0; // No node, no path
    }

    // If it's a leaf node, check if the current sum equals the node's key
    if (node->left == NULL && node->right == NULL) {
        return (node->key == sum);
    }

    // Recursively check both the left and right subtrees
    int left_path_sum = FIND_PATH_SUM(node->left, sum - node->key);
    int right_path_sum = FIND_PATH_SUM(node->right, sum - node->key);

    //check if any of them went to 1 in which case you will return a 1 at the end of the first function
    if (left_path_sum != 0) {
        return 1;
    }
    if (right_path_sum != 0) {
    	return 1;
    }

    return 0;
}

int FIND_RANGE_SUM(struct bst_node* node, int lower, int upper) {
    if (node == NULL) {
        return 0; 
    }

    int sum = 0;
    //set the sum counter outside of function calls
    if (node->key >= lower && node->key <= upper) {
        sum += node->key;
    }

    if (node->key > lower) {//if the key is in range you want to keep trying to access more nodes that are in range 
        sum += FIND_RANGE_SUM(node->left, lower, upper); 
    }
    if (node->key < upper) {// same here
        sum += FIND_RANGE_SUM(node->right, lower, upper); 
    }
    //once they all finish the total sum will be set
    return sum;
}
void FREE_NODES(struct bst_node* node) {//frees the nodes in the tree using postorder traversal
    if (node == NULL) {
      return;
    }
    FREE_NODES(node->left);
    FREE_NODES(node->right);
    free(node);
}

