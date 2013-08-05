#ifndef Object_HEADER
#define Object_HEADER

#include "Vector.h"
#include <vector>

using std::vector;

class Object{
    public:
        Object();
        Object(vec2 pos);
        Object(vec2 pos, vec2 dimensions);//for rectangle
        Object(vector<vec2> vertices);//for polygon
        Object(vec2 pos, float radius);//for circle

        int getleft();
        int getright();
        int getdown();
        int getup();

        vec2 pos, dimensions;

        vector<vec2> vertices;//for polygon

        float radius;//for circle is different than -1
        vec2 center;//for circle
        vec4 color;
        vec2 speed;

        void move();
        void draw();
};

void drawquad(vec2, vec2, vec4, bool);
void drawcircle(float x, float y, float r, int segments);

#endif // Object_HEADER
