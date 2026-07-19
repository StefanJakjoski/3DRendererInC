#include "../include/camera.h"

double DegreesToRadians(double degrees){
    return degrees * M_PI / 180.0;
}

Camera* InitializeDefaultCamera(){
    Camera* res = (Camera *) malloc(sizeof(Camera));
    res->position = CreateVector2(3);

    res->right = CreateVector2(3);
    vSet(res->right, 0, 1.0);
    res->up = CreateVector2(3);
    vSet(res->up, 1, 1.0);
    res->forward = CreateVector2(3);
    vSet(res->forward, 2, 1.0);

    res->fov=DegreesToRadians(60.0);
    res->aspectRatio = 1.0;
    res->near=10.0;
    res->far=1000.0;

    return res;
}

Vector* GetCameraViewOfVertex(Camera* c, Vector* v){
    Vector* diff = SubtractVectors2(v, c->position);
    //VectorToString(diff);
    double x = DotProduct2(diff, c->right);
    double y = DotProduct2(diff, c->up);
    double z = DotProduct2(diff, c->forward);

    vSet(diff, 0, x); vSet(diff, 1, y); vSet(diff, 2, z);
    return diff;
}

Triangle* GetCameraViewOfTriangle(Camera* c, Triangle* t){
    Vector* v1Prime = GetCameraViewOfVertex(c, t->vertex[0]);
    Vector* v2Prime = GetCameraViewOfVertex(c, t->vertex[1]);
    Vector* v3Prime = GetCameraViewOfVertex(c, t->vertex[2]);
    Triangle *res = (Triangle *) malloc(sizeof(Triangle));


    res->vertex[0] = v1Prime;
    res->vertex[1] = v2Prime;
    res->vertex[2] = v3Prime;
    res->normal = CalculateTriangleNormal(v1Prime, v2Prime, v3Prime);

    return res;
}

//assumes always 2 in, always 1 out
Triangle** SplitTriangleInTwo(Camera* c, Vector** ins, Vector** outs){
    
    //double zPrime = DotProduct2(c->forward, c->position) + c->near;
    double zPrime = c->near;
    Vector* out = outs[0];
    //parameter:
    // P(t) = A + t(B-A)
    // t = (zPrime - zOut) / (zIn - zOut)

    double zOut = vGet(out, 2);

    //first vector
    Vector *in1 = ins[0];
    double zIn1 = vGet(in1, 2);
    double vt1 = (zPrime - zOut) / (zIn1 - zOut);
    Vector *P1 = SumVectors2(out, MultiplyVector2(SubtractVectors2(in1, out), vt1));

    //second vector
    Vector *in2 = ins[1];
    double zIn2 = vGet(in2, 2);
    double vt2 = (zPrime - zOut) / (zIn2 - zOut);
    Vector *P2 = SumVectors2(out, MultiplyVector2(SubtractVectors2(in2, out), vt2));

    Triangle *t1 = (Triangle *) malloc(sizeof(Triangle));
    t1->vertex[0] = P1; t1->vertex[1] = in1; t1->vertex[2] = in2;
    t1->normal = CalculateTriangleNormal(P1, in1, in2);

    Triangle *t2 = (Triangle *) malloc(sizeof(Triangle));
    t2->vertex[0] = P1; t2->vertex[1] = P2; t2->vertex[2] = in2;
    t2->normal = CalculateTriangleNormal(P1, P2, in2);

    Triangle **res = (Triangle **) malloc(2*sizeof(Triangle *));
    res[0] = t1; res[1] = t2;

    return res;
}

