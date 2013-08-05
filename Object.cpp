#include "Object.h"
#include <GL/gl.h>
#include <cmath>
#include <vector>

using std::vector;

void drawquad(vec2 a, vec2 dimensions, vec4 color, bool fill){
    glPushMatrix();
    glTranslatef(a.x, a.y, 0);
    glColor4f(color.x, color.y, color.z, color.w);
    glBegin(fill ? GL_QUADS : GL_LINE_LOOP);
        glVertex2d(0, 0);
        glVertex2d(dimensions.x, 0);
        glVertex2d(dimensions.x, dimensions.y);
        glVertex2d(0, dimensions.y);
    glEnd();
    glPopMatrix();
}

void drawcircle(float x, float y, float r, int segments){
    glPushMatrix();
    glTranslatef(-x, -y, 0);
    glBegin( GL_TRIANGLE_FAN );
        glVertex2f(x, y);
        for( int n = 0; n <= segments; ++n ) {
            float const t = 2*3.14159*(float)n/(float)segments;
            glVertex2f(x + sin(t)*r, y + cos(t)*r);
        }
    glEnd();
    glPopMatrix();
}

Object::Object(){
}

Object::Object(vec2 _pos){
    pos = _pos;
    vertices.push_back(pos);
}

Object::Object(vec2 _pos, vec2 _dimensions){
    pos = _pos;
    dimensions = _dimensions;
    vertices.push_back(pos);
}

Object::Object(vector<vec2> _vertices){
    vertices = _vertices;
    pos = vec2(0, 0);
    radius = -1;
}

Object::Object(vec2 _pos, float _radius){
    pos = _pos;
    radius = _radius;
}

int Object::getleft(){
    return pos.x;
}

int Object::getright(){
    return pos.x + dimensions.x;
}

int Object::getup(){
    return pos.y;
}

int Object::getdown(){
    return pos.y + dimensions.y;
}

void Object::move(){
    pos += speed;
}

void Object::draw(){
    glPushMatrix();

    glTranslated(pos.x, pos.y, 0);

    if(radius == -1){
        glColor4f(color.x, color.y, color.z, color.w);

        glBegin(vertices.size() >= 3 ? GL_TRIANGLE_FAN : GL_LINES);
            for(int i = 0;i < (signed)vertices.size();i++)
                glVertex2d(vertices[i].x, vertices[i].y);
        glEnd();

        glColor3f(1, 0, 0);
        glBegin(GL_POINTS);
            for(int i = 0;i < (signed)vertices.size();i++)
                glVertex2d(vertices[i].x, vertices[i].y);
        glEnd();
    }
    else{
        glColor4f(color.x, color.y, color.z, color.w);
        drawcircle(pos.x, pos.y, radius, 32);
    }

    glPopMatrix();
}
