#ifndef INT_VEC_2D_H
#define INT_VEC_2D_H

#include <stdbool.h>

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

IntVec2D invertX(const IntVec2D* v);

IntVec2D invertY(const IntVec2D* v);

bool isVert(const IntVec2D* v1);

bool isHoriz(const IntVec2D* v1);

bool isOrigin(const IntVec2D* v1);

bool isDiag(const IntVec2D* v1);

bool isVecDivByVec(const IntVec2D* v1, const IntVec2D* v2);

#endif
