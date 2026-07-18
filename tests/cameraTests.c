#include "../include/camera.h"

Vector* NewVector(double a, double b, double c){
    Vector* res = CreateVector2(3);
    vSet(res, 0, a); vSet(res, 1, b); vSet(res, 2, c);

    return res;
}

void PrintPercentages(Camera *cam, Vector *v)
{
    Vector *p = GetPercentageOfVertexPosition(cam, v);

    printf("(%6.2f,%6.2f,%6.2f) -> (%7.4f,%7.4f,%7.4f)\n",
        vGet(v,0), vGet(v,1), vGet(v,2),
        vGet(p,0), vGet(p,1), vGet(p,2));

    FreeVector(p);
}

int main(){
    Camera* cam = InitializeDefaultCamera();

    cam->fov  = M_PI / 2.0;    // 90°
    cam->near = 1.0;
    cam->far  = 10.0;

    cam->aspectRatio = 1.0;

    printf("Front center:      %d (expected 1)\n",
        IsVertexInView(NewVector(0,0,5), cam));

    printf("Near plane:        %d (expected 1)\n",
        IsVertexInView(NewVector(0,0,1), cam));

    printf("Far plane:         %d (expected 1)\n",
        IsVertexInView(NewVector(0,0,10), cam));

    printf("Before near:       %d (expected 0)\n",
        IsVertexInView(NewVector(0,0,0.9), cam));

    printf("Past far:          %d (expected 0)\n",
        IsVertexInView(NewVector(0,0,11), cam));

    printf("Behind camera:     %d (expected 0)\n",
        IsVertexInView(NewVector(0,0,-5), cam));

    printf("Right edge:        %d (expected 1)\n",
        IsVertexInView(NewVector(5,0,5), cam));

    printf("Right outside:     %d (expected 0)\n",
        IsVertexInView(NewVector(5.01,0,5), cam));

    printf("Left edge:         %d (expected 1)\n",
        IsVertexInView(NewVector(-5,0,5), cam));

    printf("Left outside:      %d (expected 0)\n",
        IsVertexInView(NewVector(-5.01,0,5), cam));

    printf("Top edge:          %d (expected 1)\n",
        IsVertexInView(NewVector(0,5,5), cam));

    printf("Top outside:       %d (expected 0)\n",
        IsVertexInView(NewVector(0,5.01,5), cam));

    printf("Bottom edge:       %d (expected 1)\n",
        IsVertexInView(NewVector(0,-5,5), cam));

    printf("Bottom outside:    %d (expected 0)\n",
        IsVertexInView(NewVector(0,-5.01,5), cam));

    printf("Top-right corner:  %d (expected 1)\n",
        IsVertexInView(NewVector(5,5,5), cam));

    printf("Corner outside:    %d (expected 0)\n",
        IsVertexInView(NewVector(5.1,5.1,5), cam));

    printf("\n--- Percentage Tests ---\n");

    PrintPercentages(cam, NewVector(0,0,5));
    // Expected: (0.5, 0.5, ~0.4444)

    PrintPercentages(cam, NewVector(-5,0,5));
    // Expected: (0.0, 0.5, ~0.4444)

    PrintPercentages(cam, NewVector(5,0,5));
    // Expected: (1.0, 0.5, ~0.4444)

    PrintPercentages(cam, NewVector(0,-5,5));
    // Expected: (0.5, 0.0, ~0.4444)

    PrintPercentages(cam, NewVector(0,5,5));
    // Expected: (0.5, 1.0, ~0.4444)

    PrintPercentages(cam, NewVector(-5,-5,5));
    // Expected: (0.0, 0.0, ~0.4444)

    PrintPercentages(cam, NewVector(5,5,5));
    // Expected: (1.0, 1.0, ~0.4444)

    PrintPercentages(cam, NewVector(2.5,2.5,5));
    // Expected: (0.75, 0.75, ~0.4444)

    PrintPercentages(cam, NewVector(-2.5,-2.5,5));
    // Expected: (0.25, 0.25, ~0.4444)

    PrintPercentages(cam, NewVector(0,0,1));
    // Expected: (0.5, 0.5, 0.0)

    PrintPercentages(cam, NewVector(0,0,10));
    // Expected: (0.5, 0.5, 1.0)
}