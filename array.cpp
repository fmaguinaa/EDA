#include <iostream>
#include "array.h"
using namespace std;

CArray::CArray(string name) 
       : m_name(name){
}

CArray::CArray() : CArray("Empty") {
}

CArray::~CArray(){
    cout << "Destroying " << m_name << "..." << endl;
    delete [] m_pVect;
    m_pVect = nullptr;
    m_vcount = 0;
    m_vmax = 0;
}

void CArray::resize(){
    int *pTemp = new int[m_vmax+10];
    for(auto i = 0u ; i < m_vcount ; ++i)
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

void CArray::insert(int val){
    if(m_vcount == m_vmax) // Array is already full?
        resize();
    m_pVect[m_vcount++] = val;
    cout << "Val=" << val << " inserted, m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
}

void CArray::print(){
    cout << "Printing: " << m_name << endl;
    for(auto i = 0; i < m_vcount ; ++i )
        cout << "m_pVect[" << i << "]=" << m_pVect[i] << endl;
    //cout << "m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
}

