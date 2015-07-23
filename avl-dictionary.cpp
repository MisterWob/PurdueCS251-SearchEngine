
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
	restructure(n);

	if (debug) {
		checkRecursive(root);
		
		printf("---------- After Restructure -----------------\n");
		printNode("", root, 0);
	}
	nElements++;
	return true;
}

void AVLDictionary::restructure(AVLNode * n) {
	//Balance the tree starting at n 

	AVLNode *z;
	z = n->parent;
	
	while(z != NULL) {
		int height_l = 0;
        if(z->left != NULL) {
            height_l = z->left->height;
      	}
        
        int height_r = 0;
        if(z->right != NULL) {
            height_r = z->right->height;
        }
        
        int heightdiff = height_l - height_r;
   
   		 if(heightdiff < 0) heightdiff = -heightdiff;
    
   		 if(height_l > height_r) z->height = 1+height_l;
   		 else z->height = 1+height_r;
    
   		 if(heightdiff <= 1) {
   	         z = z->parent;
   	         continue;
   		 }
   		 
   		 AVLNode *y = NULL;
        int mh = 0;
        if(z->left != NULL) {
            y = z->left;
            mh = y->height;
        }
        
        if(z->right != NULL && z->right->height > mh) {
            y = y->right;
            mh = z->height;
        }
        
        AVLNode *x = NULL;
        mh = 0;
        if(y->left != NULL) {
            x = y->left;
            mh = x->height;
        }

        if(y->right != NULL && y->right->height > mh) {
            x = y->right;
            mh = x->height;
        }
        
        assert(x != NULL);
        
        AVLNode * a, * b, * c, * t0, * t1, * t2, * t3;
        if(z->right == y) {
            if(y->right == x) {
                a = z;
                b = y;
                c = x;
                t0 = z->left;
                t1 = y->left;
                t2 = x->left;
                t3 = x->right;
            }
            
            else {
                a = z;
                b = x;
                c = y;
                t0 = z->left;
                t1 = x->left;
                t2 = y->left;
                t3 = y->right;
            }
            
        }
        
        else {
            if(y->left == x) {
                a = x;
                b = y;
                c = z;
                t0 = x->left;
                t1 = x->right;
                t2 = y->right;
                t3 = z->right;
            }
            else {
                a = y;
                b = x;
                c = z;
                t0 = y->left;
                t1 = x->left;
                t2 = x->right;
                t3 = z->right;
            }
        }
        
        AVLNode *p;
		p = z->parent;
        if(p == NULL) {
            root = b;
        }
        else {
            if(b->left == z)
                p->left = b;
            else
                p->right = b;
        }  
        
        b->parent = p;
        b->left = a;
        b->right = c;
        a->parent = b;
        a->left = t0;
        a->right = t1;
        c->parent = b;
        c->left = t2;
        c->right = t3;

        if(t0 != NULL) t0->parent = a;
        if(t1 != NULL) t1->parent = a;
        if(t2 != NULL) t2->parent = c;
        if(t3 != NULL) t3->parent = c;
            
            mh = 0;
        if(a->left != NULL) mh = a->left->height;
        if(a->right != NULL && a->right->height > mh) {
            mh = a->right->height;
        }
        a->height = 1+mh;
        
        mh = 0;
        if(c->left != NULL) mh = c->left->height;
        if(c->right != NULL && c->right->height > mh) mh = c->right->height;
        c->height = 1+mh;

        mh = 0;
        
        if(b->left != NULL) mh = b->left->height;
        if(b->right != NULL && b->right->height > mh) mh = b->right->height;
        b->height = 1+mh;
        
        z = p;
            
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

	AVLNode *n;
	n = root;
	
	while (n != NULL) {
		if (strcmp(n->key, key) == 0) {
			return (DataType)n->data;
		}
		else if (strcmp(n->key, key) > 0) {
			n = n->left;
		}
		else {
			n = n->right;
		}
	}

	
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

