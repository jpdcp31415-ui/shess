#include "../include/int-vec-2d.h"
#include <math.h>

IntVec2D addVecs(const IntVec2D* v1, const IntVec2D* v2)
{
    return (IntVec2D){v1->x + v2->x, v1->y + v2->y};
}

IntVec2D subVecs(const IntVec2D* v1, const IntVec2D* v2)
{
    return (IntVec2D){v1->x - v2->x, v1->y - v2->y};
}

IntVec2D multNumByVec(int n, const IntVec2D* v)
{
    return (IntVec2D){n * v->x, n * v->y};
}

IntVec2D negVec(const IntVec2D* v)
{
    return (IntVec2D){-v->x, -v->y};
}

int equalVecs(const IntVec2D* v1, const IntVec2D* v2)
{
    return ((v1->x == v2->x) && (v1->y == v2->y));
}

IntVec2D invertX(const IntVec2D* v)
{
    return (IntVec2D){-v->x,v->y};
}

IntVec2D invertY(const IntVec2D* v)
{
    return (IntVec2D){v->x,-v->y};
}

bool isVert(const IntVec2D* v1)
{
    return (v1->x == 0 && v1->y != 0);
}

bool isHoriz(const IntVec2D* v1)
{
    return (v1->x != 0 && v1->y == 0);
}

bool isOrigin(const IntVec2D* v1)
{
    return (v1->x == 0 && v1->y == 0);
}

bool isDiag(const IntVec2D* v1)
{
    return (v1->x != 0 && v1->y != 0);
}

bool isVecDivByVec(const IntVec2D* v1, const IntVec2D* v2)
{
    double xRatio = (double) v1->x / v2->x;
    double yRatio = (double) v1->y / v2->y;
    
    if (isinf(xRatio) || isinf(yRatio) ||
        xRatio == 0   || yRatio == 0)
        return false;
        
    if (!isnan(xRatio) && isnan(yRatio))
        return xRatio == (int)xRatio;
        
    if (isnan(xRatio) && !isnan(yRatio))
        return yRatio == (int)yRatio;
    
    if (isnan(xRatio) && isnan(yRatio))
        return false;
        
    return (xRatio == yRatio);
}
