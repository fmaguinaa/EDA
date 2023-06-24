#ifndef __ARRAY_H__
#define __ARRAY_H__

void create_array (int *&pVect, int &vcount, int &vmax);
void insert       (int *&pVect, int &vcount, int &vmax, int val);
void print        (int *&pVect, int &vcount, int &vmax);
void destroy_array(int *&pVect, int &vcount, int &vmax);

#endif // __ARRAY_H__
