#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <sstream>
#include "Vector.h"
#include "Quadtree.h"
#include "Object.h"
#include "Camera.h"

using namespace std;
using namespace sf;

int SW = 800, SH = 600;
int mx, my;

Camera camera;

bool semiscacamera;
bool collisiondetected;

Quadtree tree;

string toString(int nr) {
    stringstream ss;
    ss << nr;
    return ss.str();
}

string toString(float nr) {
    stringstream ss;
    ss << nr;
    return ss.str();
}

int random(int a, int b) {
    return a + rand() % (a + b);
}

double nextdouble() {
    return (double)random(0, 100) / 100.;
}

double random(double a, double b) {
    return (b - a) * nextdouble() + a;
}

void reset() {
    camera = Camera(vec2(0, 0), 1);
}

void handlecollisions(){
    //int i, j;

    collisiondetected = false;

    /*for(i = 0;i < (signed)objects.size();i++){
        if(objects[i].radius != -1){
            if(objects[i].pos.x < objects[i].radius){//stanga
                objects[i].pos.x = objects[i].radius;
                objects[i].speed.x *= -1;
            }

            if(objects[i].pos.y < objects[i].radius){//sus
                objects[i].pos.y = objects[i].radius;
                objects[i].speed.y *= -1;
            }

            if(objects[i].pos.x > SW - objects[i].radius){//dreapta
                objects[i].pos.x = SW - objects[i].radius;
                objects[i].speed.x *= -1;
            }

            if(objects[i].pos.y > SH - objects[i].radius){//jos
                objects[i].pos.y = SH - objects[i].radius;
                objects[i].speed.y *= -1;
            }
        }
    }*/

    /*for(i = 0;i < (signed)objects.size();i++)
        for(j = 0;j < (signed)objects.size();j++)
            if(i != j){
                float radsum = objects[i].radius + objects[j].radius;
                float raddif = objects[i].radius - objects[j].radius;

                if(objects[i].radius != -1 and objects[j].radius != -1){//circle-circle
                    if(objects[i].pos.dist2(objects[j].pos) <= radsum * radsum)
                        collisiondetected = true;
                }
            }*/
}

void update(RenderWindow &window) {
    semiscacamera = false;

    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed or Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();

        if(event.type == Event::Resized) {
            SW = event.size.width;
            SH = event.size.height;
            glViewport( 0, 0, SW, SH );
        }

        if(event.type == Event::MouseWheelMoved)
            camera.Scale(event.mouseWheel.delta);


        if(event.type == Event::MouseButtonReleased and
           event.mouseButton.button == Mouse::Left and
           mx > 0 and mx < SW and my > 0 and my < SH){
            Object obj = Object(vec2(mx, my), 30);
            obj.color = vec4(nextdouble(), nextdouble(), nextdouble(), nextdouble());
            tree.addobject(obj);
        }
    }

    if(Keyboard::isKeyPressed(Keyboard::W)){
        camera.Move(vec2(0, -1));
        semiscacamera = true;
    }

    if(Keyboard::isKeyPressed(Keyboard::A)){
        camera.Move(vec2(-1, 0));
        semiscacamera = true;
    }

    if(Keyboard::isKeyPressed(Keyboard::S)){
        camera.Move(vec2(0, 1));
        semiscacamera = true;
    }

    if(Keyboard::isKeyPressed(Keyboard::D)){
        camera.Move(vec2(1, 0));
        semiscacamera = true;
    }

    /*if(objects.size() > 0)
        objects[0].speed = vec2(0, 0);

    if(objects.size() > 0){
        if(Keyboard::isKeyPressed(Keyboard::Up))
            objects[0].speed.y = -2;
        if(Keyboard::isKeyPressed(Keyboard::Down))
            objects[0].speed.y = 2;
        if(Keyboard::isKeyPressed(Keyboard::Right))
            objects[0].speed.x = 2;
        if(Keyboard::isKeyPressed(Keyboard::Left))
            objects[0].speed.x = -2;
    }*/

    if(Keyboard::isKeyPressed(Keyboard::R))
        reset();

    Vector2i mpos = Mouse::getPosition(window);
    mx = mpos.x;
    my = mpos.y;
    mx /= camera.scale;
    my /= camera.scale;
    mx += camera.pos.x;
    my += camera.pos.y;

    /*for(int i = 0;i < (signed)objects.size();i++)
        objects[i].move();*/

    handlecollisions();
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluOrtho2D(0, SW, SH, 0);

    glScalef(camera.scale, camera.scale, 0);

    glTranslatef(-camera.pos.x, -camera.pos.y, 0);

    for(int i = 0;i < (signed)tree.objects.size();i++)
        tree.objects[i].draw();

    drawquad(vec2(mx, my), vec2(5, 5), vec4(1, 1, 1, 1), true);
}

int main() {
    RenderWindow window(VideoMode(SW, SH), "Crows", Style::Default, ContextSettings(32));
    window.setVerticalSyncEnabled(1);
    window.setMouseCursorVisible(false);
    window.setKeyRepeatEnabled(false);

    glEnable(GL_COLOR_MATERIAL);

    window.setActive(true);

    glPointSize(5);
    glLineWidth(1);
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    string hudtext;

    Text hud;
    Font fontubuntu;
    fontubuntu.loadFromFile("data/Ubuntu-R.ttf");
    hud.setFont(fontubuntu);
    hud.setColor(Color::White);
    hud.setCharacterSize(14);

    srand(time(0));
    cout<<"Initial time:"<<time(0)<<'\n';

    camera = Camera(vec2(0, 0), 1);

    tree = Quadtree(vec2(0, 0), vec2(SW, SH));

    while(window.isOpen()) {
        update(window);
        draw();

        window.pushGLStates();

        hudtext = "Hello world!\n";
        hudtext += "Camera pos: " + toString(camera.pos.x) + " " + toString(camera.pos.y) + "\n";
        hudtext += "Camera scale: " + toString(camera.scale) + "\n";
        hudtext += "Mouse pos: " + toString(mx) + " " + toString(my) + "\n";
        hudtext += "Obiecte:" + toString((int)tree.objects.size()) + "\n";

        if(semiscacamera)
            hudtext += "Se misca camera...\n";

        if(collisiondetected)
            hudtext += "Collision!\n";

        hud.setString(hudtext);
        window.draw(hud);

        window.popGLStates();

        window.display();
    }

    return 0;
}