//assumes always 1 in, always 2 out
Triangle** ClipTriangleToNear(Camera* c, Vector** ins, Vector** outs){
    Vector *in = ins[0];
    Vector *out1 = outs[0];
    Vector *out2 = outs[1];

    //double zPrime = DotProduct2(c->forward, c->position) + c->near;
    double zPrime = c->near;
    double zIn = vGet(in, 2);

    //parameter:
    // P(t) = A + t(B-A)
    // t = (zPrime - zOut) / (zIn - zOut)

    //first vector
    double zOut1 = vGet(out1, 2);
    double vt1 = (zPrime - zOut1) / (zIn - zOut1);
    Vector *P1 = SumVectors2(out1, MultiplyVector2(SubtractVectors2(in, out1), vt1));

    //second vector
    double zOut2 = vGet(out2, 2);
    double vt2 = (zPrime - zOut2) / (zIn - zOut2);
    Vector *P2 = SumVectors2(out2, MultiplyVector2(SubtractVectors2(in, out2), vt2));

    Triangle *t1 = (Triangle *) malloc(sizeof(Triangle));
    t1->vertex[0] = P1; t1->vertex[1] = in; t1->vertex[2] = P2;
    t1->normal = CalculateTriangleNormal(P1, in, P2);

    Triangle **res = (Triangle **) malloc(1*sizeof(Triangle *));
    res[0] = t1; 

    return res;
}

int ClipTriangleCloseToCamera(Camera* c, Triangle* t, Triangle** res){
    bool v1In = vGet(t->vertex[0], 2) >= c->near;
    bool v2In = vGet(t->vertex[1], 2) >= c->near;
    bool v3In = vGet(t->vertex[2], 2) >= c->near;

    Vector** vIn = (Vector **) malloc(3 * sizeof(Vector *));
    Vector** vOut = (Vector **) malloc(3 * sizeof(Vector *));
    int inIt = 0;
    int outIt = 0;

    if(v1In)
        vIn[inIt++] = t->vertex[0];
    else
        vOut[outIt++] = t->vertex[0];
    
    if(v2In)
        vIn[inIt++] = t->vertex[1];
    else
        vOut[outIt++] = t->vertex[1];

    if(v3In)
        vIn[inIt++] = t->vertex[2];
    else
        vOut[outIt++] = t->vertex[2];

    int inCount = v1In+v2In+v3In;
    //printf("inCount: %d\n", inCount);
    if(inCount == 3){
        res[0] = t;
        free(vIn); free(vOut);
        return 1;
    }else if(inCount == 2){
        Triangle **resBuffer = SplitTriangleInTwo(c, vIn, vOut);
        res[0] = resBuffer[0]; res[1] = resBuffer[1];
        free(vIn); free(vOut); free(resBuffer);
        return 2;
    }else if(inCount == 1){
        Triangle **resBuffer = ClipTriangleToNear(c, vIn, vOut);
        res[0] = resBuffer[0];
        free(vIn); free(vOut); free(resBuffer);
        return 1; 
    }

    free(vIn); free(vOut);
    return 0;
}

Vector* GetPercentageOfVertexPosition(Camera* c, Vector* skewedV){
    //Vector* skewedV = GetCameraViewOfVertex(c, v);

    double x = vGet(skewedV, 0);
    double y = vGet(skewedV, 1);
    double z = vGet(skewedV, 2);

    //FreeVector(skewedV);

    double angle = c->fov / 2.0;
    double tanAngle = tan(angle);
    double maxY = tanAngle * z;
    double maxX = maxY * c->aspectRatio;

    Vector *percentages = CreateVector2(3);

    vSet(percentages, 0, (x+maxX) / (2.0*maxX));
    vSet(percentages, 1, (y+maxY) / (2.0*maxY));
    vSet(percentages, 2, ((z-c->near) / (c->far-c->near)));

    return percentages;
}

Vector* CreateImageScaledVertex(Camera* c, Image* i, Vector* v){
    Vector* p = GetPercentageOfVertexPosition(c, v);
    double w = i->width;
    double h = i->height;

    Vector* res = CreateVector2(3);
    double x = vGet(p, 0) * w;
    double y = vGet(p, 1) * h;
    double z = vGet(p, 2);
    vSet(res, 0, x); vSet(res, 1, y); vSet(res, 2, z);

    FreeVector(p);
    return res;
}

