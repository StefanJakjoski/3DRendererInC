#include "../include/objReader.h"
#include "../include/rasterizer.h"

#include <time.h>

int main(int argc, char** argv){
    if(argc < 3){
        fprintf(stderr, "Provide input .obj file and output file name.\n");
        return 1;
    }

    char *filename = argv[1];
    printf("%s\n", filename);
    Mesh* mesh = ParseObjFile(filename);

    Vector* avg = FindMeshCenter(mesh);
    printf("Mesh Center: \n");
    VectorToString(avg);

    /*
    srand(time(NULL));
    Matrix *mBase = CreateMatrix2(3, 3);
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            mSet(mBase, i, j, rand());
        }
    }

    Matrix* Q; Matrix* R;
    QRDecomposition(&Q, &R, mBase);
    MatrixToString(Q);
    //RotateMesh(mesh, Q);
    */

    printf("Rotating mesh...\n");
    RotateMeshUsingAngles(mesh, 280.0, 340.0, 30.0);

    Vector* pushVector = CreateVector2(3);
    vSet(pushVector, 0, 500.0); 
    vSet(pushVector, 1, 500.0);
    vSet(pushVector, 2, 0.0); 
    int moveCheck = MoveMesh(mesh, pushVector);
    moveCheck = ScaleMesh(mesh, 25.0);

    avg = FindMeshCenter(mesh);
    printf("New Mesh Center: \n");
    VectorToString(avg);
    
    printf("Vertex count: %d, Triangle count: %d\n", mesh->vertexCount, mesh->triangleCount);

    Vector* light = CreateVector2(3);
    vSet(light, 0, 1.0); vSet(light, 1, -1.0); vSet(light, 2, 0.0);
    MultiplyVector(light, 1.0);
    DefineLightSource(light);

    Image* image = ImageCreate(1000, 1000);
    //int check = AddColorToAllTriangles(image, mesh);
    Color c = (Color) {0, 170, 20};
    AddMonochromeColorToAllTriangles(image, mesh, c);

    char *filenameOut = argv[2];
    ImageToPPM(image, filenameOut);
}