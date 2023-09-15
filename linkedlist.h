#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__
#include <iostream>
#include <string>
#include "types.h"
using namespace std;

template <typename Node, typename MemberType>
void CreateBridge(Node *&rParent, Node *pNew, MemberType _pMember)
{
  Node *Node::*pMember = (Node * Node::*)_pMember;
  pNew->*pMember = rParent;
  rParent = pNew;
}

template <typename T, typename V>
class NodeLinkedList : public KeyNode<T, V>
{
public:
  typedef T value_type;
  typedef T Type;
  typedef V LinkedValueType;
  using Parent = class KeyNode<T, V>;

private:
  typedef NodeLinkedList<T, V> Node;

public:
  Node *m_pNext;

public:
  NodeLinkedList(value_type key, LinkedValueType value, Node *pNext = nullptr)
      : Parent(key, value), m_pNext(pNext){};
  NodeLinkedList(value_type key, Node *pNext = nullptr)
      : m_pNext(pNext)
  {
    Parent::m_key = key;
  };

  NodeLinkedList(LinkedValueType value, Node *pNext = nullptr)
      : m_pNext(pNext)
  {
    Parent::m_value = value;
  };

  void setpNext(NodeLinkedList *pNext) { m_pNext = pNext; }
  Node *getpNext() { return getpNextRef(); }
  Node *&getpNextRef() { return m_pNext; }
};

template <typename Container>
class linkedlist_iterator
{
public:
  typedef typename Container::Node Node;
  typedef typename Node::Type Type;
  typedef linkedlist_iterator<Container> myself;

private:
  Container *m_pContainer;
  Node *m_pNode;

public:
  linkedlist_iterator(Container *pContainer, Node *pNode) : m_pContainer(pContainer), m_pNode(pNode) {}
  linkedlist_iterator(myself &other)
  {
    m_pContainer = other.m_pContainer;
    m_pNode = other.m_pNode;
  }
  linkedlist_iterator(myself &&other)
  {
    m_pContainer = std::move(other.m_pContainer);
    m_pNode = std::move(other.m_pNode);
  }
  linkedlist_iterator operator=(linkedlist_iterator &iter)
  {
    m_pContainer = std::move(iter.m_pContainer);
    m_pNode = std::move(iter.m_pNode);
  }
  bool operator==(linkedlist_iterator iter)
  {
    return m_pNode == iter.m_pNode;
  }
  bool operator!=(linkedlist_iterator iter)
  {
    return !(*this == iter);
  }
  Node &operator*()
  {
    return *m_pNode;
  }
  linkedlist_iterator operator++()
  {
    m_pNode = m_pNode->getpNext();
    return *this;
  }
};

template <typename _T, typename _V>
struct LLTraitAsc
{
  using value_type = _T;
  using LinkedValueType = _V;
  using Node = NodeLinkedList<_T, _V>;
  using CompareFn = less<NodeLinkedList<_T, _V>>;
};

template <typename _T, typename _V>
struct LLTraitDesc
{
  using value_type = _T;
  using LinkedValueType = _V;
  using Node = NodeLinkedList<_T, _V>;
  using CompareFn = greater<NodeLinkedList<_T, _V>>;
};

template <typename Traits>
class LinkedList
{
public:
  typedef typename Traits::value_type value_type;
  typedef typename Traits::LinkedValueType LinkedValueType;
  typedef typename Traits::Node Node;
  typedef typename Traits::CompareFn CompareFn;
  typedef LinkedList<Traits> myself;
  typedef linkedlist_iterator<myself> iterator;

protected:
  Node *m_pHead = nullptr;
  Node *m_pTail = nullptr;
  size_t m_size = 0;
  CompareFn Compfn;

public:
  size_t size() const { return m_size; }
  bool empty() const { return size() == 0; }
  LinkedList() {}
  void insert(value_type key, LinkedValueType value)
  {
    insert_forward(key, value);
  }
  value_type &operator[](size_t pos)
  {
    assert(pos <= size());
    Node *pTmp = m_pHead;
    for (auto i = 0; i < pos; i++)
      pTmp = pTmp->getpNext();
    return pTmp->getDataRef();
  }
  iterator begin()
  {
    iterator iter(this, m_pHead);
    return iter;
  }
  iterator end()
  {
    iterator iter(this, nullptr);
    return iter;
  }
  void push_front(value_type key, LinkedValueType value)
  {
    Node *pNew = CreateNode(key, value);
    pNew->setpNext(m_pHead);
    m_pHead = pNew;
    m_size++;
    if (m_size == 1)
      m_pTail = pNew;
  }
  void push_back(value_type key, LinkedValueType value)
  {
    Node *pNew = CreateNode(key, value, nullptr);
    if (m_pTail)
      m_pTail->setpNext(pNew);
    m_pTail = pNew;
    if (!m_pHead)
      m_pHead = pNew;
    m_size++;
  }

protected:
  Node **findPrev(value_type key)
  {
    return findPrev(m_pHead, key);
  }
  Node **findPrev(Node *&rpPrev, value_type key)
  {
    if (!rpPrev || Compfn(key, rpPrev->getData()))
      return &rpPrev; // Retorna la direccion del puntero que me apunta
    return findPrev((Node *&)rpPrev->getpNextRef(), key);
  }
  Node *CreateNode(value_type key, LinkedValueType value, Node *pNext = nullptr)
  {
    return new Node(key, value, pNext);
  }
  Node **insert_forward(value_type key, LinkedValueType value)
  {
    Node **pParent = findPrev(key);
    Node *pNew = CreateNode(key, value);
    ::CreateBridge(*pParent, pNew, &Node::m_pNext);
    if (!pNew->getpNext())
      m_pTail = pNew;
    return pParent;
  }

public:
  value_type PopHead()
  {
    if (m_pHead)
    {
      Node *pNode = m_pHead;
      value_type data = pNode->getData();
      m_pHead = m_pHead->getpNext();
      delete pNode;
      m_size--;
      if (!m_size)
        m_pTail = nullptr;
      return data;
    }
    throw "hola excepcion"; // Create custom exception pending
  }
  void print(ostream &os)
  {
    Node *node = m_pHead;
    while (node != nullptr)
    {
      os << node->getData() << "\t:\t" << node->getValue() << endl;
      node = node->getpNext();
    }
  }
  void read(istream &is)
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
};

template <typename Trait>
ostream &operator<<(ostream &os, LinkedList<Trait> &linkedList)
{
  linkedList.print(os);
  return os;
}

template <typename Trait>
istream &operator>>(istream &is, LinkedList<Trait> &linkedList)
{
  linkedList.read(is);
  return is;
}

#endif