#ifndef MESH_H
#define MESH_H

// Handles mesh creation and editing including triangles creation and editing, vertex handling, normal calculations, etc... 

#include "LinearAlgebra.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846

typedef struct {
    Vector *vertex[3];
    Vector normal;
} Triangle;

typedef struct {
    Vector** vertices;
    int maxVertexCount;
    int vertexCount;

    Triangle** triangles;
    int triangleCount;
    int maxTriangleCount;
} Mesh;

Mesh* CreateMesh();

Vector CalculateTriangleNormal(Vector* a, Vector* b, Vector* c);

int MeshAddVertex(Mesh* mesh, Vector* vertex);

int MeshAddTriangle(Mesh* mesh, Vector** vertices);

void FreeMesh(Mesh* mesh);

Vector* FindMeshCenter(Mesh* mesh);

int MoveMesh(Mesh* mesh, Vector* pushVector);

int ScaleMesh(Mesh* mesh, double scaleFactor);

int RotateMesh(Mesh* mesh, Matrix* Q);

int RotateMeshUsingAngles(Mesh* mesh, double xTheta, double yTheta, double zTheta);

#endif