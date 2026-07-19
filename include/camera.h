#ifndef CAMERA_H
#define CAMERA_H

// Handles camera operations including initialization and editing

#include "LinearAlgebra.h"
#include "image.h"
#include "mesh.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct {
    Vector* position;

    Vector* right;
    Vector* up;
    Vector* forward;

    double fov;
    double aspectRatio;
    double near;
    double far;
} Camera;

Camera* InitializeDefaultCamera();

Vector* GetCameraViewOfVertex(Camera* c, Vector* v);

Triangle* GetCameraViewOfTriangle(Camera* c, Triangle* t);

Triangle** SplitTriangleInTwo(Camera* c, Vector** ins, Vector** outs);

Triangle** ClipTriangleToNear(Camera* c, Vector** ins, Vector** outs);

int ClipTriangleCloseToCamera(Camera* c, Triangle* t, Triangle** res);

Vector* GetPercentageOfVertexPosition(Camera* c, Vector* v);

Vector* CreateImageScaledVertex(Camera* c, Image* i, Vector* v);

Triangle* CreateSkewedTriangle(Image* image, Triangle* t, Camera* c);

int IsVertexInView(Vector* v, Camera* c);

#endif 
