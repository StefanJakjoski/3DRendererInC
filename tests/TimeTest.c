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
    RotateMeshUsingAngles(mesh, 270.0, 200.0, 0.0);

    Vector* pushVector = CreateVector2(3);
    vSet(pushVector, 0, 400.0); 
    vSet(pushVector, 1, 500.0);
    vSet(pushVector, 2, 200.0); 
    int moveCheck = MoveMesh(mesh, pushVector);
    moveCheck = ScaleMesh(mesh, 5.0);

    avg = FindMeshCenter(mesh);
    printf("New Mesh Center: \n");
    VectorToString(avg);
    
    printf("Vertex count: %d, Triangle count: %d\n", mesh->vertexCount, mesh->triangleCount);

    Vector* light = CreateVector2(3);
    vSet(light, 0, 1.0); vSet(light, 1, -1.0); vSet(light, 2, 0.0);
    MultiplyVector(light, 1.0);
    DefineLightSource(light);

    Camera *cam = InitializeDefaultCamera();
    vSet(cam->position, 0, 400);
    vSet(cam->position, 1, 500);
    vSet(cam->position, 2, 120);

    int repetitions = 5;
    double elapsedAvg = 0.0;

    for(int i = 0; i < repetitions; i++){
        //double zPos = vGet(cam->position, 2);
        //vSet(cam->position, 2, zPos+30);
        //cam->fov += 0.1;

        Image* image = ImageCreate(1000, 1000);
        //int check = AddColorToAllTriangles(image, mesh);
        Color c = (Color) {0, 170, 20};
        
        //AddMonochromeColorToAllTriangles(image, mesh, c);
        
        //clock_t start = clock();
        ////ClipSkewAllTrianglesAndAddMonochrome(image, mesh, cam, c);
        //MonochromeRasterParallel2(image, mesh, cam, c);
        ////MonochromeRasterParallel(image, mesh, cam, c);
        //clock_t end = clock();
        //double elapsedMs = 1000.0 * (end - start) / CLOCKS_PER_SEC;
        
        //double elapsedMs = TimedMonochromeRasterParallel(image, mesh, cam, c);
        double elapsedMs = TimedMonochromeRasterParallel2(image, mesh, cam, c);
        printf("Completed rasterization in %lfms\n\n", elapsedMs);
        elapsedAvg += elapsedMs;

        //ClipSkewAllTrianglesAndAddColor(image, mesh, cam);

        char *filenameOut = argv[2];
        char filenameFinal[64];
        if(i < 10)
            sprintf(filenameFinal, "zoom/%s0%d", filenameOut, i);
        else
            sprintf(filenameFinal, "zoom/%s%d", filenameOut, i);
        ImageToPPM(image, filenameFinal);
    }

    elapsedAvg /= (double) repetitions;
    printf("\n\nAverage time elapsed: %lfms\n", elapsedAvg);

    //Image* image = ImageCreate(1000, 1000);
    ////int check = AddColorToAllTriangles(image, mesh);
    //Color c = (Color) {0, 170, 20};
    //
    ////AddMonochromeColorToAllTriangles(image, mesh, c);
    //SkewAllTrianglesAndAddMonochrome(image, mesh, cam, c);
    //
    //char *filenameOut = argv[2];
    //ImageToPPM(image, filenameOut);
}