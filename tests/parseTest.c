#include "../include/objReader.h"
#include "../include/rasterizer.h"

int main(){
    char filename[32] = "../assets/Cube/cube.obj";
    printf("%s\n", filename);
    Mesh* mesh = ParseObjFile(filename);

    Vector* avg = FindMeshCenter(mesh);
    printf("Mesh Center: \n");
    VectorToString(avg);

    Matrix *mBase = CreateMatrix2(3, 3);
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            mSet(mBase, i, j, rand());
        }
    }

    Matrix* Q; Matrix* R;
    QRDecomposition(&Q, &R, mBase);
    MatrixToString(Q);
    RotateMesh(mesh, Q);

    Vector* pushVector = CreateVector2(3);
    vSet(pushVector, 0, 500.0); 
    vSet(pushVector, 1, 500.0);
    vSet(pushVector, 2, 0.0); 
    int moveCheck = MoveMesh(mesh, pushVector);
    moveCheck = ScaleMesh(mesh, 200.0);

    

    Image* image = ImageCreate(1000, 1000);
    int check = AddColorToAllTriangles(image, mesh);

    char *filenameOut = (char *) calloc(50, sizeof(char));
    strcat(filenameOut, "parse_cube_test");
    ImageToPPM(image, filenameOut);
}