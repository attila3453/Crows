#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace sf;

int SW = 800, SH = 600;
int mx, my;

GLuint loadShaders(const char *vertexpath, const char *fragmentpath){
    GLuint vertexid = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentid = glCreateShader(GL_FRAGMENT_SHADER);

    string vertexcode, fragmentcode, line;

    ifstream stream;
    stream.open(vertexpath);
    for(;getline(stream, line);vertexcode += "\n" + line);
    stream.close();

    stream.open(fragmentpath);
    for(;getline(stream, line);fragmentcode += "\n" + line);
    stream.close();

    GLint ret = GL_FALSE;
    int loglen = 0;
    GLchar *log;

    printf("Compiling shader:%s\n", vertexpath);
    const char *vertexcodeptr = vertexcode.c_str();
    glShaderSource(vertexid, 1, &vertexcodeptr, NULL);
    glCompileShader(vertexid);

    glGetShaderiv(vertexid, GL_COMPILE_STATUS, &ret);
    glGetShaderiv(vertexid, GL_INFO_LOG_LENGTH, &loglen);

    if(loglen){
        glGetShaderInfoLog(vertexid, loglen, NULL, log);
        printf("%s\n", log);
    }

    printf("Compiling shader:%s\n", fragmentpath);
    const char *fragmentcodeptr = fragmentcode.c_str();
    glShaderSource(fragmentid, 1, &fragmentcodeptr, NULL);
    glCompileShader(fragmentid);

    loglen = 0;

    glGetShaderiv(fragmentid, GL_COMPILE_STATUS, &ret);
    glGetShaderiv(fragmentid, GL_INFO_LOG_LENGTH, &loglen);

    if(loglen){
        glGetShaderInfoLog(fragmentid, loglen, NULL, log);
        printf("%s\n", log);
    }

    printf("Linking GL program\n");

    GLuint programid = glCreateProgram();
    glAttachShader(programid, vertexid);
    glAttachShader(programid, fragmentid);
    glLinkProgram(programid);

    loglen = 0;

    glGetProgramiv(programid, GL_LINK_STATUS, &ret);
    glGetProgramiv(programid, GL_INFO_LOG_LENGTH, &loglen);

    if(loglen){
        glGetProgramInfoLog(programid, loglen, 0, log);
        printf("%s\n", log);
    }

    glDeleteShader(vertexid);
    glDeleteShader(fragmentid);

    return programid;
}

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

int main() {
    Window window(VideoMode(SW, SH), "Crows", Style::Default, ContextSettings(32));
    window.setVerticalSyncEnabled(1);
    window.setActive(1);

    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    srand(time(0));

    glewExperimental = 1;
    glewInit();

    GLuint triangleid;
    glGenVertexArrays(1, &triangleid);
    glBindVertexArray(triangleid);

    GLfloat triangledata[] = {
            -1, -1, 0,
            1,  -1, 0,
            0,   1, 0};

    GLuint vertexbuffer;

    GLuint programid = loadShaders("shader.vertex", "shader.fragment");

    glClearColor(0, 0, 1, 1);

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangledata), triangledata, GL_STATIC_DRAW);

    glm::mat4 projectionmat = glm::perspective(45.0f, (float)SW / (float)SH, 0.1f, 100.0f);
    glm::mat4 viewmat = glm::lookAt(glm::vec3(4.0f, 4.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 modelmat = glm::mat4(1.0f);

    while(window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed or Keyboard::isKeyPressed(Keyboard::Escape))
                window.close();

            if(event.type == Event::Resized) {
                SW = event.size.width;
                SH = event.size.height;
                glViewport( 0, 0, SW, SH );
            }

            if(event.type == Event::KeyPressed){
                if(event.key.code == Keyboard::D)
                    modelmat = glm::rotate(modelmat, -5.0f, glm::vec3(0, 1, 0));

                if(event.key.code == Keyboard::A)
                    modelmat = glm::rotate(modelmat, 5.0f, glm::vec3(0, 1, 0));

                if(event.key.code == Keyboard::W)
                    modelmat += glm::translate(modelmat, glm::vec3(-1,0,0));

                if(event.key.code == Keyboard::S)
                    modelmat += glm::translate(modelmat, glm::vec3(1,0,0));
            }
        }

        Vector2i mpos = Mouse::getPosition(window);
        mx = mpos.x;
        my = mpos.y;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glm::mat4 mvp = projectionmat * viewmat * modelmat;
        GLuint matid = glGetUniformLocation(programid, "mvp");
        glUniformMatrix4fv(matid, 1, GL_FALSE, &mvp[0][0]);
        glUseProgram(programid);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glDisableVertexAttribArray(0);

        window.display();
    }

    return 0;
}
