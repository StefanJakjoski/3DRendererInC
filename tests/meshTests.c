#include "../include/mesh.h"
#include <stdbool.h>

#define ANSI_GREEN "\e[0;32m"
#define ANSI_RED "\e[0;31m"
#define ANSI_DEF "\x1b[0m"

void Eval(bool res){
    if(res){ 
        printf("\x1b[32m Clear\x1b[0m\n");
    }else{
        printf("\x1b[31m Error\x1b[0m\n");
    }
}

bool CreateMeshTest(){
    Mesh *mesh = CreateMesh(10, 10);

    bool check =
        mesh != NULL &&
        mesh->vertexCount == 0 &&
        mesh->triangleCount == 0 &&
        mesh->maxVertexCount == 10 &&
        mesh->maxTriangleCount == 10;

    FreeMesh(mesh);

    return check;
}

bool MeshAddVertexTest(){
    Mesh *mesh = CreateMesh(10, 10);

    Vector *v = CreateVector2(3);
    vSet(v,0,1);
    vSet(v,1,2);
    vSet(v,2,3);

    MeshAddVertex(mesh, v);

    bool check =
        mesh->vertexCount == 1 &&
        CompareVectors(mesh->vertices[0], v);

    FreeVector(v);
    FreeMesh(mesh);

    return check;
}

bool MeshDuplicateVertexTest(){
    Mesh *mesh = CreateMesh(10,10);

    Vector *v = CreateVector2(3);
    vSet(v,0,1);
    vSet(v,1,2);
    vSet(v,2,3);

    MeshAddVertex(mesh, v);

    vSet(v,0,999);

    bool check = (vGet(mesh->vertices[0],0) == 1);

    FreeVector(v);
    FreeMesh(mesh);

    return check;
}

bool MeshAddTriangleTest(){
    Mesh *mesh = CreateMesh(10,10);

    Vector *v1 = CreateVector2(3);
    Vector *v2 = CreateVector2(3);
    Vector *v3 = CreateVector2(3);

    vSet(v2,0,1);
    vSet(v3,1,1);

    MeshAddVertex(mesh,v1);
    MeshAddVertex(mesh,v2);
    MeshAddVertex(mesh,v3);

    Vector *triangle[3] = {
        mesh->vertices[0],
        mesh->vertices[1],
        mesh->vertices[2]
    };

    MeshAddTriangle(mesh,triangle);

    bool check =
        mesh->triangleCount == 1 &&
        mesh->triangles[0]->vertex[0] == mesh->vertices[0] &&
        mesh->triangles[0]->vertex[1] == mesh->vertices[1] &&
        mesh->triangles[0]->vertex[2] == mesh->vertices[2];

    FreeVector(v1);
    FreeVector(v2);
    FreeVector(v3);
    FreeMesh(mesh);

    return check;
}

bool MeshVertexCapacityTest(){
    Mesh *mesh = CreateMesh(10,10);

    Vector *v = CreateVector2(3);

    for(int i=0;i<20;i++)
        MeshAddVertex(mesh,v);

    bool check = (mesh->vertexCount == 10);

    FreeVector(v);
    FreeMesh(mesh);

    return check;
}

bool MeshTriangleCapacityTest(){
    Mesh *mesh = CreateMesh(10,10);

    Vector *v1 = CreateVector2(3);
    Vector *v2 = CreateVector2(3);
    Vector *v3 = CreateVector2(3);

    MeshAddVertex(mesh,v1);
    MeshAddVertex(mesh,v2);
    MeshAddVertex(mesh,v3);

    Vector *triangle[3]={
        mesh->vertices[0],
        mesh->vertices[1],
        mesh->vertices[2]
    };

    for(int i=0;i<20;i++)
        MeshAddTriangle(mesh,triangle);

    bool check = (mesh->triangleCount == 10);

    FreeVector(v1);
    FreeVector(v2);
    FreeVector(v3);
    FreeMesh(mesh);

    return check;
}

bool TriangleNormalTest(){
    Mesh *mesh = CreateMesh(10,10);

    Vector *v1 = CreateVector2(3);
    Vector *v2 = CreateVector2(3);
    Vector *v3 = CreateVector2(3);

    vSet(v2,0,1);
    vSet(v3,1,1);

    MeshAddVertex(mesh,v1);
    MeshAddVertex(mesh,v2);
    MeshAddVertex(mesh,v3);

    Vector *triangle[3]={
        mesh->vertices[0],
        mesh->vertices[1],
        mesh->vertices[2]
    };

    MeshAddTriangle(mesh,triangle);

    Vector *expected = CreateVector2(3);
    vSet(expected,2,1);

    bool check = CompareVectors(expected,&mesh->triangles[0]->normal);

    FreeVector(expected);
    FreeVector(v1);
    FreeVector(v2);
    FreeVector(v3);
    FreeMesh(mesh);

    return check;
}

void main(){
    printf("Testing Mesh...\n");

    printf("CreateMesh() "); Eval(CreateMeshTest());
    printf("MeshAddVertex() "); Eval(MeshAddVertexTest());
    printf("DuplicateVertex() "); Eval(MeshDuplicateVertexTest());
    printf("MeshAddTriangle() "); Eval(MeshAddTriangleTest());
    printf("TriangleNormal() "); Eval(TriangleNormalTest());
    printf("VertexCapacity() "); Eval(MeshVertexCapacityTest());
    printf("TriangleCapacity() "); Eval(MeshTriangleCapacityTest());
}