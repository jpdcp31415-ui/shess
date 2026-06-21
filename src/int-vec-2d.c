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

IntVec2D negVecs(const IntVec2D* v)
{
    return (IntVec2D){-v->x, -v->y};
}

int equalVecs(const IntVec2D* v1, const IntVec2D* v2)
{
    return ((v1->x == v2->x) && (v1->y == v2->y));
}
