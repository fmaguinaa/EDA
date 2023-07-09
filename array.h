#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <iostream>
#include <algorithm> // sort algorithm
#include "iterator.h"
using namespace std;

template <typename Container>
class array_forward_iterator 
     : public general_iterator<Container,  class array_forward_iterator<Container> > // 
{public: 
    // TODO: subir al padre  
    typedef class general_iterator<Container, array_forward_iterator<Container> > Parent; 
    typedef typename Container::Node           Node; // 
    typedef array_forward_iterator<Container>  myself;

  public:
    array_forward_iterator(Container *pContainer, Node *pNode) 
            : Parent (pContainer,pNode) {}
    array_forward_iterator(myself &other)  : Parent (other) {}
    array_forward_iterator(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

public:
    array_forward_iterator operator++() { Parent::m_pNode = Parent::m_pNode++;  
                                          return *this;
                                        }
};


template <typename T, typename V>
class NodeArray
{
public:
  using KeyType   = T;
  using Type      = T;
  using ValueType = V;
private:
  using Node      = NodeArray<T, V> ;
public:
    KeyType       m_key;
    ValueType      m_value;

public:
    NodeArray(KeyType key, ValueType value) 
        : m_key(key), m_value(value) {}

    NodeArray(const NodeArray<T, V>& other) : 
        NodeArray(other.m_key, other.m_value) {}

    NodeArray(NodeArray<T, V>&& other) // Move constructor
        : m_key  (std::move(other.m_key)), 
          m_value(std::move(other.m_value)) {}
    NodeArray() {}

    NodeArray& operator=(const NodeArray& other) {
        if (this != &other) {
            m_key = other.m_key;
            m_value = other.m_value;
        }
        return *this;
    }

    KeyType    getData() const   { return m_key; }
    KeyType&   getDataRef()      { return m_key; }
    ValueType  getValue() const { return m_value; }
    ValueType& getValueRef()    { return m_value; }

    bool operator<(const NodeArray<T, V>& other) const { 
        return m_key < other.m_key;
    }
    // Error was here. Next line was missing
    bool operator>(const NodeArray<T, V>& other) const { 
        return m_key > other.m_key;
    }
};

template <typename Node>
bool xless (Node &obj1, Node &obj2) 
{ return (obj1.getData() < obj2.getData()); }

template <typename _K, typename _V, 
            typename _CompareFn = std::less< NodeArray<_K, _V> & >>
struct ArrayTrait
{
    using  KeyType   = _K;
    using  ValueType = _V;
    using  Node      = NodeArray<_K, _V>;
    using  CompareFn = _CompareFn;
};

using TraitArrayFloatString = ArrayTrait<float, string>;
using TraitArrayIntInt      = ArrayTrait<int  , int   , std::greater<NodeArray<int  , int > &>>;
using TraitFloatLong        = ArrayTrait<float, long  , std::greater<NodeArray<float, long> &>>;

// Created by: @ecuadros
template <typename Traits>
class CArray{
public:
    using KeyType   = typename Traits::KeyType;
    using ValueType = typename Traits::ValueType ;
    using Node      = typename Traits::Node;
    using CompareFn = typename Traits::CompareFn;
    using myself    = CArray<Traits>;
    using iterator  = array_forward_iterator<myself>;
private:
    Node     *m_pVect = nullptr;
    size_t    m_vcount = 0, m_vmax = 0;
    string    m_name = "Empty";
public:
    CArray(): m_name("Empty"){}
    CArray(string name) : m_name(name) {}
    ~CArray(){
        cout << "Destroying " << m_name << "..." << endl;
        reset();
    }
    void insert(KeyType key, ValueType value){
        if(m_vcount == m_vmax) // Array is already full?
            resize();
        m_pVect[m_vcount++] = Node(key, value);
        // cout << "Key=" << key << " Value=" << value << "\tinserted, m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
    }
    void resize       ();
    void reset(){
        delete [] m_pVect;
        m_pVect = nullptr;
        m_vcount = 0;
        m_vmax = 0;
    }
    
    void print        (ostream &os){
        // os << "Printing: " << m_name << endl;
        os << m_vcount << " " << m_vmax << endl;
        sort(m_pVect, m_pVect+m_vcount, CompareFn() );
        // sort(m_pVect, m_pVect+m_vcount, std::less<Node>());
        for(size_t i = 0; i < m_vcount ; ++i )
            os << m_pVect[i].getData() << "\t: " << m_pVect[i].getValue() << endl;
        //os << "m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
    }
    void read(istream &is){
        reset();
        // read here
    }

    size_t size()
    {  return m_vcount;    }
    KeyType &operator[](size_t pos)
    {   return m_pVect[pos].getDataRef();    }

    iterator begin() { iterator iter(this, m_pVect);    return iter;    }
    iterator end()   { iterator iter(this, nullptr);    return iter;    }
};

template <typename Traits>
void CArray<Traits>::resize(){
    Node *pTemp = new Node[m_vmax+10];
    for(size_t i = 0 ; i < m_vcount ; ++i)
        pTemp[i]   = m_pVect[i];
        // *(pTemp+i) = m_pVect[i];
        // pTemp[i]   = *(m_pVect+i);
        // *(pTemp+i) = *(m_pVect+i);
        // *(i+pTemp) = m_pVect[i];
        // i[pTemp]   = m_pVect[i];
    delete [] m_pVect;
    m_pVect = pTemp;
    m_vmax +=10;
    // cout << "Vector resized m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
}

template <typename T>
ostream &operator<<(ostream &os, CArray<T> &obj){
    obj.print(os);
    return os;
}

// TODO
template <typename T>
istream & operator>>(istream &is, CArray<T> &obj){
    // TODO
    return is;
}


#endif // __ARRAY_H__
