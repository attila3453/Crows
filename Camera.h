#ifndef CAMHEADER
#define CAMHEADER

#include "Vector.h"

static float scspeed = 0.1;
static float mvspeed = 2.0;

class Camera{
    public:
        vec2 pos;
        float scale;
        float scalespeed;
        float movespeed;

        Camera(){
            movespeed = mvspeed;
            scalespeed = scspeed;
        }

        Camera(vec2 _pos, float _scale){
            pos = _pos;
            scale = _scale;
            scalespeed = scspeed;
            movespeed = mvspeed;
        }

        void Move(vec2 offset);
        void Scale(float offset);

    private:
};

#endif // CAMHEADER
