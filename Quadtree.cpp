#include "Quadtree.h"
#include "Object.h"
#include <GL/gl.h>

const int maxcap = 1;
const int maxlev = 5;

Quadtree::Quadtree(){
    maxcapacity = maxcap;
    maxlevels = maxlev;
    level = 0;
    nodes = 0;
}

Quadtree::Quadtree(vec2 _pos, vec2 _dimensions){
    pos = _pos;
    dimensions = _dimensions;
    maxcapacity = maxcap;
    maxlevels = maxlev;
    level = 0;
    nodes = 0;
}

void Quadtree::clear(){
    objects.clear();

    if(nodes != 0)
        for(int i = 0;i < 4;i++){
            nodes[i].clear();
        }

    nodes = 0;
}

void Quadtree::split(){
    nodes = new Quadtree[4];
    vec2 half = dimensions / 2;
    nodes[0] = Quadtree(vec2(pos.x, pos.y), half);//NW
    nodes[1] = Quadtree(vec2(pos.x + half.x, pos.y), half);//NE
    nodes[2] = Quadtree(vec2(pos.x + half.x, pos.y + half.y), half);//SE
    nodes[3] = Quadtree(vec2(pos.x,pos.y + half.y), half);//SW
    nodes[0].level = level + 1;
    nodes[1].level = level + 1;
    nodes[2].level = level + 1;
    nodes[3].level = level + 1;
}

int Quadtree::getindex(Object obj){
    int index = -1;

    if(obj.pos.x < pos.x or obj.pos.y < pos.y or obj.pos.x > pos.x + dimensions.x or obj.pos.y > pos.y + dimensions.y)
        return index;

    bool left = obj.pos.x < pos.x + dimensions.x / 2;
    bool up = obj.pos.y < pos.y + dimensions.y / 2;

    if(left and up)
        return 0;
    if(!left and up)
        return 1;
    if(!left and !up)
        return 2;
    if(left and !up)
        return 3;

    return -1;
}

void Quadtree::addobject(Object obj){
    if(nodes != 0){
        int index = getindex(obj);
        if(index != -1){
            nodes[index].addobject(obj);
            return;
        }
    }

    objects.push_back(obj);

    if((signed)objects.size() > maxcapacity and level < maxlevels){
        if(nodes == 0)
            split();

        int i = 0;
        while(i < (signed)objects.size()){
            int id = getindex(objects[i]);
            if(id != -1){
                nodes[id].addobject(objects[i]);
                objects.erase(objects.begin() + i);
            }
            else
                i++;
        }
    }
}

bool Quadtree::contains(Object obj){
    return obj.getleft() > pos.x and obj.getup() > pos.y and
           obj.getright() < pos.x + dimensions.x and
           obj.getdown() < pos.y + dimensions.y;
}

void Quadtree::moveobjects(){
    for(int i = 0;i < (signed)objects.size();i++)
        objects[i].move();
}

void Quadtree::removeobject(Object obj){
    if(nodes == 0){
        for(int i = 0;i < (signed)objects.size();i++)
            if(objects[i].pos.x == obj.pos.x and objects[i].pos.y == obj.pos.y){//temporar
                objects.erase(objects.begin() + i);
                return;
            }
    }

    if(objects.size() == 0)
        clear();
}

void Quadtree::draw(){
    drawquad(vec2(pos.x, pos.y), dimensions, vec4(1,1,1,1), 0);

    for(int i = 0;i < (signed)objects.size();i++)
        objects[i].draw();

    if(nodes != 0)
        for(int i = 0;i < 4;i++)
            nodes[i].draw();
}
