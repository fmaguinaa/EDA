#include <iostream>
#include "array.h"
using namespace std;

void create_array(CArray *pThis){
    pThis->pVect   = nullptr;
    pThis->vcount  = 0;
    pThis->vmax    = 0;
}

void destroy_array(CArray *pThis){
    delete [] pThis->pVect;
    pThis->pVect = nullptr;
    pThis->vcount = 0;
    pThis->vmax = 0;
}

void resize(CArray *pThis){
    int *pTemp = new int[pThis->vmax+10];
    for(auto i = 0u ; i < pThis->vcount ; ++i)
        pTemp[i]   = pThis->pVect[i];
        // *(pTemp+i) = pThis->pVect[i];
        // pTemp[i]   = *(pThis->pVect+i);
        // *(pTemp+i) = *(pThis->pVect+i);
        // *(i+pTemp) = pThis->pVect[i];
        // i[pTemp]   = pThis->pVect[i];
    delete [] pThis->pVect;
    pThis->pVect = pTemp;
    pThis->vmax +=10;
    // cout << "Vector resized vcount=" << vcount << " vmax=" << vmax << endl;
}

void insert(CArray *pThis, int val){
    if(pThis->vcount == pThis->vmax) // Array is already full?
        resize(pThis);
    pThis->pVect[pThis->vcount++] = val;
    cout << "Val=" << val << " inserted, vcount=" << pThis->vcount << " vmax=" << pThis->vmax << endl;
}

void print(CArray *pThis){
    for(auto i = 0; i < pThis->vcount ; ++i )
        cout << "pVect[" << i << "]=" << pThis->pVect[i] << endl;
    //cout << "vcount=" << pThis->vcount << " vmax=" << vmax << endl;
}

