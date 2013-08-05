#include "Quadtree.h"
#include "object.h"
#include <GL/gl.h>

Quadtree::Quadtree(){
}

Quadtree::Quadtree(vec2 _pos, vec2 _dimensions, int _mincapacity, int _maxcapacity, int _maxdepth){
    pos = _pos;
    dimensions = _dimensions;
    isleaf = true;
    maxcapacity = _maxcapacity;
    mincapacity = _mincapacity;
    maxdepth = _maxdepth;
    depth = 0;
}

Quadtree::~Quadtree(){

}

int Quadtree::getleft(){
    return pos.x;
}

int Quadtree::getright(){
    return pos.x + dimensions.x;
}

int Quadtree::getup(){
    return pos.y;
}

int Quadtree::getdown(){
    return pos.y + dimensions.y;
}

void Quadtree::clear(){
    objects.clear();

    if(!isleaf){
        int i;
        for(i = 0;i < 4;i++){
            nodes[i]->clear();
            delete nodes[i];
        }
    }

    isleaf = true;
}

bool Quadtree::contains(Object obj){
    return obj.getleft() > getleft() and obj.getup() > getup() and
           obj.getright() < getright() and
           obj.getdown() < getdown();
}

bool Quadtree::contains(double x, double y){
    return x >= getleft() and x <= getright() and
           y >= getup() and y <= getdown();
}

void Quadtree::createleaves(){
    isleaf = false;
    nodes[0] = new Quadtree(pos, dimensions / 2, mincapacity, maxcapacity, depth + 1);
    nodes[1] = new Quadtree(vec2((getleft() + getright()) / 2, getup()), dimensions / 2, mincapacity, maxcapacity, depth + 1);
    nodes[2] = new Quadtree(vec2(getleft(), (getup() + getdown()) / 2), dimensions / 2, mincapacity, maxcapacity, depth + 1);
    nodes[3] = new Quadtree(vec2((getleft() + getright()) / 2, (getup() + getdown()) / 2), dimensions / 2, mincapacity, maxcapacity, depth + 1);
}

void Quadtree::moveobjectstoleaves(){
    int i, j;
    for(i = 0;i < 4;i++){
        for(j = 0;j < (signed)objects.size();j++){
            if(nodes[i]->contains(objects[j])){
                nodes[i]->addobject(objects[j]);
                objects.erase(objects.begin() + j);
                j--;
            }
        }
    }
}

void Quadtree::addobject(Object obj){
    if(depth >= maxdepth)
        return;

    if(isleaf){
        objects.push_back(obj);

        if((signed)objects.size() == maxcapacity){
            createleaves();
            moveobjectstoleaves();
            isleaf = false;
        }

        return;
    }
    else{
        int i;
        for(i = 0;i < 4;i++){
            if(nodes[i]->contains(obj)){
                nodes[i]->addobject(obj);
                return;
            }
        }

        objects.push_back(obj);
    }
}

void Quadtree::removeobject(int index){
    if(!isleaf){
        if((signed)objects.size() < mincapacity){
            clear();
            return;
        }

        int i;
        for(i = 0;i < 4;i++)
            if(nodes[i]->contains(objects[i]))
                nodes[i]->removeobject(i);
    }else{
        objects.erase(objects.begin() + index);
        return;
    }
}

void Quadtree::draw(){
    drawquad(pos, dimensions / 2, vec4(1, 1, 1, .5), 1);
    drawquad(vec2((getleft() + getright()) / 2, getup()), dimensions / 2, vec4(1, 1, 1, .5), 1);
    drawquad(vec2(getleft(), (getup() + getdown()) / 2), dimensions / 2, vec4(1, 1, 1, .5), 1);
    drawquad(vec2((getleft() + getright()) / 2, (getup() + getdown()) / 2), dimensions / 2, vec4(1, 1, 1, .5), 1);

    int i = 0;
    for(i = 0;i < 4;i++)
        if(!isleaf)
            nodes[i]->draw();
}
