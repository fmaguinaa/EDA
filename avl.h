#ifndef __AVL_TREE_H__  
#define __AVL_TREE_H__ 
//#include <utility>
//#include <algorithm>
#include <cassert>
#include "types.h"
#include "binarytree.h"
//#include "util.h"
using namespace std;

template <typename Traits>
class CAVL: public BinaryTree<Traits>
{
  public:
    typedef typename Traits::value_type       value_type;
    typedef typename Traits::LinkedValueType  LinkedValueType;
    typedef typename Traits::Node             Node;
    typedef typename Traits::CompareFn        CompareFn;

    typedef BinaryTree<Traits>                Parent;
    typedef CAVL<Traits>                      myself;

protected:
public: 

    size_t getDepth(Node* node){
        if(!node) return 0;
        return node->getDepth();
    }

    void updateDepth(Node* node){
        node->setDepth(max(getDepth(node->getChildRef(0)), getDepth(node->getChildRef(1))) + 1);
    }

    int getBalance(Node* node){
        if(!node) return 0;
        return getDepth(node->getChildRef(0)) - getDepth(node->getChildRef(1));
    }

    Node *CreateNode(value_type &key, LinkedValueType &value, Node *pParent, size_t level, size_t depth)
    {   return new Node(key, value, pParent, level, depth); }

    void balance(Node* node, Node*& root){
        updateDepth(node);
        Node* parent = node->getParent();
        int nodeBalance = getBalance(node);
        if(nodeBalance < -1 || nodeBalance > 1){
            size_t child = -1;
            if(parent) child = parent->getChild(1) == node;
            if(nodeBalance == -2){
                // Left Left Case
                if(getBalance(node->getChild(1)) == -1){
                    root = leftRotate(node, parent);
                }
                // Right Left Case
                if(getBalance(node->getChild(1)) == 1){
                    node->getChildRef(1) = rightRotate(node->getChildRef(1), node);
                    root = leftRotate(node, parent);
                }
            }
            if(nodeBalance == 2){
                // Right Right Case
                if(getBalance(node->getChild(0)) == 1){
                    root = rightRotate(node, parent);
                }
                // Left Right Case
                if(getBalance(node->getChild(0)) == -1){
                    node->getChildRef(0) = leftRotate(node->getChildRef(0), node);
                    root = rightRotate(node, parent);
                }
            }
            if(parent) parent->setChild(root, child);
            else Parent::m_pRoot = root;
        }
        if(parent) balance(parent, root);
    }

    void insert(value_type &key, LinkedValueType &value) { 
        Node* newNode = Parent::internal_insert(key, value, nullptr, Parent::m_pRoot, 0);
        Node* newRoot = nullptr;
        if(newNode->getParent()) balance(newNode->getParent(), newRoot);
    }

    void setChildParent(Node *parent, Node *child, size_t branch){
        parent->getChildRef(branch) = child;
        if (child) child->setParent(parent);
    }

    Node* rightRotate(Node* y, Node* parent){
        Node* x = y->getChild(0);
        Node* T2 = x->getChild(1);
        setChildParent(x, y, 1);
        setChildParent(y, T2, 0);
        x->setParent(parent);
        updateDepth(x);
        updateDepth(y);
        return x;
    }

    Node* leftRotate(Node* x, Node* parent){
        Node* y = x->getChildRef(1);
        Node* T2 = x->getChildRef(0);
        setChildParent(y, x, 0);
        setChildParent(x, T2, 1);
        y->setParent(parent);
        updateDepth(x);
        updateDepth(y);
        return y;
    }
public:

    void print        (ostream &os){
        Parent::preorder([](Node &node){
            node.setLevel((node.getParent() ? node.getParent()->getLevel() : -1) + 1);
        });
        foreach(Parent::printbegin(), Parent::printend(), [](Node &node, ostream& os){
            string whitespace = "";
            for(size_t i = 0; i < 6 * node.getLevel(); i++ )
                whitespace += " ";
            string parent = node.getParent() ? to_string(node.getParent()->getData()) : "Root";
            string keyValue = to_string(node.getData()) + " : " + to_string(node.getValue()) + " (" + parent + ") [" + to_string(node.getDepth()) +"]";
            os << whitespace << keyValue << endl;
        }, os);
    }
};

template<typename Traits>
ostream& operator<<(ostream& os, CAVL<Traits>& tree)
{
    tree.print(os);
    return os;
}

template<typename Traits>
istream& operator>>(istream& is, CAVL<Traits>& tree)
{
    tree.read(is);
    return is;
}

#endif