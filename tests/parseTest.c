#include "../include/objReader.h"
#include "../include/rasterizer.h"

int main(){
    char filename[64] = "../assets/Icosahedron/icosahedron.obj";
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
    //RotateMesh(mesh, Q);

    Vector* pushVector = CreateVector2(3);
    vSet(pushVector, 0, 500.0); 
    vSet(pushVector, 1, 500.0);
    vSet(pushVector, 2, 500.0); 
    int moveCheck = MoveMesh(mesh, pushVector);
    moveCheck = ScaleMesh(mesh, 200.0);

    Camera *cam = InitializeDefaultCamera();
    vSet(cam->position, 0, 500);
    vSet(cam->position, 1, 500);
    vSet(cam->position, 2, 150);

    for(int i = 0; i < 20; i++){
        double zPos = vGet(cam->position, 2);
        vSet(cam->position, 2, zPos + 10);

        Image* image = ImageCreate(1000, 1000);
        int check = SkewAllTrianglesAndAddColor(image, mesh, cam);
        //AddColorToAllTriangles(image, mesh);
        char *filenameOut = (char *) calloc(50, sizeof(char));
        if(i < 10)
            sprintf(filenameOut, "zoom/ico_camera_test0%d", i);
        else
            sprintf(filenameOut, "zoom/ico_camera_test%d", i);
        //strcat(filenameOut, "/ico_camera_test5");
        ImageToPPM(image, filenameOut);
    }

    //Image* image = ImageCreate(1000, 1000);
    //int check = SkewAllTrianglesAndAddColor(image, mesh, cam);
    ////AddColorToAllTriangles(image, mesh);
    //char *filenameOut = (char *) calloc(50, sizeof(char));
    //strcat(filenameOut, "ico_camera_test5");
    //ImageToPPM(image, filenameOut);
}