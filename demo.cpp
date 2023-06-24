#include <iostream> // cout
#include "demo.h"
#include "array.h"
using namespace std;

void DemoArray()
{
    cout << "Hello from DemoArray()" <<endl;
    create_array();
    for(auto i = 0 ; i < 15 ; i++)
        insert(i); 
    print();
    destroy_array();


}

void DemoBinaryTree()
{
    cout << "Hello from DemoBinaryTree()" <<endl;
}

void DemoHash()
{
    cout << "Hello from DemoHash()" <<endl;
}

