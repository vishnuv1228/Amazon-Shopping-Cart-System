/**
 * bst.h
 *  Implements a(n unbalanced) BST storing Key,Value pairs
 */

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>

/* -----------------------------------------------------
 * Regular Binary Tree Nodes
 ------------------------------------------------------*/

template <class KeyType, class ValueType>
class Node {
public:
    Node (const KeyType & k, const ValueType & v, Node<KeyType, ValueType> *p)
        : _item(k, v)
        // the default is to create new nodes as leaves
    {
        _parent = p;
        _left = _right = NULL;
    }

    virtual ~Node() {
    }

    std::pair<const KeyType, ValueType> const & getItem () const {
        return _item;
    }

    std::pair<const KeyType, ValueType> & getItem () {
        return _item;
    }

    const KeyType & getKey () const {
        return _item.first;
    }

    const ValueType & getValue () const {
        return _item.second;
    }

    /* the next three functions are virtual because for Red-Black-Trees,
       we'll want to use Red-Black nodes, and for those, the
       getParent, getLeft, and getRight functions should return
       Red-Black nodes, not just ordinary nodes.
       That's an advantage of using getters/setters rather than a struct. */

    virtual Node<KeyType, ValueType> *getParent () const {
        return _parent;
    }

    virtual Node<KeyType, ValueType> *getLeft () const {
        return _left;
    }

    virtual Node<KeyType, ValueType> *getRight () const {
        return _right;
    }


    void setParent (Node<KeyType, ValueType> *p) {
        _parent = p;
    }

    void setLeft (Node<KeyType, ValueType> *l) {
        _left = l;
    }

    void setRight (Node<KeyType, ValueType> *r) {
        _right = r;
    }

    void setValue (const ValueType &v) {
        _item.second = v;
    }

protected:
    std::pair<const KeyType, ValueType> _item;
    Node <KeyType, ValueType> *_left, *_right, *_parent;
};

/* -----------------------------------------------------
 * Regular Binary Search Tree
 ------------------------------------------------------*/

template <class KeyType, class ValueType>
class BinarySearchTree {
protected:
    // Main data member of the class
    Node<KeyType, ValueType> *root;

public:
    /**
     * Constructor
     */
    BinarySearchTree () {
        root = NULL;
    }

    /**
     * Destructor
     */
    ~BinarySearchTree () {
        deleteAll (root);
    }

    /**
     * Prints the entire tree structure in a nice format
     *
     * It will denote subtrees in [] brackets.
     *  This could be helpful if you want to debug your functions.
     */
    void print () const {
        printRoot (root);
        std::cout << "\n";
    }

    /**
     * An In-Order iterator
     *  !!! You must implement this !!!
     */
    class iterator {
    public:
        /**
         * Initialize the internal members of the iterator
         */
        iterator(Node<KeyType,ValueType>* ptr) {
            curr = ptr;
        }

        std::pair<const KeyType,ValueType>& operator*() {
            return curr->getItem();
        }

        std::pair<const KeyType,ValueType>* operator->() {
            return &(curr->getItem());
        }

        /**
         * Checks if 'this' iterator's internals have the same value
         *  as 'rhs'
         */
        bool operator==(const iterator& rhs) const {
            if (curr == rhs.curr)
            {
              return true;
            }
            else
            {
              return false;
            }
        }

        /**
         * Checks if 'this' iterator's internals have a different value
         *  as 'rhs'
         */
        bool operator!=(const iterator& rhs) const {
          
            if (curr == rhs.curr)
            {
              return false;
            }
            else
            {
              return true;
            }
        }
        

        /**
         * Advances the iterator's location using an in-order sequencing
         */
        iterator& operator++() {

          Node<KeyType, ValueType> *temp = curr;
              // Find the right-most node in the tree
              while (temp->getParent() != NULL) {
                temp = temp->getParent();
              }
              while (temp->getRight() != NULL) {
                temp = temp->getRight();
              }
              if (curr == temp)
              {
                curr = NULL;
                return *this;
              }

            if (curr->getRight() != NULL) {
                // If we're at a node that has right children, find the left child (successor)
                curr = curr->getRight();
                while(curr->getLeft() != NULL) {
                    curr = curr->getLeft();
                }
            } else {
                // This is when the left subtree has been traversed already. Look for parent
                // with left child being curr. If the parent is not NULL, it becomes the succesor
                while (curr->getParent() != NULL) {
                    if (curr->getParent()->getKey() > curr->getKey())
                    {
                      curr = curr->getParent();
                      break;
                    }
                    
                    curr = curr->getParent();
                }
            }
            return *this;
            
        }

    protected:
        Node<KeyType, ValueType>* curr;
        //you are welcome to add any necessary variables and helper functions here.
    };

    /**
     * Returns an iterator to the "smallest" item in the tree
     */
    iterator begin() {
        Node<KeyType, ValueType> *temp = root;
        // Find the left-most node in the tree
        while (temp != NULL && temp->getLeft() != NULL) {
            temp = temp->getLeft();
        }
        iterator it(temp);
        return it;
    }

    /**
     * Returns an iterator whose value means INVALID
     */
    iterator end() {
        iterator it(NULL);
        return it;
    }

    /**
     * Returns an iterator to the item with the given key, k
     * or the end iterator if k does not exist in the tree
     */
    iterator find (const KeyType & k) const {
        Node<KeyType, ValueType> *curr = internalFind(k);
        iterator it(curr);
        return it;
    }


protected:
    /**
     * Helper function to find a node with given key, k and
     * return a pointer to it or NULL if no item with that key
     * exists
     */
    Node<KeyType, ValueType>* internalFind(const KeyType& k) const {
        Node<KeyType, ValueType> *curr = root;
        while (curr) {
            if (curr->getKey() == k) {
                return curr;
            } else if (k < curr->getKey()) {
                curr = curr->getLeft();
            } else {
                curr = curr->getRight();
            }
        }
        return NULL;
    }

    /**
     * Helper function to print the tree's contents
     */
    void printRoot (Node<KeyType, ValueType> *r) const {
        if (r != NULL) {
            std::cout << "[";
            printRoot (r->getLeft());
            std::cout << " (" << r->getKey() << ", " << r->getValue() << ") ";
            printRoot (r->getRight());
            std::cout << "]";
        }
    }

    /**
     * Helper function to delete all the items
     */
    void deleteAll (Node<KeyType, ValueType> *r) {
        if (r != NULL) {
            deleteAll (r->getLeft());
            deleteAll (r->getRight());
            delete r;
        }
    }
};

/* Feel free to add member function definitions here if you need */
