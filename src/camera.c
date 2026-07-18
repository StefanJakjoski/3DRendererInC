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

Vector* GetPercentageOfVertexPosition(Camera* c, Vector* v){
    Vector* skewedV = GetCameraViewOfVertex(c, v);

    double x = vGet(skewedV, 0);
    double y = vGet(skewedV, 1);
    double z = vGet(skewedV, 2);

    FreeVector(skewedV);

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


