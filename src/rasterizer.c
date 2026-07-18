#include "../include/rasterizer.h"

#define VERTEX_THICKNESS 3

Vector* lightSource = NULL;

void DrawVertex(Image* image, Vector* vertex){
    uint16_t w = image->width;
    uint16_t h = image->height;

    Color** pixels = image->pixels;

    int start = -1*(VERTEX_THICKNESS-1)/2;
    int end = VERTEX_THICKNESS/2;

    double xPrime = vGet(vertex, 0);
    double yPrime = (h-1) - vGet(vertex, 1);

    for(int i = start; i <= end; i++){
        for(int j = start; j < end; j++){
            double y = yPrime+i;
            double x = xPrime+j;

            bool withinX = 0 <= x && x < w;
            bool withinY = 0 <= y && y < h;

            if(withinX && withinY){
                pixels[(int) y][(int) x] = (Color) {255, 255, 255};
            }
        }
    }    
}

double CalculateSlopeBetweenTwoPoints(Vector* v1, Vector* v2){
    if(v1 == NULL || v2 == NULL){
        fprintf(stderr, "Rasterizer: Invalid vectors\n");
        return 0.0;
    }

    return (vGet(v2,1) - vGet(v1, 1)) / (vGet(v2,0) - vGet(v1, 0)); 
}

int LimitValue(int val, int max){
    if(val < 0)
        return 0;

    if(val >= max)
        return max-1;

    return val;
}

double Sign(Vector* p1, Vector* p2, Vector* p3){
    return (vGet(p1, 0) - vGet(p3, 0)) * (vGet(p2, 1) - vGet(p3, 1))
        - (vGet(p2, 0) - vGet(p3, 0)) * (vGet(p1, 1) - vGet(p3, 1));
}

bool PointInTriangle(Vector* point, Triangle* t){
    double d1, d2, d3;
    bool hasNeg, hasPos;

    Vector* v1 = t->vertex[0];
    Vector* v2 = t->vertex[1];
    Vector* v3 = t->vertex[2];

    d1 = Sign(point, v1, v2);
    d2 = Sign(point, v2, v3);
    d3 = Sign(point, v3, v1);

    hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(hasNeg && hasPos);
}


void DrawLineBetweenTwoVertices(Image* image, Vector* v1, Vector* v2){
    double x1 = vGet(v1, 0);
    double x2 = vGet(v2, 0);

    // v1 is the rightmost vector 
    if(x1 > x2){
        Vector* buff = v1;
        v1 = v2;
        v2 = buff;
    }

    uint16_t w = image->width;
    uint16_t h = image->height;

    double slope = CalculateSlopeBetweenTwoPoints(v1, v2);

    int xStart = LimitValue((int) vGet(v1, 0), w);
    int xEnd = LimitValue((int) vGet(v2, 0), w);

    int yOffset = (int) vGet(v1, 1);


    for(int x = xStart; x < xEnd; x++){
        int y = (int) (slope*(x-xStart) + yOffset);

        bool withinX = 0 <= x && x < w;
        bool withinY = 0 <= y && y < h;

        if(!withinX || !withinY)
            continue;

        image->pixels[(h -1) - y][x] = (Color) {255,255,255};
    }
}

int DrawLinesBetweenVertices(Image* image, Mesh* mesh){
    uint16_t w = image->width;
    uint16_t h = image->height;

    Color** pixels = image->pixels;
    double** zMap = image->zMap;
    Triangle** triangles = mesh->triangles;

    int triangleCount = mesh->triangleCount;
    int vertexCount = mesh->vertexCount;

    for(int i = 0; i < vertexCount; i++){
        Vector* vertex = mesh->vertices[i];
        DrawVertex(image, vertex);
    }

    //Draw lines between vertices
    for(int i = 0; i < triangleCount; i++){
        Triangle* t = mesh->triangles[i];
        DrawLineBetweenTwoVertices(image, t->vertex[0], t->vertex[1]);
        DrawLineBetweenTwoVertices(image, t->vertex[0], t->vertex[2]);
        DrawLineBetweenTwoVertices(image, t->vertex[1], t->vertex[2]);
    }

    return 0;
}

