#include <iostream>
#include "demo.h"
// #include "array.h"

using namespace std;

// g++ -pthread -std=c++17 -o main main.cpp demo.cpp array.cpp
int main()
{
    DemoArray();
    DemoBinaryTree();
    DemoHash();
    return 0;
}