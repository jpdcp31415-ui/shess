#include "../include/int-vec-2d.h"

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

bool signOf(double d) {return d == 0 ? 0 : d >= 0 ? 1 : -1;}

bool isVecDivByVec(const IntVec2D* v1, const IntVec2D* v2)
{
    if (!(signOf(v1->x) == signOf(v2->x) &&
          signOf(v1->y) == signOf(v2->y) &&
          !isOrigin(v1) && !isOrigin(v2)))
        return false;

    if (isDiag(v1) && isDiag(v2))
        return ((double)v1->x / v2->x ==
                (double)v1->y / v2->y);

    if (isHoriz(v1) && isHoriz(v2))
        return (v1->x % v2->x == 0);

    if (isVert(v1) && isVert(v2))
        return (v1->y % v2->y == 0);

    return false;
}

