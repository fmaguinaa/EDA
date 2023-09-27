#ifndef __BTREE_H__
#define __BTREE_H__

#include <iostream>
#include <mutex>
#include "btreepage.h"
#include "btree_iterator.h"

#define DEFAULT_BTREE_ORDER 3

const size_t MaxHeight = 5; 

template <typename _T, typename _V>
struct BTreeTrait
{
  using value_type = _T;
  using LinkedValueType = _V;
  using Node = CBTreePage<_T, _V>;
  using CompareFn = std::greater<CBTreePage<_T, _V>>;
};

template <typename Trait>
class BTree // this is the full version of the BTree
{
public:
  typedef typename Trait::value_type        value_type;
  typedef typename Trait::LinkedValueType   LinkedValueType;
  typedef typename Trait::Node              Node;
  typedef typename Trait::Node              BTNode;
  typedef typename Trait::CompareFn         CompareFn;
  typedef BTree<Trait> self;
  typedef btree_forward_iterator<self> iterator;
  typedef btree_backward_iterator<self> riterator;
private:
       mutable mutex m_mutex;
public:
       //typedef ObjectInfo iterator;
      //  typedef typename Node::ObjectInfo      ObjectInfo;

public:
       BTree(size_t order = DEFAULT_BTREE_ORDER, bool unique = true)
              : m_Order(order),
                m_Root(2 * order  + 1, unique),
                m_Unique(unique),
                m_NumKeys(0)
       {
              m_Root.SetMaxKeysForChilds(order);
              m_Height = 1;
       }
       ~BTree() {}
       //int           Open (char * name, int mode);
       //int           Create (char * name, int mode);
       //int           Close ();
       bool            Insert (const value_type key, const LinkedValueType value);
       bool            Remove (const value_type key, const LinkedValueType value);
       LinkedValueType       Search (const value_type key)
       {
              lock_guard<mutex> lock(m_mutex);
              LinkedValueType value = -1;
              m_Root.Search(key, value, m_compareFn);
              return value;
       }
       size_t            size()  { return m_NumKeys; }
       size_t            height() { return m_Height;      }
       size_t            GetOrder() { return m_Order;     }

       void            Print (ostream &os)
       {               m_Root.Print(os);                              }
       void            Read(istream& is);
       template<typename Callable, typename... Args>
       void             ForEach( Callable fn, Args&& ...args)
       {                m_Root.ForEach(fn, 0, args...);          }

       template<typename Callable, typename... Args>
       LinkedValueType* FirstThat( Callable fn, Args&& ...args)
       {                return m_Root.FirstThat(fn, 0, args...);          }

       iterator begin(){iterator iter(this, 0); return iter;}
       iterator end(){updateNodes(); iterator iter(this, m_nodes.size()); return iter;}
       riterator rbegin(){updateNodes(); riterator iter(this, m_nodes.size() - 1); return iter;}
       riterator rend(){riterator iter(this, 0); return iter;}

       BTNode* getNode(size_t index){return m_nodes[index];}

       void updateNodes();

protected:
       BTNode          m_Root;
       size_t          m_Height;  // height of tree
       size_t          m_Order;   // order of tree
       size_t          m_NumKeys; // number of keys
       CompareFn       m_compareFn;
       vector<BTNode *> m_nodes;
       bool            m_Unique;  // Accept the elements only once ?
};     

template <typename Trait>
bool BTree<Trait>::Insert(const value_type key, const LinkedValueType value){
       lock_guard<mutex> lock(m_mutex);
       bt_ErrorCode error = m_Root.Insert(key, value, m_compareFn);
       if( error == bt_duplicate )
               return false;
       m_NumKeys++;
       if( error == bt_overflow )
       {
               m_Root.SplitRoot();
               m_Height++;
       }
       return true;
}

template <typename Trait>
bool BTree<Trait>::Remove (const value_type key, const LinkedValueType value)
{
       lock_guard<mutex> lock(m_mutex);
       bt_ErrorCode error = m_Root.Remove(key, value, m_compareFn);
       if( error == bt_duplicate || error == bt_nofound )
               return false;
       m_NumKeys--;
       if( error == bt_rootmerged )
               m_Height--;
       return true;
}

template <typename Trait>
void BTree<Trait>::Read(istream& is) {
       size_t size;
       is >> size;
       string separator;
       value_type key;
       LinkedValueType value;
       while (size--) {
              is >> key;
              is >> separator;
              is >> value;
              Insert(key, value);
       }
}


template <typename value_type, typename LinkedValueType, typename Node>
void pushNode(tagObjectInfo<value_type, LinkedValueType> &info, size_t level, vector<Node *> &nodes)
{
       Node* node = new Node(info.key, info.value);
       node->getDataRef() = info.key;
       node->getValueRef() = info.value;
       nodes.push_back(node);
}

template <typename Trait>
void BTree<Trait>::updateNodes()
{
       m_nodes.clear();
       m_Root.ForEach(&::pushNode<value_type, LinkedValueType, BTNode>, 0, m_nodes);
}

template <typename Trait>
ostream &operator<<(ostream &os, BTree<Trait> &bTree)
{
  bTree.Print(os);
  return os;
}

template <typename Trait>
istream &operator>>(istream &is, BTree<Trait> &bTree)
{
  bTree.Read(is);
  return is;
}

#endif