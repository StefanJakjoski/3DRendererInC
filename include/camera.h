#ifndef CAMERA_H
#define CAMERA_H

// Handles camera operations including initialization and editing

#include "LinearAlgebra.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    Vector* position;

    Vector* right;
    Vector* up;
    Vector* forward;

    float fov;
    float near;
    float far;
} Camera;

Camera* InitializeDefaultCamera();

#endif 
