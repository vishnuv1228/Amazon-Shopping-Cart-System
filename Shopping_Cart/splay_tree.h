#include "bst.h"
#include <iostream>
#include <exception>
#include <cstdlib>

template <class KeyType, class ValueType>

class SplayTree : public BinarySearchTree<KeyType, ValueType> {

public:

    void insert (const std::pair<const KeyType, ValueType>& new_item) {

        if (this->internalFind(new_item.first) == NULL) { //Checks to see if the new node currently exists in tree

            Node<KeyType, ValueType>* new_node = new Node<KeyType, ValueType>(new_item.first, new_item.second, NULL); //Dynamically allocates a new node

            Node<KeyType, ValueType>* parent_temp = NULL;

            Node<KeyType, ValueType>* temp_node = this->root;

            while (temp_node != NULL) { //While the temp node is not NULL

                parent_temp = temp_node;

                // If node's value is less than the root's value, set it to be the root's left child

                if (new_node->getKey() < temp_node->getKey()) { //If the key is less than the current node's key

                    temp_node = temp_node->getLeft(); //Temporarily set the temp node to be the current node's left child
                }

                else {

                    temp_node = temp_node->getRight(); //Otherwise temporarily set the temp node to be the current node's right child
                }

            }

            /*

            The parent of the temp node is a leaf node while the temp node is where the new node belongs

            */

            new_node->setParent(parent_temp); //Parent node of temp node is now new node's parent

            if (parent_temp == NULL) { //If parent node is null, that means new node is now the root

                this->root = new_node;

            }

            else if (new_node->getKey() < parent_temp->getKey()) {

                parent_temp->setLeft(new_node); // //If the new node is smaller than its parent, it is now its parent's left child

            }

            else {

                parent_temp->setRight(new_node); //Else it is its parent's right child

            }

            /*

            New node is a leaf node 

            */
            new_node->setLeft(NULL);

            new_node->setRight(NULL);

            splay(new_node); //Splays the node to fix the tree

        }

        else {

            Node<KeyType, ValueType>* existing_node = this->internalFind(new_item.first); //Retrieves the pointer to that node

            existing_node->setValue(new_item.second); //Updates its value

            return;

        }

    };

    /*

    Slightly modified BST find function

    */

    typename BinarySearchTree<KeyType, ValueType>::iterator find (const  KeyType existing_item) {

        Node<KeyType, ValueType>* temp_node = this->root; //Starts from root and can go all the way down to the leaves

        while (temp_node != NULL) {

            if (existing_item < temp_node->getKey()) {

                temp_node = temp_node->getLeft(); //Set it equal to the left child of current

            }

            else if (existing_item > temp_node->getKey()) {

                temp_node = temp_node->getRight(); //Set it equal to the right child of current

            }

            else {

                splay(temp_node); //Node is found and needs to be splayed

                return typename BinarySearchTree<KeyType, ValueType>::iterator(temp_node); //Returns iterator to the current node

            }

        }

        return typename BinarySearchTree<KeyType, ValueType>::iterator(NULL); //Node not in tree

    };

private:

    /*

    There are 5 cases to choose from

    */

    void splay(Node<KeyType, ValueType>* n) {

        while(n->getParent() != NULL) {

            //Case 1: Splayed node is a child of the root (simple rotation only)

            if(n->getParent()->getParent() == NULL) {

                if(leftChild(n)) { //

                    rightRotate(n->getParent());

                }

                else {

                    leftRotate(n->getParent());

                }

            }

            //Case 2: Splayed node is the right child of a right child (Zag Zag)

            else if(rightChild(n) && rightChild(n->getParent())) {

                leftRotate(grandParent(n));

                leftRotate(n->getParent());

            }

            //Case 3: Splayed node is the left child of a left child (Zig Zig)

            else if(leftChild(n) && leftChild(n->getParent())) {

                rightRotate(grandParent(n));

                rightRotate(n->getParent());

            }

            //Case 4: Splayed node is the left child of a right child (Zag Zig)

            else if(leftChild(n) && rightChild(n->getParent())) {

                rightRotate(n->getParent());

                leftRotate(n->getParent());

            }

            //Case 5: Splayed node is the right child of a left child (Zig Zag)

            else {

                leftRotate(n->getParent());

                rightRotate(n->getParent());

            }
        }
    }

    //Checks if current node is a right child

    bool rightChild(Node<KeyType, ValueType>* n) {

        if (n == n->getParent()->getRight()) {

            return true;

        }

        return false;

    }

    //Checks if current node is a left child

    bool leftChild(Node<KeyType, ValueType>* n) {

        if (n == n->getParent()->getLeft()) {

            return true;
        }

        return false;

    }

    Node<KeyType, ValueType>* grandParent(Node<KeyType, ValueType>* grandchild) {

        if (grandchild->getParent()->getParent() != NULL) {

            return grandchild->getParent()->getParent();

        }

        return NULL;

    }

    void rightRotate(Node<KeyType, ValueType>* n) {

        Node<KeyType, ValueType>* left = n->getLeft(); 

        replace(n, left); 

        n->setLeft(left->getRight()); 

        if (left->getRight() != NULL) { 

            left->getRight()->setParent(n); 

        }

        left->setRight(n);

        n->setParent(left); 

    }

    void leftRotate(Node<KeyType, ValueType>* n) {

        Node<KeyType, ValueType>* right = n->getRight(); 

        replace(n, right); 

        n->setRight(right->getLeft());

        if (right->getLeft() != NULL) { 

            right->getLeft()->setParent(n); 
        }

        right->setLeft(n); 

        n->setParent(right); 

    }

    void replace(Node<KeyType, ValueType>* old_parent, Node<KeyType, ValueType>* new_parent) {

        if (old_parent->getParent() == NULL) { 

            this->root = new_parent; 
        }

        else {

            if (old_parent == old_parent->getParent()->getLeft()) { 

                old_parent->getParent()->setLeft(new_parent);

            }

            else {

                old_parent->getParent()->setRight(new_parent);
            }

        }

        if (new_parent != NULL) {

            new_parent->setParent(old_parent->getParent());

        }
    }

};