bool PointOnTopOfZMap(Vector* point, double** zMap){
    int x = (int) vGet(point, 0);
    int y = (int) vGet(point, 1);
    double z = vGet(point, 2);
    if(z > zMap[y][x])
        return true;

    return false;
}


void AddColorToTriangle(Image* image, Triangle* triangle, Color c){
    Vector* v1 = triangle->vertex[0];
    Vector* v2 = triangle->vertex[1];
    Vector* v3 = triangle->vertex[2];

    //calculating plane from triangle normal
    // ax + by + cz + d = 0
    // z = - (ax + by + d) / c
    Vector* tNormal = &triangle->normal;
    double dNormal = -1.0 * DotProduct2(tNormal, v1);
    double aNormal = vGet(tNormal, 0);
    double bNormal = vGet(tNormal, 1);
    double cNormal = vGet(tNormal, 2);

    //TESTING! DELETE AFTER FINISHING!
    /*
    VectorToString(tNormal);
    printf("d = %f\n", dNormal);
    printf("Plane check: %f\n",
        aNormal*vGet(v1,0) +
        bNormal*vGet(v1,1) +
        cNormal*vGet(v1,2) +
        dNormal);

    double cx = (vGet(v1,0)+vGet(v2,0)+vGet(v3,0))/3.0;
    double cy = (vGet(v1,1)+vGet(v2,1)+vGet(v3,1))/3.0;
    double cz = (vGet(v1,2)+vGet(v2,2)+vGet(v3,2))/3.0;
    double planeZ = -(aNormal*cx + bNormal*cy + dNormal)/cNormal;
    printf("actual centroid z = %.10f\n", cz);
    printf("plane centroid z = %.10f\n", planeZ);
    */

    Vector* topLeft = DuplicateVector2(v1);
    Vector* bottomRight = DuplicateVector2(v1);

    for(int i = 1; i < 3; i++){
        Vector* v = triangle->vertex[i];
        double x = vGet(v, 0);
        double y = vGet(v, 1);

        //smallest y, highest position
        if(y < vGet(topLeft, 1))
            vSet(topLeft, 1, y);

        //smallest x, left-most position
        if(x < vGet(topLeft, 0))
            vSet(topLeft, 0, x);

        //largest y, lowest position
        if(y > vGet(bottomRight, 1))
            vSet(bottomRight, 1, y);

        //largest x, right-most position
        if(x > vGet(bottomRight, 0))
            vSet(bottomRight, 0, x);
    }

    int xStart = LimitValue((int) vGet(topLeft,0), image->width);
    int xEnd = LimitValue((int) vGet(bottomRight,0), image->width);
    int yEnd = LimitValue((int) vGet(bottomRight, 1), image->height);
    int yStart = LimitValue((int) vGet(topLeft, 1), image->height);

    //printf("Printing from (%d, %d) to (%d, %d)\n", xStart, yStart, xEnd, yEnd);
    Color **pixels = image->pixels;
    double **zMap = image->zMap;
    int h = image->height;

    //double zAvg = 0.0;
    //int zCount = 0;

    for(int i = xStart; i <= xEnd; i++){
        for(int j = yStart; j <= yEnd; j++){

            double z = -1.0*(aNormal*i + bNormal*j + dNormal) / cNormal;
            int jT = (h-1) -j;

            Vector* point = CreateVector2(3);
            vSet(point, 0, i); vSet(point, 1, j); //vSet(point, 2, z);

            bool inTriangle = PointInTriangle(point, triangle);
            if(inTriangle && (z > zMap[jT][i] || zMap[jT][i] == 0.0)){
                
                pixels[jT][i] = c;
                zMap[jT][i] = z;
            }

            //zAvg += z*inTriangle;
            //zCount += (int) inTriangle;
            FreeVector(point);
        }
    }

    //zAvg /= (double)zCount;
    //printf("Triangle z average: %.2f\n\n", zAvg);
}

