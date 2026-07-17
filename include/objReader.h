#ifndef OBJ_READER_H
#define OBJ_READER_H

// Responsible for parsing .obj files
// Currently limited to only v (vertices), f (faces)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mesh.h"

void AddOneOrMoreTriangles(FILE* in, Mesh* mesh);

Vector* ParseVertex(FILE* in);

Mesh* ParseObjFile(char* filename);

#endif