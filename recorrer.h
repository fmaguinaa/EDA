#ifndef __RECORRRER_H__  
#define __RECORRRER_H__ 
//#include "object_function.h"
using namespace std;

template <typename T>
void f2(T &x)
{  cout << x << "  "; }

template <typename T>
void f1(T &x)
{  x+= 5; }

template <typename Iterator, typename F>
void recorrer(Iterator ItBegin, Iterator ItEnd, F ope)
{
  auto iter = ItBegin;
  for(; iter != ItEnd ; ++iter)
      ope(*iter);
}

// #1
template <typename Container, typename F>
void recorrer(Container &container, F ope)
{  
    recorrer(container.begin(), container.end(), ope);
}

// TODO implementar el recorrer inverso
// # 2
// template <typename Container, typename F>
// void recorrer_inverso(Container &container, F ope)
// { recorrer(container.rbegin(), container.rend(), ope);
// }

template <typename Container>
void recorrer(Container &container)
{
    using T = typename Container::value_type;
    // using T = typename Container::value_type;
    // recorrer(container, print<T>);  cout << endl; // recorre imprimiendo
    // recorrer(container, inc<T>);  // recorre incrementando
    // recorrer(container, print<T>);  cout << endl; // recorre imprimiendo

//     // funciones lambda
//     recorrer(container, [](T &n){ n-= 5;}); cout << endl; // -5 a todos
//     recorrer(container, print<T>);  cout << endl; // recorre imprimiendo
    
//     OperacionEspecial<T> ope; 
//     recorrer(container, ope);  
//     recorrer(container, print<T>);  cout << endl;
}

#endif
