#include "Camera.h"
#include "Vector.h"

void Camera::Move(vec2 offset){
    pos += offset * movespeed;
}

void Camera::Scale(float offset){
    movespeed *= scale;
    scale += offset * scalespeed;
    movespeed /= scale;
}
