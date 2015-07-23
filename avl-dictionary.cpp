
// Implementation of a dictionary using an AVL tree
// AVL Trees are balanced binary trees 

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "avl-dictionary.h"

bool debug = false;

// Constructor
AVLDictionary::AVLDictionary()
{
	root = NULL;
	nElements = 0;
}

// Add a record to the dictionary. Returns false if key already exists
bool
AVLDictionary::addRecord( KeyType key, DataType record)
{
	if ( debug) {
		printf("------------------------------------\n");
		printf("addRecord(\"%s\",%ld)\n",  key, (long) record);
		printf("---------- Before -----------------\n");
		printNode("", root, 0);
	}
	
	AVLNode *n;
	n = new AVLNode();
	
    n->key = key;
    n->data = record;
    n->height = 1;
    n->left = NULL;
    n->right = NULL;
    n->parent = NULL;
    
    if(root == NULL) {
        root = n;
		nElements++;
		
        return true;
    }
    
    AVLNode *curr;
	curr = root;
	
    AVLNode *prev;
	prev = NULL;
	
	while(curr != NULL) 	{
        prev = curr;
        if (strcmp(key, curr->key) < 0) {
            curr = curr->left;
        }
        else if (strcmp(key, curr->key) > 0) {
            curr = curr->right;
        }
        else {
            curr->data = record;
            return false;
        }
    }
	
	
	if (strcmp(key, prev->key) < 0) {
        prev->left = n;
    }
    else {
        prev->right = n;
    }
    
    AVLNode *m;
	m = n->parent;
	
    while(m != NULL) {
        int mh = 0;
        if(m->left != NULL)
            mh = m->left->height;
        if(m->right != NULL && mh < m->right->height)
            mh = m->right->height;
        m->height = 1+mh;
        m = m->parent;
    }
	// Add your implementation here

	//Find node to insert into 
	//Node does not exist. Create it.
	//Height might not be valid anymore.
	//We need to restructure .

	if ( debug) {
		printf("---------- Before Restructure -----------------\n");
		printNode("", root, 0);
	}
	
	// Call restructure

	if (debug) {
		checkRecursive(root);
		
		printf("---------- After Restructure -----------------\n");
		printNode("", root, 0);
	}
	nElements++;
	return true;
}

void
AVLDictionary::restructure(AVLNode * n) {
	//Balance the tree starting at n 

	AVLNode *z;
	z = n->parent;
	
	while(z != NULL) {
		
	}
	//Go upwards until root is found

        // See class notes

	// Add your implementation here
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
AVLDictionary::findRecord( KeyType key)
{
        // Add your implementation here

	return NULL;
}

// Removes one element from the dictionary
bool
AVLDictionary::removeElement(KeyType key)
{
	if (debug) {
		printf("------------------------------------\n");
		printf("removeElement(\"%s\")\n",  key);
		printf("---------- Before -----------------\n");
		printNode("", root, 0);
	}


	// Add your implementation here
	
	if (debug) {
		printf("---------- After -----------------\n");
		printNode("", root, 0);

		checkRecursive(root);
	}
	
	return true;
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
AVLDictionary::keys(int * n)
{
	KeyType * a = (KeyType *) malloc(nElements * sizeof(KeyType));
	*n = 0;
	addKeysRecursive(root, a, n);

	return a;
}

// Add keys recursively
void
AVLDictionary::addKeysRecursive(AVLNode * node, KeyType * a, int * n) {
	if (node==NULL) {
		return;
	}
	
	a[*n] = node->key;
	(*n)++;
	addKeysRecursive(node->left, a, n);
	addKeysRecursive(node->right, a, n);
}

