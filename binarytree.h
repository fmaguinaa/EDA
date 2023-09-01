#ifndef __BINARY_TREE_H__  
#define __BINARY_TREE_H__ 
#include <string>
#include <cassert>
#include "types.h"
#include "keynode.h"
#include "xtrait.h"
#include "tree_iterators.h"

using namespace std;

template <typename T, typename V>
class NodeBinaryTree : public KeyNode<T, V>
{
public:
    using Type              = T;
    using value_type        = T;
    using LinkedValueType   = V;
    using Parent            = class KeyNode<T, V>;

private:
    typedef NodeBinaryTree<T, V> Node;

public:
    Node*          m_pParent    = nullptr;
    vector<Node *> m_pChild     = {nullptr, nullptr}; // 2 hijos inicializados en nullptr
    size_t         m_level      = 0;
    bool           m_visited    = false;
public:
    NodeBinaryTree(
        value_type key, LinkedValueType value, Node *pParent = nullptr,
        size_t level = 0, Node *p0 = nullptr, Node *p1 = nullptr) 
        : m_pParent(pParent) , m_level(level), Parent(key, value)
    {   m_pChild[0] = p0;   m_pChild[1] = p1;   }

    NodeBinaryTree(
        value_type key, Node *pParent = nullptr,
        size_t level = 0, Node *p0 = nullptr, Node *p1 = nullptr) 
        : m_pParent(pParent) , m_level(level)
    {   Parent::m_key = key; m_pChild[0] = p0;   m_pChild[1] = p1;   }

    NodeBinaryTree(){}

    void      setChild(const Node *pChild, size_t pos)  {   m_pChild[pos] = pChild;  }
    Node    * getChild(size_t branch){ return m_pChild[branch];  }
    Node    *&getChildRef(size_t branch){ return m_pChild[branch];  }
    Node    * getParent() { return m_pParent;   }

    size_t getLevel(){return m_level;}
    bool getVisited(){return m_visited;}
    void setVisited(bool visited){m_visited = visited;}
};

template <typename _T, typename _V, 
          typename _CompareFn = std::less< NodeBinaryTree<_T, _V> & >>
struct BinaryTreeTrait
{
    using  value_type       = _T;
    using  LinkedValueType  = _V;
    using  Node             = NodeBinaryTree<_T, _V>;
    using  CompareFn        = _CompareFn;
};

using BinaryTreeTraitIntIntAsc      = BinaryTreeTrait<int  , int   , std::less<NodeBinaryTree<int, int > >>;
using BinaryTreeTraitFloatStringAsc = BinaryTreeTrait<float, string, std::greater<NodeBinaryTree<float, string > >>;


template <typename Traits>
class BinaryTree
{
  public:
    typedef typename Traits::value_type         value_type;
    typedef typename Traits::LinkedValueType    LinkedValueType;
    typedef typename Traits::Node               Node;
    typedef typename Traits::CompareFn          CompareFn;
    typedef BinaryTree<Traits>                  myself;
    typedef preorder_iterator<myself>           pre_iterator;
    typedef inorder_iterator<myself>            in_iterator;
    typedef postorder_iterator<myself>          post_iterator;
    typedef printer_iterator<myself>            print_iterator;

protected:
    Node    *m_pRoot = nullptr;
    size_t   m_size  = 0;
    CompareFn Compfn;
public: 
    size_t  size()  const       { return m_size;       }
    bool    empty() const       { return size() == 0;  }
    void    insert(value_type &key, LinkedValueType &value) { internal_insert(key, value, nullptr, m_pRoot, 0);  }

    in_iterator     inbegin     () {in_iterator     iter(this,  furthestLeft(m_pRoot)); return iter;}
    in_iterator     inend       () {in_iterator     iter(this, furthestRight(m_pRoot)); return iter;}
    pre_iterator    prebegin    () {pre_iterator    iter(this,               m_pRoot);  return iter;}
    pre_iterator    preend      () {pre_iterator    iter(this, furthestRight(m_pRoot)); return iter;}
    post_iterator   postbegin   () {post_iterator   iter(this,  furthestLeft(m_pRoot)); return iter;}
    post_iterator   postend     () {post_iterator   iter(this,               m_pRoot);  return iter;}
    print_iterator  printbegin  () {print_iterator  iter(this, furthestRight(m_pRoot)); return iter;}
    print_iterator  printend    () {print_iterator  iter(this,  furthestLeft(m_pRoot)); return iter;}
    
protected:
    Node *CreateNode(value_type &key, LinkedValueType &value, Node *pParent, size_t level)
    {   return new Node(key, value, pParent, level); }
    Node *internal_insert(
        value_type &key, LinkedValueType &value, Node *pParent,
        Node *&rpOrigin, size_t level = 0)
    {
        if( !rpOrigin ) //  llegué al fondo de una rama
        {   ++m_size;
            return (rpOrigin = CreateNode(key, value, pParent, level));
        }
        size_t branch = Compfn(key, rpOrigin->getDataRef() );
        return internal_insert(key, value, rpOrigin, rpOrigin->getChildRef(branch), level+1);
    }
    Node* furthestBranch(Node *pNode, size_t child){
        assert(pNode != nullptr);
        while(pNode->getChild(child) != nullptr){
            pNode = pNode->getChild(child);


        }
        return pNode;
    }
    Node* furthestLeft(Node *pNode) { return furthestBranch(pNode, 0);}
    Node* furthestRight(Node *pNode){ return furthestBranch(pNode, 1);}
public:
    // TODO: generalize this function by using iterators and apply any function
    template <typename F, typename... Args>
    void inorder(F func, Args&&... args){
        foreach(inbegin(), inend(), func, args...);
    }

    // TODO: generalize this function by using iterators and apply any function
    template <typename F, typename... Args>
    void preorder(F func, Args&&... args){
        foreach(prebegin(), preend(), func, args...);
    }

    // TODO: generalize this function by using iterators and apply any function
    template <typename F, typename... Args>
    void postorder(F func, Args&&... args){
        foreach(postbegin(), postend(), func, args...);
    }

    // TODO: generalize this function by using iterators and apply any function
    template <typename F, typename... Args>
    void print(F func, Args&&... args){
        foreach(printbegin(), printend(), func, args...);
    }
};

#endif