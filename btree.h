#ifndef __BTREE_H__
#define __BTREE_H__

#include <iostream>
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
  typedef typename Trait::value_type        value_type;
  typedef typename Trait::LinkedValueType   LinkedValueType;
  typedef typename Trait::Node              BTNode;
  typedef typename Trait::CompareFn         CompareFn;
       
  // typedef CBTreePage <Trait> BTNode;// useful shorthand

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
       {      LinkedValueType value = -1;
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

protected:
       BTNode          m_Root;
       size_t          m_Height;  // height of tree
       size_t          m_Order;   // order of tree
       size_t          m_NumKeys; // number of keys
       CompareFn       m_compareFn;
       bool            m_Unique;  // Accept the elements only once ?
};     

template <typename Trait>
bool BTree<Trait>::Insert(const value_type key, const LinkedValueType value){
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