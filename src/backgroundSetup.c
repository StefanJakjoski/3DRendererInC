#include "../include/backgroundSetup.h"

int ColorBackgroundGradient(Image *image, Color a, Color b){
    for(int i = 0; i < image->height; i++){
        float aRatio = (float)(i+1) / (float)image->height;

        int R = (float) b.R * aRatio + (float)a.R * (1.0f - aRatio);
        int G = (float) b.G * aRatio + (float)a.G * (1.0f - aRatio);
        int B = (float) b.B * aRatio + (float)a.B * (1.0f - aRatio);
        for(int j = 0; j < image->width; j++){
            image->pixels[i][j] = (Color) {R, G, B};
        }

        printf("%d %d %d\n", R, G, B);
    }
}

int AddGridlines(Image* image, Camera* cam, double intervals){
    Vector *pos = cam->position;
    double xMiddle = floor(vGet(pos, 0) / intervals) * intervals;
    double zMiddle = floor(vGet(pos, 2) / intervals) * intervals;

    Vector *baseForwardLine = CreateVector2(3);
    vSet(baseForwardLine, 0, xMiddle); 
    vSet(baseForwardLine, 1, vGet(pos, 1) - 50.0);  
    vSet(baseForwardLine, 2, vGet(pos, 2) - 9999.0);

    //Vector *intervalVector = CreateVector2(3);
    //vSet(intervalVector, 0, intervals);
    //vSet(intervalVector, 1, 0.0);
    //vSet(intervalVector, 2, intervals);

    for(int i = -10; i <= 10; i++){
        //calculate vertex
        Vector *forwardLineStart = baseForwardLine;

        vSet(forwardLineStart, 2, vGet(pos, 2) - 9999.0);
        double xNew = xMiddle + intervals * i;
        vSet(forwardLineStart, 0, xNew);
        Vector *forwardLineStartCamera = GetCameraViewOfVertex(cam, forwardLineStart);

        Vector *forwardLineEnd = baseForwardLine; 
        vSet(forwardLineEnd, 2, vGet(pos, 2) + 9999.0);
        Vector *forwardLineEndCamera = GetCameraViewOfVertex(cam, forwardLineEnd);


        vSet(forwardLineStartCamera, 2, cam->near);
        vSet(forwardLineEndCamera, 2, cam->far);


        Vector *forwardLineStartSkewed = CreateImageScaledVertex(cam, image, forwardLineStartCamera);
        Vector *forwardLineEndSkewed = CreateImageScaledVertex(cam, image, forwardLineEndCamera);

        DrawLineBetweenTwoVertices(image, forwardLineStartSkewed, forwardLineEndSkewed);

        FreeVector(forwardLineStartCamera);
        FreeVector(forwardLineEndCamera);
        FreeVector(forwardLineStartSkewed);
        FreeVector(forwardLineEndSkewed);
    }


    FreeVector(baseForwardLine);



    //SIDE LINES

    //vSet(intervalVector, 0, 0.0);
    //vSet(intervalVector, 1, 0.0);
    //vSet(intervalVector, 2, intervals);
    //double zIntervalOffset = intervals;


    Vector *baseSideLine = CreateVector2(3);
    vSet(baseSideLine, 0, vGet(pos, 0) - 9999.0); 
    vSet(baseSideLine, 1, vGet(pos, 1) - 50.0);  
    vSet(baseSideLine, 2, zMiddle);

    for(int i = -20; i <= 20; i++){
        //calculate vertex
        Vector *sideLineStart = baseSideLine;
        vSet(sideLineStart, 0, vGet(pos, 0) - 9999.0);
        //Vector *currentIntervalOffset = MultiplyVector2(intervalVector, i);
        double zNew = zMiddle + intervals * i;
        vSet(sideLineStart, 2, zNew);
        Vector *sideLineStartCamera = GetCameraViewOfVertex(cam, sideLineStart);

        Vector *sideLineEnd = baseSideLine;
        vSet(sideLineEnd, 0, vGet(pos, 0) + 9999.0);
        Vector *sideLineEndCamera = GetCameraViewOfVertex(cam, sideLineEnd);

        if(vGet(sideLineStartCamera, 2) < cam->near)
            continue;

        //printf("Camera view of vertices\n");
        //VectorToString(sideLineStartCamera);
        //VectorToString(sideLineEndCamera);
        //printf("\n");

        Vector *sideLineStartSkewed = CreateImageScaledVertex(cam, image, sideLineStartCamera);
        Vector *sideLineEndSkewed = CreateImageScaledVertex(cam, image, sideLineEndCamera);

        //printf("Skewed vertices\n");
        //VectorToString(sideLineStartSkewed);
        //VectorToString(sideLineEndSkewed);
        //printf("\n");

        DrawLineBetweenTwoVertices(image, sideLineStartSkewed, sideLineEndSkewed);

        //FreeVector(currentIntervalOffset);
        FreeVector(sideLineStartCamera);
        FreeVector(sideLineEndCamera);
        FreeVector(sideLineStartSkewed);
        FreeVector(sideLineEndSkewed);
    }

    FreeVector(baseSideLine);

    return 0;

}