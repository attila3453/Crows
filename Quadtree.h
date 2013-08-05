#ifndef QUADTREE_HEADER
#define QUADTREE_HEADER

#include "Object.h"
#include <vector>

using std::vector;

struct Quadtree{
    Quadtree(vec2, vec2);
    Quadtree();
    void clear();
    void split();
    int getindex(Object);
    void addobject(Object);
    void removeobject(Object);
    bool contains(Object);
    void moveobjects();
    void draw();
    vector<Object> objects;
    vec2 pos;
    vec2 dimensions;
    Quadtree *nodes;
    int maxcapacity;
    int maxlevels;
    int level;
};

#endif // QUADTREE_HEADER
