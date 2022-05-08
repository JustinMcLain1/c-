/* 	Justin McLain
	Z1910087
	CSCI 340- PE1
	
	I certify that this is my own work and where appropriate an extension 
	of the starter code provided for the assignment.
*/
#include <iostream>
#include <vector>
#include "node.h"
#include "bstDriver.h"


// include other header files (if any)
#ifndef BINARYSEARCHTREE_H_
#define BINARYSEARCHTREE_H_

template <typename T>
class BinarySearchTree : public BinaryTree<T>
{
public:
    void Insert(const T &x);       // inserts node with value x
    bool Search(const T &x) const; // searches leaf with value x
    bool Remove(const T &x);       // removes leaf with value x
private:
    void _Insert(Node<T> *&, const T &);      // private version of insert
    bool _Search(Node<T> *, const T &) const; // private version of search
    void _Remove(Node<T> *&, const T &);      // private version of remove
    bool _Leaf(Node<T> *node) const;          // checks if node is leaf
    Node<T>* pred ( Node<T>* ); // for right child
};

template <class T>
void BinarySearchTree<T>::Insert( const T& x ) {
   _Insert(this->root, x);
}

template <class T>
bool BinarySearchTree<T>::Search(const T& x) const {
    return _Search(this->root,x);
}


template <class T>
bool BinarySearchTree<T>::Remove( const T& x ) {
      _Remove(this->root, x);
      return (x);
}



template <class T>
void BinarySearchTree<T>::_Insert( Node<T>*& ptr, const T& x ) {
    if(ptr == NULL)
        ptr = new Node<T>(x);
    else {
        if(x < ptr->data)
            _Insert(ptr->left, x);
        else
            _Insert(ptr->right, x);
    }
}

template <class T>
bool _Search(Node<T> *root, const T & x) 
    {
        if (root == NULL) return false;
            if (root->x == x) {
                return (x);
        }
        if ( x < root->x)
            return _Search(x, root->Left);
        else
            return _Search(x, root->Right);
        
    }

template <class T>
void BinarySearchTree<T>::_Remove( Node<T>*& ptr, const T& x ) {
    Node<T>* temp = ptr;

    if(ptr == NULL)
        return ; //node doesnt exist
    else if(x < ptr->data)
        Remove(ptr->left, x);
    else if(x > ptr->data)
        Remove(ptr->right, x);
    else {
        //no Child
        if(ptr->left == NULL && ptr->right == NULL){
            delete ptr;
            ptr = NULL;
        }
            //one child
        else if( ptr->right == NULL) {
            ptr = ptr->left;
            delete temp;
        }
        else if( ptr->left == NULL) {
            ptr = ptr->right;
            delete temp;
        }
          //two childs
        else {
            temp = pred(ptr->left);
            ptr->data = temp->data;
            remove(ptr->left, temp->data);
        }
    }
}

template <class T>
Node<T>* BinarySearchTree<T>::pred( Node<T>* ptr) {
    while(ptr->right != NULL) {
        ptr = ptr->right;
    }
    return ptr;
}

template <class T>
bool _Leaf(Node<T> *node) // checks if node is leaf
        {
    if (node == NULL)
        return false;    
    if (node.right == NULL && node.left == NULL)
        return true;
    return false; 
        }   

#endif // End of BINARYSEARCHTREE_H_