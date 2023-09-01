#ifndef __AVL_TREE_H__  
#define __AVL_TREE_H__ 
//#include <utility>
//#include <algorithm>
#include <cassert>
#include "types.h"
#include "binarytree.h"
//#include "util.h"
using namespace std;

template <typename T, typename V>
class NodeAVL : public NodeBinaryTree<T, V> {
public:
    using Type              = T;
    using value_type        = T;
    using LinkedValueType   = V;
    using Parent            = class NodeBinaryTree<T, V>;

private:
    typedef NodeAVL<T, V> Node;
public:
    NodeAVL(
      value_type key, LinkedValueType value, Node *pParent = nullptr,
      size_t level = 0, size_t depth = 1, Node *p0 = nullptr, Node *p1 = nullptr) 
      : Parent(key, value, pParent, level, p0, p1) 
    {Parent::m_depth = depth;}

    NodeAVL(
      value_type key, Node *pParent = nullptr,
      size_t level = 0, size_t depth = 1, Node *p0 = nullptr, Node *p1 = nullptr) 
      : Parent(key, pParent, level, p0, p1)
    {Parent::m_depth = depth;}

    NodeAVL(
      value_type key, Node *pParent = nullptr,
      size_t level = 0, Node *p0 = nullptr, Node *p1 = nullptr) 
      : Parent(key, pParent, level, p0, p1)
    {}

    NodeAVL(){}

    void   updateDepth(){
        Parent::setDepth(max(Parent::getDepth(0), Parent::getDepth(1)) + 1);
    }
    size_t getBalance(){
        return Parent::getDepth(0) - Parent::getDepth(1);
    }
};

template <typename _T, typename _V, class _Node = NodeAVL<_T, _V>,
          typename _CompareFn = std::less< _Node >>
struct AVLTreeTrait
{
    using  value_type       = _T;
    using  LinkedValueType  = _V;
    using  Node             = _Node;
    using  CompareFn        = _CompareFn;
};

using AVLTreeTraitIntIntAsc       = AVLTreeTrait<int  , int   , NodeAVL<int, int>     , std::less<NodeAVL<int, int>> >;
using AVLTreeTraitFloatStringDesc = AVLTreeTrait<float, string, NodeAVL<float, string>, std::greater<NodeAVL<float, string>> >;

template <typename Traits>
class CAVL: public BinaryTree<Traits>
{
  public:
    typedef typename Traits::value_type       value_type;
    typedef typename Traits::LinkedValueType  LinkedValueType;
    typedef typename Traits::Node             Node;
    typedef typename Traits::CompareFn        CompareFn;

    // typedef BinaryTree<Traits>                Parent;
    using Parent            = class BinaryTree<Traits>;
    typedef CAVL<Traits>                      myself;

protected:
public: 

    Node *CreateNode(value_type &key, LinkedValueType &value, Node *pParent, size_t level, size_t depth)
    {   return new Node(key, value, pParent, level, depth); }

    Node *internal_insert(
        value_type &key, LinkedValueType &value, Node *pParent,
        Node *&rpOrigin, size_t level = 0, size_t depth = 1)
    {
        Node *newNode = Parent::internal_insert(key, value, pParent, rpOrigin, level);
        newNode->updateDepth();
        size_t balance = newNode->getBalance();

        // // Left Left Case
        // if (balance > 1 && Parent::Compfn(key, newNode->getChild(0)->getDataRef()))
        //     return rightRotate(newNode);

        // // Right Right Case
        // if (balance < 1 && !Parent::Compfn(key, newNode->getChild(1)->getDataRef()))
        //     return leftRotate(newNode);

        // // Left Right Case
        // if (balance > -1 && !Parent::Compfn(key, newNode->getChild(0)->getDataRef())) {
        //     newNode->setChild(leftRotate(newNode->getChild(0)), 0);
        //     return rightRotate(newNode);
        // }

        // // Right Left Case
        // if (balance < -1 && Parent::Compfn(key, newNode->getChild(1)->getDataRef())) {
        //     newNode->setChild(leftRotate(newNode->getChild(1)), 1);
        //     return leftRotate(newNode);
        // }

        return newNode;
    }

    void insert(value_type &key, LinkedValueType &value) { 
        internal_insert(key, value, nullptr, Parent::m_pRoot, 0, 1); 
    }

    Node* rightRotate(Node* y){
        Node* x = y->getChild(0);
        Node* T2 = x->getChild(1);
        x->setChild(y, 1);
        y->setParent(x);
        y->setChild(T2, 0);
        T2->setParent(y);
        x->updateDepth();
        y->updateDepth();
        return x;
    }

    Node* leftRotate(Node* x){
        Node* y = x->getChild(1);
        Node* T2 = x->getChild(0);
        y->setChild(x, 0);
        x->setParent(y);
        x->setChild(T2, 1);
        T2->setParent(x);
        x->updateDepth();
        y->updateDepth();
        return y;
    }


// protected:
    
// public:
    
// protected:
    
};

#endif