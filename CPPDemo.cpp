#include <iostream>
#include "CPPDemo.h"
using namespace std;

void DemoPreandPostIncrement()
{
    int x = 10, y, z;
    y = x++;
    cout << "y=" << y << " x=" << x << endl;
    x = 10;
    z = ++x;
    cout << "z=" << z << " x=" << x << endl;
}  