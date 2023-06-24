#include <iostream> // cout
#include "demo.h"
#include "array.h"
using namespace std;

void DemoArray()
{
    int *pVect1 = nullptr;
    int vcount1 = 0, vmax1 = 0;

    cout << "Hello from DemoArray()" <<endl;
    cout << "Vector #1()" <<endl;
    create_array(pVect1, vcount1, vmax1);
    for(auto i = 0 ; i < 15 ; i++)
        insert(pVect1, vcount1, vmax1, i); 

    cout << "Vector #2()" <<endl;
    int *pVect2 = nullptr;
    int vcount2 = 0, vmax2 = 0;
    create_array(pVect2, vcount2, vmax2);
    for(auto i = 0 ; i < 12 ; i++)
        insert(pVect2, vcount2, vmax2, i); 
    
    print(pVect1, vcount1, vmax1);
    destroy_array(pVect1, vcount1, vmax1);
    print(pVect2, vcount2, vmax2);
    destroy_array(pVect2, vcount2, vmax2);
}

void DemoBinaryTree()
{
    cout << "Hello from DemoBinaryTree()" <<endl;
}

void DemoHash()
{
    cout << "Hello from DemoHash()" <<endl;
}

