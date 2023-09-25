#ifndef __HASH_H__
#define __HASH_H__

#include <iostream>
#include "keynode.h"

using namespace std;

const int TABLE_SIZE = 10;

template <typename T, typename V>
class HashNode : public KeyNode<T, V> {
public:
  typedef T value_type;
  typedef T Type;
  typedef V LinkedValueType;
  using Parent = class KeyNode<T, V>;

private:
  typedef HashNode<T, V> Node;

private:
  Node* m_next;

public:
  HashNode(value_type key, LinkedValueType value) : Parent(key, value) { }
  HashNode *getNext() const { return m_next; }
  void setNext(HashNode *next) { m_next = next;}
};

template <typename T>
struct KeyHash {
  size_t operator()(const T& key) const
  {
    return reinterpret_cast<size_t>(key) % TABLE_SIZE;
  }
};

template <typename _T, typename _V, typename F = KeyHash<_T>>
struct HashMapTraits
{
  using value_type = _T;
  using LinkedValueType = _V;
  using Node = HashNode<_T, _V>;
  using KeyHash = F;
};

template <typename Traits>
class HashMap {
public:
  typedef typename Traits::value_type value_type;
  typedef typename Traits::LinkedValueType LinkedValueType;
  typedef typename Traits::Node Node;
  typedef typename Traits::KeyHash KeyHash;
  typedef HashMap<Traits> myself;

private:
  Node** m_table;
  KeyHash m_hashFunc;

public:
  HashMap() {
    m_table = new Node *[TABLE_SIZE]();
  }

  ~HashMap(){}

  bool get(const value_type& key, LinkedValueType& value){
    size_t hashValue = m_hashFunc(key);
    Node *entry = m_table[hashValue];

    while (entry != nullptr) {
      if (entry->getData() == key) {
        value = entry->getValue();
        return true;
      }
      entry = entry->getNext();
    }
    return false;
  }

  void insert(const value_type& key, const LinkedValueType& value) {
    size_t hashValue = m_hashFunc(key);
    Node *prev = nullptr;
    Node *entry = m_table[hashValue];

    while (entry != nullptr && entry->getData() != key) {
      prev = entry;
      entry = entry->getNext();
    }

    if (entry == nullptr) {
      entry = new Node(key, value);
      if (prev == nullptr) {
        m_table[hashValue] = entry;
      }
      else {
        prev->setNext(entry);
      }
    }
    else {
      entry->getValueRef() = value;
    }
  }

  void remove(const value_type &key) {
    size_t hashValue = m_hashFunc(key);
    Node *prev = nullptr;
    Node *entry = m_table[hashValue];

    while (entry != nullptr && entry->getData() != key) {
      prev = entry;
      entry = entry->getNext();
    }

    if (entry == nullptr) {
      return;
    }
    else {
      if (prev == nullptr) {
        m_table[hashValue] = entry->getNext();
      }
      else {
        prev->setNext(entry->getNext());
      }
      delete entry;
    }
  }

  void print(ostream &os)
  {
    size_t i = 0;
    while (i != TABLE_SIZE)
    {
      Node *node = m_table[i];
      os << i;
      while(node != nullptr){
        os << " => (" << node->getData() << " : " << node->getValue() << ") ";
        node = node->getNext();
      }
      os << endl;
      i++;
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
ostream &operator<<(ostream &os, HashMap<Trait> &hashMap)
{
  hashMap.print(os);
  return os;
}

template <typename Trait>
istream &operator>>(istream &is, HashMap<Trait> &hashMap)
{
  hashMap.read(is);
  return is;
}

#endif
