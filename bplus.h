#ifndef __B_PLUS_TREE_H__
#define __B_PLUS_TREE_H__

#include <iostream>
#include "keynode.h"

using namespace std;

int MAX = 3;

template <typename T, typename V>
class CBPlusNode : public KeyNode<T, V>
{
public:
  typedef T value_type;
  typedef T Type;
  typedef V LinkedValueType;
  using Parent = class KeyNode<T, V>;
  // friend class CBPlus;
private:
  typedef CBPlusNode<T, V> Node;

public:
  bool m_leaf = true;
  Node **m_ptr = new Node *[MAX + 1];
  value_type *key = new value_type[MAX];
  size_t size;

public:
  CBPlusNode(value_type key, LinkedValueType value) : Parent(key, value)
  {
  }
  CBPlusNode(value_type _key)
  {
    Parent::m_key = _key;
  }
  CBPlusNode() {}
  bool isLeaf() { return m_leaf; }
};

// class Node {
//   bool m_leaf;
//   int *key, size;
//   Node **m_ptr;
//   friend class CBPlus;

// public:
//   Node();
// };
// Node::Node() {
//   key = new int[MAX];
//   m_ptr = new Node *[MAX + 1];
// }

template <typename _T, typename _V>
struct CBPlusTraitAsc
{
  using value_type = _T;
  using LinkedValueType = _V;
  using Node = CBPlusNode<_T, _V>;
  using CompareFn = std::less<CBPlusNode<_T, _V>>;
};

template <typename _T, typename _V>
struct CBPlusTraitDesc
{
  using value_type = _T;
  using LinkedValueType = _V;
  using Node = CBPlusNode<_T, _V>;
  using CompareFn = std::greater<CBPlusNode<_T, _V>>;
};

template <typename Traits>
class CBPlus
{
  typedef typename Traits::value_type value_type;
  typedef typename Traits::LinkedValueType LinkedValueType;
  typedef typename Traits::Node Node;
  typedef typename Traits::CompareFn CompareFn;

private:
  Node *m_root;
  CompareFn m_compareFn;
  void insertInternal(value_type, LinkedValueType, Node *, Node *);
  Node *findParent(Node *, Node *);

public:
  CBPlus() { m_root = nullptr; }
  void search(value_type);
  void insert(value_type, LinkedValueType);
  void print(ostream &, Node *);
  void print(ostream &);
  void read(istream &);
  Node *getRoot();
};

template <typename Traits>
void CBPlus<Traits>::search(value_type x)
{
  if (m_root == nullptr)
  {
    cout << "Tree is empty\n";
  }
  else
  {
    Node *cursor = m_root;
    while (cursor->m_leaf == false)
    {
      for (int i = 0; i < cursor->size; i++)
      {
        if (m_compareFn(x, cursor->key[i]))
        {
          cursor = cursor->m_ptr[i];
          break;
        }
        if (i == cursor->size - 1)
        {
          cursor = cursor->m_ptr[i + 1];
          break;
        }
      }
    }
    for (int i = 0; i < cursor->size; i++)
    {
      if (cursor->key[i] == x)
      {
        cout << "Found\n";
        return;
      }
    }
    cout << "Not found\n";
  }
}

template <typename Traits>
void CBPlus<Traits>::read(istream &is)
{
  size_t size;
  is >> size;
  string separator;
  value_type key;
  LinkedValueType value;
  while (size--)
  {
    is >> key;
    is >> separator;
    is >> value;
    insert(key, value);
  }
}

template <typename Traits>
void CBPlus<Traits>::insert(value_type key, LinkedValueType value)
{
  if (m_root == nullptr)
  {
    m_root = new Node;
    m_root->key[0] = key;
    m_root->m_leaf = true;
    m_root->size = 1;
  }
  else
  {
    Node *cursor = m_root;
    Node *parent;
    while (cursor->m_leaf == false)
    {
      parent = cursor;
      for (int i = 0; i < cursor->size; i++)
      {
        if (m_compareFn(key, cursor->key[i]))
        {
          cursor = cursor->m_ptr[i];
          break;
        }
        if (i == cursor->size - 1)
        {
          cursor = cursor->m_ptr[i + 1];
          break;
        }
      }
    }
    if (cursor->size < MAX)
    {
      int i = 0;
      while (!m_compareFn(key, cursor->key[i]) && i < cursor->size)
        i++;
      for (int j = cursor->size; j > i; j--)
      {
        cursor->key[j] = cursor->key[j - 1];
      }
      cursor->key[i] = key;
      cursor->size++;
      cursor->m_ptr[cursor->size] = cursor->m_ptr[cursor->size - 1];
      cursor->m_ptr[cursor->size - 1] = nullptr;
    }
    else
    {
      Node *newLeaf = new Node;
      int virtualNode[MAX + 1];
      for (int i = 0; i < MAX; i++)
      {
        virtualNode[i] = cursor->key[i];
      }
      int i = 0, j;
      while (!m_compareFn(key, cursor->key[i]) && i < MAX)
        i++;
      for (int j = MAX + 1; j > i; j--)
      {
        virtualNode[j] = virtualNode[j - 1];
      }
      virtualNode[i] = key;
      newLeaf->m_leaf = true;
      cursor->size = (MAX + 1) / 2;
      newLeaf->size = MAX + 1 - (MAX + 1) / 2;
      cursor->m_ptr[cursor->size] = newLeaf;
      newLeaf->m_ptr[newLeaf->size] = cursor->m_ptr[MAX];
      cursor->m_ptr[MAX] = nullptr;
      for (i = 0; i < cursor->size; i++)
      {
        cursor->key[i] = virtualNode[i];
      }
      for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++)
      {
        newLeaf->key[i] = virtualNode[j];
      }
      if (cursor == m_root)
      {
        Node *newRoot = new Node;
        newRoot->key[0] = newLeaf->key[0];
        newRoot->m_ptr[0] = cursor;
        newRoot->m_ptr[1] = newLeaf;
        newRoot->m_leaf = false;
        newRoot->size = 1;
        m_root = newRoot;
      }
      else
      {
        insertInternal(newLeaf->key[0], value, parent, newLeaf);
      }
    }
  }
}

