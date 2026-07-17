#ifndef RASTERIZER_H
#define RASTERIZER_H

// Handles drawing lines between vertices, coloring triangles, creation of color array, etc...
// Uses pixel array defined in Image struct from "image.h" and Mesh struct from "mesh.h"

// Currently hardcoded flat projection which does not utilize camera 

// TODO: update functions to use camera

#include "LinearAlgebra.h"
#include "image.h"
#include "mesh.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

void DrawVertex(Image* image, Vector* vertex);

double CalculateSlopeBetweenTwoPoints(Vector* v1, Vector* v2);

int LimitValue(int val, int max);

double Sign(Vector* p1, Vector* p2, Vector* p3);

bool PointInTriangle(Vector* point, Triangle* t);

void DrawLineBetweenTwoVertices(Image* image, Vector* v1, Vector* v2);

int DrawLinesBetweenVertices(Image* image, Mesh* mesh);

void AddColorToTriangle(Image* image, Triangle* triangle, Color c);

Color ShadeTriangleFromDiffuseLighting(Triangle* t, Color c);

int AddColorToAllTriangles(Image* image, Mesh* mesh);

int AddMonochromeColorToAllTriangles(Image* image, Mesh* mesh, Color c);

int DefineLightSource(Vector* source);

#endif