#ifndef RASTERIZER_H
#define RASTERIZER_H

// Handles drawing lines between vertices, coloring triangles, creation of color array, etc...
// Uses pixel array defined in Image struct from "image.h" and Mesh struct from "mesh.h"

// Currently hardcoded flat projection which does not utilize camera 

// TODO: update functions to use camera

#include "LinearAlgebra.h"
#include "image.h"
#include "mesh.h"
#include "camera.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>

typedef struct {
    int xStart, xEnd, yStart, yEnd;
    int tileIndexY, tileIndexX;
    Triangle **toRender;
    Color *colors;
    int triangleCount;
    int maxTriangleCount;
} Tile;

//stores single screen split
typedef struct{
    Image *image;
    Tile *tiles;
    int tileCount;

    int threadID;
    int threadCount;
} TileThreadData;

//stores multiple screen splits
typedef struct{
    Image *image;
    Mesh *mesh;
    Camera *camera;

    Tile *tiles;

    int startTriangle;
    int endTriangle;
    int tileNumY, tileNumX;

    Color color;
} GeometryThreadData;

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

int AddColorToSkewedTriangle(Image* image, Triangle *t, Color c);

int SkewAllTrianglesAndAddColor(Image* image, Mesh* m, Camera* c);

int SkewAllTrianglesAndAddMonochrome(Image* image, Mesh* mesh, Camera* cam, Color c);

int ClipSkewAllTrianglesAndAddColor(Image* image, Mesh* mesh, Camera* c);

int ClipSkewAllTrianglesAndAddMonochrome(Image* image, Mesh* mesh, Camera* cam, Color c);

int MonochromeRasterParallel(Image* image, Mesh* mesh, Camera* cam, Color c);

int MonochromeRasterParallel2(Image* image, Mesh* mesh, Camera* cam, Color c);

double TimedMonochromeRasterParallel2(Image* image, Mesh* mesh, Camera* cam, Color c);

double TimedMonochromeRasterParallel(Image* image, Mesh* mesh, Camera* cam, Color c);

#endif