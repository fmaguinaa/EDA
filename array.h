#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <iostream>
using namespace std;

template <typename T, typename V>
class NodeArray
{
public:
  typedef T     KeyType;
  typedef V     ValueType;
private:
  typedef NodeArray<T, V> Node;
  public:
    KeyType       m_key;
    ValueType      m_value;
  public:
    NodeArray(KeyType key, ValueType value) 
        : m_key(key), m_value(value)
    {};
    NodeArray(){}
    KeyType         getKey()                {   return m_key;    }
    KeyType        &getKeyRef()             {   return m_key;    }
    ValueType       getValue()               {   return m_value;   }
    ValueType      &getValueRef()            {   return m_value;   }
};

template <typename _K, typename _V>
struct ArrayTrait
{
    using  T         = _K;
    using  ValueType = _V;
    using  Node      = NodeArray<_K, _V>;
    //using  CompareFn = less<T>;
};

// Created by: @ecuadros
template <typename Traits>
class CArray{
private:
    typedef typename Traits::T          KeyType;
    typedef typename Traits::ValueType  ValueType;
    typedef typename Traits::Node       Node;

    // typedef T KeyType;
    Node   *m_pVect = nullptr;
    size_t  m_vcount = 0, m_vmax = 0;
    string  m_name = "Empty";
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
        cout << "Key=" << key << " Value=" << value << "inserted, m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
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
        for(size_t i = 0; i < m_vcount ; ++i )
            os << m_pVect[i].getKey() << endl;
        //os << "m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
    }
    void read(istream &is){
        reset();
        // read here
    }

    size_t size()
    {  return m_vcount;    }
    KeyType &operator[](size_t pos)
    {   return m_pVect[pos].getKeyRef();    }
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
