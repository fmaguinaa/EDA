#include <iostream>
#include "demo.h"
// #include "array.h"

using namespace std;

// g++ -pthread -std=c++17   -o main main.cpp demo.cpp
// g++ -pthread -std=c++20   -o main main.cpp demo.cpp
// g++ -pthread -std=c++2b   -o main main.cpp demo.cpp
// g++ -pthread -std=gnu++2b -o main main.cpp demo.cpp
int main()
{
    // DemoParameters();
    //DemoArray();
    //DemoIterators();
    DemoReverseIterators();
    // DemoArray();
    //DemoIterators();
    // DemoBinaryTree();
    // DemoHash();

    // DemoPreandPostIncrement();
    // DemoSmartPointers();
    DemoDynamicMatrixes();
    return 0;
}