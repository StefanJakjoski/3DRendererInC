#include "../include/rasterizer.h"

void main(){
    Mesh *mesh = CreateMesh(10,10);

    Vector *v1 = CreateVector2(3);
    Vector *v2 = CreateVector2(3);
    Vector *v3 = CreateVector2(3);
    Vector *v4 = CreateVector2(3);

    vSet(v1,0,10); vSet(v1,1,30); vSet(v1,2,30); 
    vSet(v2,0,70); vSet(v2,1,40); vSet(v2,2,40);
    vSet(v3,0,50); vSet(v3,1,70); vSet(v3,2,50);
    vSet(v4,0,50); vSet(v4,1,10); vSet(v4,2,60);

    MeshAddVertex(mesh,v1);
    MeshAddVertex(mesh,v2);
    MeshAddVertex(mesh,v3);
    MeshAddVertex(mesh,v4);

    Vector *triangle1[3] = {
        mesh->vertices[0],
        mesh->vertices[1],
        mesh->vertices[2]
    };

    Vector *triangle2[3] = {
        mesh->vertices[1],
        mesh->vertices[2],
        mesh->vertices[3]
    };

    MeshAddTriangle(mesh,triangle2);
    MeshAddTriangle(mesh,triangle1);


    Image* image = ImageCreate(100, 100);
    int check = DrawLinesBetweenVertices(image, mesh);
    check = AddColorToAllTriangles(image, mesh);

    char *filename = (char *) calloc(50, sizeof(char));
    strcat(filename, "test10");
    ImageToPPM(image, filename);
}