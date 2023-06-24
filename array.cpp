#include <iostream>
#include "array.h"
using namespace std;

int *pVect = nullptr;
int vcount = 0, vmax = 0;

void create_array(){
    pVect   = nullptr;
    vcount  = 0;
    vmax    = 0;
}

void destroy_array(){
    delete [] pVect;
    pVect = nullptr;
    vcount = 0;
    vmax = 0;
}

void resize(){
    int *pTemp = new int[vmax+10];
    for(auto i = 0u ; i < vcount ; ++i)
        pTemp[i]   = pVect[i];
        // *(pTemp+i) = pVect[i];
        // pTemp[i]   = *(pVect+i);
        // *(pTemp+i) = *(pVect+i);
        // *(i+pTemp) = pVect[i];
        // i[pTemp]   = pVect[i];
    delete [] pVect;
    pVect = pTemp;
    vmax +=10;
    cout << "Vector resized vcount=" << vcount << " vmax=" << vmax << endl;
}

void insert(int val){
    if(vcount == vmax) // Array is already full?
        resize();
    pVect[vcount++] = val;
    cout << "Val=" << val << " inserted, vcount=" << vcount << " vmax=" << vmax << endl;
}

void print(){
    for(auto i = 0; i < vcount ; ++i )
        cout << "pVect[" << i << "]=" << pVect[i] << endl;
    cout << "vcount=" << vcount << " vmax=" << vmax << endl;
}

