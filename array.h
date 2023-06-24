#ifndef __ARRAY_H__
#define __ARRAY_H__

struct CArray{
    int *pVect = nullptr;
    int  vcount = 0, vmax = 0;
};

void create_array (CArray *pThis);
void insert       (CArray *pThis, int val);
void print        (CArray *pThis);
void destroy_array(CArray *pThis);

#endif // __ARRAY_H__
