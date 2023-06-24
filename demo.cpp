#include <iostream> // cout
#include "demo.h"
#include "array.h"
using namespace std;

void DemoArray()
{   
    cout << "Hello from DemoArray()" <<endl;
    cout << "Vector #1()" <<endl;
    CArray v1;
    create_array(&v1);
    for(auto i = 0 ; i < 15 ; i++)
        insert(&v1, i); 

    cout << "Vector #2()" <<endl;
    CArray v2;
    create_array(&v2);
    for(auto i = 0 ; i < 12 ; i++)
        insert(&v2, i); 
    
    print(&v1);
    destroy_array(&v1);
    print(&v2);
    destroy_array(&v2);
}

void DemoBinaryTree()
{
    cout << "Hello from DemoBinaryTree()" <<endl;
}

void DemoHash()
{
    cout << "Hello from DemoHash()" <<endl;
}

