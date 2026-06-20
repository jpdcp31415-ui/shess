#ifndef INT_VEC_2D_H
#define INT_VEC_2D_H

typedef struct
{
    int x;
    int y;
} IntVec2D;

IntVec2D addVecs(const IntVec2D* v1, const IntVec2D* v2);

IntVec2D subVecs(const IntVec2D* v1, const IntVec2D* v2);

IntVec2D multNumByVec(int n, const IntVec2D* v);

IntVec2D negVecs(const IntVec2D* v);
    
int equalVecs(const IntVec2D* v1, const IntVec2D* v2);

#endif
