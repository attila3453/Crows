#ifndef QUADTREE_HEADER
#define QUADTREE_HEADER

#include "Object.h"
#include <vector>

using std::vector;

class Quadtree{
    public:
        Quadtree();
        Quadtree(vec2 pos, vec2 dimensions, int mincapacity = 2, int maxcapacity = 4, int maxdepth = 4);
        ~Quadtree();
        void addobject(Object obj);
        void removeobject(int index);
        void clear();
        vector<Object> getObjectsat(vec2 pos);
        bool contains(Object obj);
        bool contains(double x, double y);
        int getleft();
        int getright();
        int getdown();
        int getup();
        void draw();
        vector<Object> objects;
    private:
        vec2 pos, dimensions;
        int mincapacity, maxcapacity, maxdepth;
        Quadtree* nodes[4];
        bool isleaf;
        int depth;
        void createleaves();
        void moveobjectstoleaves();
};

#endif // QUADTREE_HEADER
