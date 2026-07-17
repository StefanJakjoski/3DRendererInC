#include "../include/camera.h"

Camera* InitializeDefaultCamera(){
    Camera* res = (Camera *) malloc(sizeof(Camera));
    res->position = CreateVector2(0,0,0);

    res->right = CreateVector2(1,0,0);
    res->up = CreateVector2(0,1,0);
    res->forward = CreateVector2(0,0,-1);

    res->fov=60.0f;
    res->near=1.0f;
    res->far=100.0f;

    return res;
}