Color ShadeTriangleFromDiffuseLighting(Triangle* t, Color c){
    if(t == NULL){
        fprintf(stderr, "Lighting: Null pointer error.\n");
        return (Color) {-1,-1,-1};
    }

    double ratio = 1.0 - (DotProduct2(&t->normal, lightSource)+1.0)/2.0;
    return (Color) {c.R*ratio, c.G*ratio, c.B*ratio};
}

int AddColorToAllTriangles(Image* image, Mesh* mesh){
    srand(time(NULL));

    printf("Beginning coloring...\n");
    for(int i = 0; i < mesh->triangleCount; i++){
        printf("\rProgress: %d/%d", (i+1), mesh->triangleCount);
        fflush(stdout);

        Color c = (Color) {(int) (rand() * 255), (int) (rand() * 255), (int) (rand() * 255)};
        if(lightSource != NULL)
            c = ShadeTriangleFromDiffuseLighting(mesh->triangles[i], c);
        
        AddColorToTriangle(image, mesh->triangles[i], c);
    }
    printf("\nColoring complete.\n");

    return 0;
}

int AddMonochromeColorToAllTriangles(Image* image, Mesh* mesh, Color c){
    srand(time(NULL));

    printf("Beginning coloring...\n");
    for(int i = 0; i < mesh->triangleCount; i++){
        printf("\rProgress: %d/%d", (i+1), mesh->triangleCount);
        fflush(stdout);

        Color cFinal = (Color) {c.R, c.G, c.B};
        if(lightSource != NULL)
            cFinal = ShadeTriangleFromDiffuseLighting(mesh->triangles[i], cFinal);
        
        //printf("%d %d %d\n", cFinal.R, cFinal.G, cFinal.B);
        AddColorToTriangle(image, mesh->triangles[i], cFinal);
    }
    printf("\nColoring complete.\n");

    return 0;
}

int DefineLightSource(Vector* source){
    if(source == NULL){
        fprintf(stderr, "Lighting: Null pointer error.\n");
        return 1;
    }

    lightSource = UnitVector2(source);
    return 0;
}



