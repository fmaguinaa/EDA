#include <iostream>
#include "array.h"
using namespace std;

int v[100], vcount = 0;

void insert(int val){
    v[vcount] = val;
    vcount++;
    cout << "Val=" << val << "inserted" <<endl;
}

void print(){
    for(auto i = 0; i < vcount ; ++i )
        cout << "v[" << i << "]=" << v[i] << endl;
    vcount++;
}