template <typename Traits>
void CBPlus<Traits>::insertInternal(value_type x, LinkedValueType value, Node *cursor, Node *child)
{
  if (cursor->size < MAX)
  {
    int i = 0;
    while (!m_compareFn(x, cursor->key[i]) && i < cursor->size)
      i++;
    for (int j = cursor->size; j > i; j--)
    {
      cursor->key[j] = cursor->key[j - 1];
    }
    for (int j = cursor->size + 1; j > i + 1; j--)
    {
      cursor->m_ptr[j] = cursor->m_ptr[j - 1];
    }
    cursor->key[i] = x;
    cursor->size++;
    cursor->m_ptr[i + 1] = child;
  }
  else
  {
    Node *newInternal = new Node;
    int virtualKey[MAX + 1];
    Node *virtualPtr[MAX + 2];
    for (int i = 0; i < MAX; i++)
    {
      virtualKey[i] = cursor->key[i];
    }
    for (int i = 0; i < MAX + 1; i++)
    {
      virtualPtr[i] = cursor->m_ptr[i];
    }
    int i = 0, j;
    while (x > virtualKey[i] && i < MAX)
      i++;
    for (int j = MAX + 1; j > i; j--)
    {
      virtualKey[j] = virtualKey[j - 1];
    }
    virtualKey[i] = x;
    for (int j = MAX + 2; j > i + 1; j--)
    {
      virtualPtr[j] = virtualPtr[j - 1];
    }
    virtualPtr[i + 1] = child;
    newInternal->m_leaf = false;
    cursor->size = (MAX + 1) / 2;
    newInternal->size = MAX - (MAX + 1) / 2;
    for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++)
    {
      newInternal->key[i] = virtualKey[j];
    }
    for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++)
    {
      newInternal->m_ptr[i] = virtualPtr[j];
    }
    if (cursor == m_root)
    {
      Node *newRoot = new Node;
      newRoot->key[0] = cursor->key[cursor->size];
      newRoot->m_ptr[0] = cursor;
      newRoot->m_ptr[1] = newInternal;
      newRoot->m_leaf = false;
      newRoot->size = 1;
      m_root = newRoot;
    }
    else
    {
      insertInternal(cursor->key[cursor->size], value, findParent(m_root, cursor), newInternal);
    }
  }
}

template <typename Traits>
typename CBPlus<Traits>::Node *CBPlus<Traits>::findParent(Node *cursor, Node *child)
{
  Node *parent;
  if (cursor->m_leaf || (cursor->m_ptr[0])->m_leaf)
  {
    return nullptr;
  }
  for (int i = 0; i < cursor->size + 1; i++)
  {
    if (cursor->m_ptr[i] == child)
    {
      parent = cursor;
      return parent;
    }
    else
    {
      parent = findParent(cursor->m_ptr[i], child);
      if (parent != nullptr)
        return parent;
    }
  }
  return parent;
}

template <typename Traits>
void CBPlus<Traits>::print(ostream &os, Node *cursor)
{
  if (cursor != nullptr)
  {
    for (int i = 0; i < cursor->size; i++)
    {
      os << cursor->key[i] << " ";
      // os << cursor->key[i].getData() << " : " << cursor->key[i].getValue() << " ";
    }
    os << endl;
    if (cursor->m_leaf != true)
    {
      for (int i = 0; i < cursor->size + 1; i++)
      {
        print(os, cursor->m_ptr[i]);
      }
    }
  }
}

template <typename Traits>
void CBPlus<Traits>::print(ostream &os)
{
  print(os, m_root);
}

template <typename Traits>
typename CBPlus<Traits>::Node *CBPlus<Traits>::getRoot()
{
  return m_root;
};

template <typename Traits>
ostream &operator<<(ostream &os, CBPlus<Traits> &bplus)
{
  bplus.print(os);
  return os;
}

template <typename Traits>
istream &operator>>(istream &is, CBPlus<Traits> &bplus)
{
  bplus.read(is);
  return is;
}

#endif