int AddColorToSkewedTriangle(Image* image, Triangle *triangle, Color c){
    Vector* v1 = triangle->vertex[0];
    Vector* v2 = triangle->vertex[1];
    Vector* v3 = triangle->vertex[2];

    //VectorToString(v1);
    //VectorToString(v2);
    //VectorToString(v3);

    //calculating plane from triangle normal
    // ax + by + cz + d = 0
    // z = - (ax + by + d) / c
    Vector* tNormal = &triangle->normal;
    double dNormal = -1.0 * DotProduct2(tNormal, v1);
    double aNormal = vGet(tNormal, 0);
    double bNormal = vGet(tNormal, 1);
    double cNormal = vGet(tNormal, 2);

    //TESTING! DELETE AFTER FINISHING!
    /*
    VectorToString(tNormal);
    printf("d = %f\n", dNormal);
    printf("Plane check: %f\n",
        aNormal*vGet(v1,0) +
        bNormal*vGet(v1,1) +
        cNormal*vGet(v1,2) +
        dNormal);

    double cx = (vGet(v1,0)+vGet(v2,0)+vGet(v3,0))/3.0;
    double cy = (vGet(v1,1)+vGet(v2,1)+vGet(v3,1))/3.0;
    double cz = (vGet(v1,2)+vGet(v2,2)+vGet(v3,2))/3.0;
    double planeZ = -(aNormal*cx + bNormal*cy + dNormal)/cNormal;
    printf("actual centroid z = %.10f\n", cz);
    printf("plane centroid z = %.10f\n", planeZ);
    */

    Vector* topLeft = DuplicateVector2(v1);
    Vector* bottomRight = DuplicateVector2(v1);

    for(int i = 1; i < 3; i++){
        Vector* v = triangle->vertex[i];
        double x = vGet(v, 0);
        double y = vGet(v, 1);

        //smallest y, highest position
        if(y < vGet(topLeft, 1))
            vSet(topLeft, 1, y);

        //smallest x, left-most position
        if(x < vGet(topLeft, 0))
            vSet(topLeft, 0, x);

        //largest y, lowest position
        if(y > vGet(bottomRight, 1))
            vSet(bottomRight, 1, y);

        //largest x, right-most position
        if(x > vGet(bottomRight, 0))
            vSet(bottomRight, 0, x);
    }

    int xStart = LimitValue((int) vGet(topLeft,0), image->width);
    int xEnd = LimitValue((int) vGet(bottomRight,0), image->width);
    int yEnd = LimitValue((int) vGet(bottomRight, 1), image->height);
    int yStart = LimitValue((int) vGet(topLeft, 1), image->height);

    //printf("Printing from (%d, %d) to (%d, %d)\n", xStart, yStart, xEnd, yEnd);
    Color **pixels = image->pixels;
    double **zMap = image->zMap;
    int h = image->height;

    //double zAvg = 0.0;
    //int zCount = 0;

    for(int i = xStart; i <= xEnd; i++){
        for(int j = yStart; j <= yEnd; j++){

            double z = -1.0*(aNormal*i + bNormal*j + dNormal) / cNormal;
            //printf("%5.3lf \n", z);
            if(z < 0.0 || z > 1.0)
                continue;
            
            int jT = (h-1) -j;

            Vector* point = CreateVector2(3);
            vSet(point, 0, i); vSet(point, 1, j); //vSet(point, 2, z);

            bool inTriangle = PointInTriangle(point, triangle);
            if(inTriangle && (z < zMap[jT][i] || zMap[jT][i] == 0.0)){
                
                pixels[jT][i] = c;
                zMap[jT][i] = z;
            }

            //zAvg += z*inTriangle;
            //zCount += (int) inTriangle;
            FreeVector(point);
        }
    }

    //zAvg /= (double)zCount;
    //printf("Triangle z average: %.2f\n\n", zAvg);

}

int SkewAllTrianglesAndAddColor(Image* image, Mesh* mesh, Camera* c){
    srand(time(NULL));

    printf("Beginning coloring...\n");
    for(int i = 0; i < mesh->triangleCount; i++){

        printf("\rProgress: %d/%d", (i+1), mesh->triangleCount);
        fflush(stdout);

        Color col = (Color) {(int) (rand() * 255), (int) (rand() * 255), (int) (rand() * 255)};
        if(lightSource != NULL)
            col = ShadeTriangleFromDiffuseLighting(mesh->triangles[i], col);
        
        Triangle *tPrime = CreateSkewedTriangle(image, mesh->triangles[i], c);
        
        AddColorToSkewedTriangle(image, tPrime, col);

        FreeTriangle(tPrime);
    }
    printf("\nColoring complete.\n");

    return 0;
}

int SkewAllTrianglesAndAddMonochrome(Image* image, Mesh* mesh, Camera* cam, Color c){
    srand(time(NULL));

    printf("Beginning coloring...\n");
    for(int i = 0; i < mesh->triangleCount; i++){
        printf("\rProgress: %d/%d", (i+1), mesh->triangleCount);
        fflush(stdout);

        Color cFinal = (Color) {c.R, c.G, c.B};
        if(lightSource != NULL)
            cFinal = ShadeTriangleFromDiffuseLighting(mesh->triangles[i], cFinal);
        
        //printf("%d %d %d\n", cFinal.R, cFinal.G, cFinal.B);
        Triangle *tPrime = CreateSkewedTriangle(image, mesh->triangles[i], cam);
        
        AddColorToSkewedTriangle(image, tPrime, cFinal);

        FreeTriangle(tPrime);
        //AddColorToTriangle(image, mesh->triangles[i], cFinal);
    }
    printf("\nColoring complete.\n");

    return 0;
}

