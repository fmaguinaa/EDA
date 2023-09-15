#ifndef __DOUBLE_LINKEDLIST_H__
#define __DOUBLE_LINKEDLIST_H__

#include "linkedlist.h"
#include "keynode.h"

template <typename Container>
class backward_iterator
{
public:
  typedef typename Container::Node Node;
  typedef typename Node::Type Type;
  typedef backward_iterator<Container> myself;

private:
  Container *m_pContainer;
  Node *m_pNode;

public:
  backward_iterator(Container *pContainer, Node *pNode) : m_pContainer(pContainer), m_pNode(pNode) {}
  backward_iterator(myself &other)
  {
    m_pContainer = other.m_pContainer;
    m_pNode = other.m_pNode;
  }
  backward_iterator(myself &&other)
  {
    m_pContainer = std::move(other.m_pContainer);
    m_pNode = std::move(other.m_pNode);
  }
  backward_iterator operator=(backward_iterator &iter)
  {
    m_pContainer = std::move(iter.m_pContainer);
    m_pNode = std::move(iter.m_pNode);
  }
  bool operator==(backward_iterator iter)
  {
    return m_pNode == iter.m_pNode;
  }
  bool operator!=(backward_iterator iter)
  {
    return !(*this == iter);
  }
  Node &operator*()
  {
    return *m_pNode;
  }
  backward_iterator operator++()
  {
    m_pNode = m_pNode->getpPrev();
    return *this;
  }
};

template <typename T, typename V>
class NodeDLL : public KeyNode<T, V>
{
public:
  typedef T value_type;
  typedef T Type;
  typedef V LinkedValueType;
  using Parent = class KeyNode<T, V>;

private:
  typedef NodeDLL<T, V> Node;

public:
  Node *m_pNext;
  Node *m_pPrev;

public:
  NodeDLL(value_type key, LinkedValueType value, Node *pNext = nullptr, Node *pPrev = nullptr)
      : Parent(key, value), m_pNext(pNext), m_pPrev(pPrev)
  {
  }
  NodeDLL(value_type key, Node *pNext = nullptr, Node *pPrev = nullptr)
      : m_pNext(pNext), m_pPrev(pPrev)
  {
    Parent::m_key = key;
  }
  NodeDLL(LinkedValueType value, Node *pNext = nullptr, Node *pPrev = nullptr)
      : m_pNext(pNext), m_pPrev(pPrev)
  {
    Parent::m_value = value;
  }
  void setpNext(NodeDLL *pNext) { m_pNext = pNext; }
  Node *getpNext() { return getpNextRef(); }
  Node *&getpNextRef() { return m_pNext; }
  void setpPrev(NodeDLL *pPrev) { m_pPrev = pPrev; }
  Node *getpPrev() { return getpPrevRef(); }
  Node *&getpPrevRef() { return m_pPrev; }
};

template <typename _T, typename _V>
struct DLLAscTraits
{
  using value_type = _T;
  using LinkedValueType = _V;
  using Node = NodeDLL<_T, _V>;
  using CompareFn = std::less<NodeDLL<_T, _V>>;
};

template <typename _T, typename _V>
struct DLLDescTraits
{
  using value_type = _T;
  using LinkedValueType = _V;
  using Node = NodeDLL<_T, _V>;
  using CompareFn = std::greater<NodeDLL<_T, _V>>;
};

template <typename Traits>
class DoubleLinkedList
{
public:
  typedef typename Traits::value_type value_type;
  typedef typename Traits::LinkedValueType LinkedValueType;
  typedef typename Traits::Node Node;
  typedef typename Traits::CompareFn CompareFn;
  typedef DoubleLinkedList<Traits> myself;
  typedef linkedlist_iterator<myself> iterator;
  typedef backward_iterator<myself> riterator;

protected:
  Node *m_pHead = nullptr;
  Node *m_pTail = nullptr;
  size_t m_size = 0;
  CompareFn Compfn;

public:
  size_t size() const { return m_size; }
  bool empty() const { return size() == 0; }
  DoubleLinkedList() {}
  void insert(value_type key, LinkedValueType value)
  {
    Node *pPrevTail = m_pTail;
    Node *pNew = *insert_forward(key, value);
    if (pNew != m_pTail)
      ::CreateBridge(pNew->getpNext()->getpPrevRef(), pNew, &Node::m_pPrev);
    else
      pNew->setpPrev(pPrevTail);
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
  riterator rbegin()
  {
    riterator iter(this, m_pTail);
    return iter;
  }
  riterator rend()
  {
    riterator iter(this, nullptr);
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
    if (size() > 1)
      m_pHead->m_pNext->m_pPrev = m_pHead;
  }
  void push_back(value_type key, LinkedValueType value)
  {
    Node *pPrevTail = m_pTail;
    Node *pNew = CreateNode(key, value, nullptr, nullptr);
    if (m_pTail)
      m_pTail->setpNext(pNew);
    m_pTail = pNew;
    if (!m_pHead)
      m_pHead = pNew;
    m_size++;
    m_pTail->setpPrev(pPrevTail);
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
  Node *CreateNode(
      value_type key, LinkedValueType value, Node *pNext = nullptr, Node *pPrev = nullptr)
  {
    return new Node(key, value, pNext, pPrev);
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
ostream &operator<<(ostream &os, DoubleLinkedList<Trait> &doubleLinkedList)
{
  doubleLinkedList.print(os);
  return os;
}

template <typename Trait>
istream &operator>>(istream &is, DoubleLinkedList<Trait> &doubleLinkedList)
{
  doubleLinkedList.read(is);
  return is;
}

#endif