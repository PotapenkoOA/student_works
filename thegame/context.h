#ifndef CONTEXT_H
#define CONTEXT_H

#include <memory>

struct Point {
    unsigned char x;
    unsigned char y;
};

using PointPtr = std::shared_ptr<Point>;

struct Context
{
    unsigned char color; // 0 - white, 1 - black, 2 - empty
    Point point;
};

using ContextPtr = std::shared_ptr<Context>;

#endif // CONTEXT_H