Triangle* CreateSkewedTriangle(Image* image, Triangle* t, Camera* c){
    Vector* v1Prime = CreateImageScaledVertex(c, image, t->vertex[0]);
    Vector* v2Prime = CreateImageScaledVertex(c, image, t->vertex[1]);
    Vector* v3Prime = CreateImageScaledVertex(c, image, t->vertex[2]);
    Triangle *res = (Triangle *) malloc(sizeof(Triangle));


    res->vertex[0] = v1Prime;
    res->vertex[1] = v2Prime;
    res->vertex[2] = v3Prime;
    res->normal = CalculateTriangleNormal(v1Prime, v2Prime, v3Prime);

    return res;
}

int IsVertexInView(Vector* v, Camera* c){
    Vector* cameraV = GetCameraViewOfVertex(c, v);
    double x = vGet(cameraV, 0);
    double y = vGet(cameraV, 1);
    double z = vGet(cameraV, 2);

    FreeVector(cameraV);

    if(!(c->near <= z && z <= c->far)){
        fprintf(stderr, "%lf is not within near/far\n", z);
        return 0;
    }
        
    

    //tanAngle = opp / adj (y / z)
    //opp = tanAngle * adj
    /*
                    |    
                    |
                    |  y/x
                    |
    ________________|
             z      |
                    |
                    | y/x
                    |
                    |

    */

    double angle = c->fov / 2.0;
    double tanAngle = tan(angle);
    double maxY = tanAngle * z;
    double maxX = maxY * c->aspectRatio;

    if(fabs(y) > maxY){
        fprintf(stderr, "%lf is not within maxY (%lf)\n", fabs(y), maxY);
        return 0;
    }

    if(fabs(x) > maxX){
        fprintf(stderr, "%1.17lf is not within maxX (%1.17lf)\n", fabs(x), maxX);
        return 0;
    }

    return 1;
}

int RotateCamera(Camera* camera, Matrix* Q){
    if(camera == NULL || Q == NULL){
        fprintf(stderr, "Camera: Null pointer error\n");
        return 1;
    }

    *camera->up = *MultiplyMatrixVector2(Q, camera->up);
    *camera->forward = *MultiplyMatrixVector2(Q, camera->forward);
    *camera->right = *MultiplyMatrixVector2(Q, camera->right);

    return 0;
}

int RotateCameraUsingAngles(Camera* c, double xTheta, double yTheta, double zTheta){
    if(c == NULL){
        fprintf(stderr, "Camera: Null pointer error\n");
        return 1;
    }

    if(xTheta != 0.0){
        Matrix *xRotation = CreateIdentityMatrix2(3);
        double sinX = sin((PI/180.0) * xTheta); 
        double cosX = cos((PI/180.0) * xTheta);
        mSet(xRotation, 1, 1, cosX); mSet(xRotation, 1, 2, -1.0*sinX);
        mSet(xRotation, 2, 1, sinX); mSet(xRotation, 2, 2, cosX);

        RotateCamera(c, xRotation);
        FreeMatrix(xRotation);
    }

    if(yTheta != 0.0){
        Matrix *yRotation = CreateIdentityMatrix2(3);
        double sinY = sin((PI/180.0) * yTheta); 
        double cosY = cos((PI/180.0) * yTheta);
        mSet(yRotation, 0, 0, cosY); mSet(yRotation, 0, 2, sinY);
        mSet(yRotation, 2, 0, -1.0*sinY); mSet(yRotation, 2, 2, cosY);

        RotateCamera(c, yRotation);
        FreeMatrix(yRotation);
    }

    if(zTheta != 0.0){
        Matrix *zRotation = CreateIdentityMatrix2(3);
        double sinZ = sin((PI/180.0) * zTheta); 
        double cosZ = cos((PI/180.0) * zTheta);
        mSet(zRotation, 0, 0, cosZ); mSet(zRotation, 0, 1, -1.0*sinZ);
        mSet(zRotation, 1, 0, sinZ); mSet(zRotation, 1, 1, cosZ);

        RotateCamera(c, zRotation);
        FreeMatrix(zRotation);
    }
}