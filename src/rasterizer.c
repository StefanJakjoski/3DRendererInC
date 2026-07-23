#include "../include/rasterizer.h"

#define VERTEX_THICKNESS 3
#define TRIANGLES_PER_TILE 150000

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


//TIME OPTIMIZED ALTERNATIVES
double Sign2(double x1, double y1, Vector* p2, Vector* p3){
    return (x1 - vGet(p3, 0)) * (vGet(p2, 1) - vGet(p3, 1))
        - (vGet(p2, 0) - vGet(p3, 0)) * (y1 - vGet(p3, 1));
}

bool PointInTriangle2(double x, double y, Triangle* t){
    double d1, d2, d3;
    bool hasNeg, hasPos;

    Vector* v1 = t->vertex[0];
    Vector* v2 = t->vertex[1];
    Vector* v3 = t->vertex[2];

    d1 = Sign2(x, y, v1, v2);
    d2 = Sign2(x, y, v2, v3);
    d3 = Sign2(x, y, v3, v1);

    hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(hasNeg && hasPos);
}

bool ClipTest(double p, double q, double *t0, double *t1){
    if(p == 0.0){
        return q >= 0.0;
    }

    double r = q / p;

    if(p < 0.0){
        // Potentially entering.
        if(r > *t1)
            return false;
        if(r > *t0)
            *t0 = r;
    }else{
        // Potentially leaving.
        if(r < *t0)
            return false;
        if(r < *t1)
            *t1 = r;
    }

    return true;
}

bool LiangBarskyClip(double *x0, double *y0, double *x1, double *y1,
    double xmin, double xmax, double ymin, double ymax){
    double dx = *x1 - *x0;
    double dy = *y1 - *y0;

    double t0 = 0.0;
    double t1 = 1.0;

    if(!ClipTest(-dx, *x0 - xmin, &t0, &t1))
        return false;

    if(!ClipTest(dx, xmax - *x0, &t0, &t1))
        return false;

    if(!ClipTest(-dy, *y0 - ymin, &t0, &t1))
        return false;

    if(!ClipTest(dy, ymax - *y0, &t0, &t1))
        return false;

    if(t1 < 1.0){
        *x1 = *x0 + t1 * dx;
        *y1 = *y0 + t1 * dy;
    }

    if(t0 > 0.0){
        *x0 = *x0 + t0 * dx;
        *y0 = *y0 + t0 * dy;
    }

    return true;
}



void DrawLineBetweenTwoVertices(Image* image, Vector* v1, Vector* v2){
    uint16_t w = image->width;
    uint16_t h = image->height;
    
    double x1 = vGet(v1, 0);
    double x2 = vGet(v2, 0);

    // v1 is the rightmost vector 
    if(x1 > x2){
        Vector* buff = v1;
        v1 = v2;
        v2 = buff;
        x1 = vGet(v1, 0);
        x2 = vGet(v2, 0);
    }

    //printf("Initial: %lf to %lf\n", x1, x2);

    double y1 = vGet(v1,1);
    double y2 = vGet(v2,1);

    double slope = CalculateSlopeBetweenTwoPoints(v1, v2);


    double yOffset = vGet(v1, 1) - vGet(v1, 0) * slope;
    //printf("Offset: %lf\n", yOffset);

    if(!LiangBarskyClip(
        &x1, &y1, &x2, &y2, 0.0, image->width - 1,
        0.0, image->height - 1)){
            return;     //line out of box
        }
    
    


    //int xStart = fmax(vGet(v1, 0), fmax( -yOffset / slope, 0.0));
    //int xEnd = fmin(vGet(v2, 0), fmin((y2 - yOffset) / slope, w));
    //printf("%d to %d\n", xStart, xEnd);
    int xStart = vGet(v1, 0);
    int xEnd = vGet(v2, 0);
    //printf("%d to %d\n", xStart, xEnd);
    
    //int yStart = LimitValue((int) vGet(v1, 1), h);
    //int yEnd = LimitValue((int) vGet(v2, 1), h);

    //vertical
    if(x1 == x2){
        for(int y = fmax(vGet(v1, 1), 0.0); y < fmin(vGet(v2, 1), h); y++){
            image->pixels[(h -1) - y][xStart] = (Color) {255,255,255};
        }

        return;
    }

    for(int x = xStart; x < xEnd; x++){
        int y = (int) (slope*(x) + yOffset);

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

            //Vector* point = CreateVector2(3);
            //vSet(point, 0, i); vSet(point, 1, j); //vSet(point, 2, z);

            //bool inTriangle = PointInTriangle(point, triangle);
            bool inTriangle = PointInTriangle2(i, j, triangle);
            if(inTriangle && (z < zMap[jT][i] || zMap[jT][i] == 0.0)){
                
                pixels[jT][i] = c;
                zMap[jT][i] = z;
            }

            //zAvg += z*inTriangle;
            //zCount += (int) inTriangle;
            //FreeVector(point);
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



// CLIPPED

int ClipSkewAllTrianglesAndAddColor(Image* image, Mesh* mesh, Camera* c){
    srand(time(NULL));

    printf("Beginning coloring...\n");
    for(int i = 0; i < mesh->triangleCount; i++){

        printf("\rProgress: %d/%d", (i+1), mesh->triangleCount);
        fflush(stdout);

        Color col = (Color) {(int) (rand() * 255), (int) (rand() * 255), (int) (rand() * 255)};
        if(lightSource != NULL)
            col = ShadeTriangleFromDiffuseLighting(mesh->triangles[i], col);
        
        Triangle *t = mesh->triangles[i];

        //pipeline
        //camera view -> split at near -> skew to image -> color
        Triangle *tCamera = GetCameraViewOfTriangle(c, t);

        Triangle **toRender = (Triangle **) malloc(2*sizeof(Triangle *));
        int trianglesToRender = ClipTriangleCloseToCamera(c, tCamera, toRender);
        
        for(int i = 0; i < trianglesToRender; i++){
            Triangle *tPrime = CreateSkewedTriangle(image, toRender[i], c);
            AddColorToSkewedTriangle(image, tPrime, col);
            FreeTriangle(tPrime);
        }

    }
    printf("\nColoring complete.\n");

    return 0;
}

int ClipSkewAllTrianglesAndAddMonochrome(Image* image, Mesh* mesh, Camera* cam, Color c){
    srand(time(NULL));

    printf("Beginning coloring...\n");
    for(int i = 0; i < mesh->triangleCount; i++){
        printf("\rProgress: %d/%d", (i+1), mesh->triangleCount);
        fflush(stdout);

        Color cFinal = (Color) {c.R, c.G, c.B};
        if(lightSource != NULL)
            cFinal = ShadeTriangleFromDiffuseLighting(mesh->triangles[i], cFinal);

        Triangle *t = mesh->triangles[i];

        //pipeline
        //camera view -> split at near -> skew to image -> color
        Triangle *tCamera = GetCameraViewOfTriangle(cam, t);

        Triangle **toRender = (Triangle **) malloc(2*sizeof(Triangle *));
        int trianglesToRender = ClipTriangleCloseToCamera(cam, tCamera, toRender);

        for(int i = 0; i < trianglesToRender; i++){
            Triangle *tPrime = CreateSkewedTriangle(image, toRender[i], cam);
            AddColorToSkewedTriangle(image, tPrime, cFinal);
            FreeTriangle(tPrime);
        }

    }
    printf("\nColoring complete.\n");

    return 0;
}

//PARALLELIZATION ATTEMPT

Tile* SplitScreenToTiles(Image* image, int tileNumY, int tileNumX){
    int width = image->width;
    int height = image->height;

    int tileWidth = width/tileNumX;
    int tileHeight = height/tileNumY;

    Tile* res = (Tile *) malloc(tileNumY*tileNumX * sizeof(Tile));
    for(int i = 0; i < tileNumY; i++){
        //res[i] = (Tile *) malloc(tileNumX * sizeof(Tile));
        for(int j = 0; j < tileNumX; j++){
            res[i * tileNumX + j].yStart = tileHeight*i;
            res[i * tileNumX + j].xStart = tileWidth*j;
            res[i * tileNumX + j].yEnd = tileHeight*(i+1);
            res[i * tileNumX + j].xEnd = tileWidth*(j+1);
            res[i * tileNumX + j].tileIndexY = i;
            res[i * tileNumX + j].tileIndexX = j;
            res[i * tileNumX + j].toRender = (Triangle **) malloc(TRIANGLES_PER_TILE * sizeof(Triangle *));
            res[i * tileNumX + j].colors = (Color *) malloc(TRIANGLES_PER_TILE * sizeof(Color));
            res[i * tileNumX + j].triangleCount = 0;
            res[i * tileNumX + j].maxTriangleCount = TRIANGLES_PER_TILE;   
        }
    }

    return res;
}

int AssignTriangleToTiles(Tile* tiles, int tileNumY, int tileNumX, Triangle* triangle, Color c){
    Vector *v1 = triangle->vertex[0];
    Vector* topLeft = DuplicateVector2(v1);
    Vector* bottomRight = DuplicateVector2(v1);

    int tWidth = tiles[0].xEnd - tiles[0].xStart;
    int tHeight = tiles[0].yEnd - tiles[0].yStart;

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

    int yBoundingStart = fmax(floor(vGet(topLeft, 1)/tHeight), 0);
    int xBoundingStart = fmax(floor(vGet(topLeft, 0)/tWidth), 0);
    int yBoundingEnd = fmin(ceil(vGet(bottomRight, 1)/tHeight), (tileNumY-1));
    int xBoundingEnd = fmin(ceil(vGet(bottomRight, 0)/tWidth), (tileNumX-1));

    for(int i = yBoundingStart; i <= yBoundingEnd; i++){
        for(int j = xBoundingStart; j <= xBoundingEnd; j++){
            tiles[i * tileNumX + j].colors[tiles[i * tileNumX + j].triangleCount] = c;
            tiles[i * tileNumX + j].toRender[tiles[i * tileNumX + j].triangleCount++] = triangle;
        }
    }

    FreeVector(topLeft);
    FreeVector(bottomRight);

    return 0;
}

int RenderTriangle(Image* image, Triangle* triangle, Color c, Tile* tile){
    //calculating plane from triangle normal
    // ax + by + cz + d = 0
    // z = - (ax + by + d) / c
    Vector* tNormal = &triangle->normal;
    Vector* v1 = triangle->vertex[0];

    double dNormal = -1.0 * DotProduct2(tNormal, v1);
    double aNormal = vGet(tNormal, 0);
    double bNormal = vGet(tNormal, 1);
    double cNormal = vGet(tNormal, 2);

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

    xStart = fmax(xStart, tile->xStart);
    xEnd = fmin(xEnd, tile->xEnd);
    yStart = fmax(yStart, tile->yStart);
    yEnd = fmin(yEnd, tile->yEnd);

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

            //Vector* point = CreateVector2(3);
            //vSet(point, 0, i); vSet(point, 1, j); //vSet(point, 2, z);

            //bool inTriangle = PointInTriangle(point, triangle);
            bool inTriangle = PointInTriangle2(i, j, triangle);
            if(inTriangle && (z < zMap[jT][i] || zMap[jT][i] == 0.0)){
                
                pixels[jT][i] = c;
                zMap[jT][i] = z;
            }

            //zAvg += z*inTriangle;
            //zCount += (int) inTriangle;
            //FreeVector(point);
        }
    }

    return 0;
}


int RenderTile(Image* image, Tile* t){
    int triangleCount = t->triangleCount;
    Triangle **triangles = t->toRender;
    Color *colors = t->colors;

    int check = 0;

    for(int i = 0; i < triangleCount; i++){
        Triangle *tr = triangles[i];
        if(check = RenderTriangle(image, tr, colors[i], t))
            return check;
    }

    return 0;
}

void *RenderWorker(void *arg){
    TileThreadData *data = arg;
    for(int i = data->threadID; i < data->tileCount; i += data->threadCount){
        RenderTile(data->image, &data->tiles[i]);
        //printf("Tile index %d %d has %d triangles\n", data->tiles[i].tileIndexY, data->tiles[i].tileIndexX, data->tiles[i].triangleCount);
    }

    return NULL;
}


//ClipSkewAllTrianglesAndAddMonochrome PARALLELIZATION
int MonochromeRasterParallel(Image* image, Mesh* mesh, Camera* cam, Color c){
    srand(time(NULL));
    printf("Beginning thread allocation...\n");

    //tile separation
    int tileNumY = 8;
    int tileNumX = 8; 
    Tile *tiles = SplitScreenToTiles(image, tileNumY, tileNumX);

    for(int i = 0; i < mesh->triangleCount; i++){
        printf("\rProgress: %d/%d", (i+1), mesh->triangleCount);
        fflush(stdout);

        Color cFinal = (Color) {c.R, c.G, c.B};
        if(lightSource != NULL)
            cFinal = ShadeTriangleFromDiffuseLighting(mesh->triangles[i], cFinal);

        Triangle *t = mesh->triangles[i];

        //pipeline
        //camera view -> split at near -> skew to image -> 
        //   -> tile separation -> color
        Triangle *tCamera = GetCameraViewOfTriangle(cam, t);

        //Triangle **toRender = (Triangle **) malloc(2*sizeof(Triangle *));
        Triangle *toRender[2];
        int trianglesToRender = ClipTriangleCloseToCamera(cam, tCamera, toRender);


        for(int i = 0; i < trianglesToRender; i++){
            Triangle *tPrime = CreateSkewedTriangle(image, toRender[i], cam);
            AssignTriangleToTiles(tiles, tileNumY, tileNumX, tPrime, cFinal);
            //AddColorToSkewedTriangle(image, tPrime, cFinal);
            //FreeTriangle(tPrime);
        }

    }

    printf("\nThread allocation complete\n");
    printf("Beginning coloring...\n");


    //THREAD CALL (COLORING)
    int threadCount = 8;
    pthread_t threads[8];
    TileThreadData data[8];

    for(int i = 0; i < threadCount; i++){
        data[i].image = image;
        data[i].tiles = tiles;
        //printf("\n\ncheck2\n\n");
        data[i].tileCount = tileNumX * tileNumY;

        data[i].threadID = i;
        data[i].threadCount = threadCount;

        int err = pthread_create(
            &threads[i],
            NULL,
            RenderWorker,
            &data[i]
        );

        if(err){
            fprintf(stderr, "Rasterizer: Worker creation error\n");

        }
    }

    //THREAD JOINING
    for(int i = 0; i < threadCount; i++)
        pthread_join(threads[i], NULL);

    printf("\nColoring complete.\n");

    return 0;
}

void TriangleGeometryPrerendering(Image* image, Mesh* mesh, Camera* cam, Tile *tList, int start, int end, Color c, int tileNumX, int tileNumY){
    for(int i = start; i < end; i++){
        //printf("\rProgress: %d/%d", (i+1), mesh->triangleCount);
        //fflush(stdout);

        Color cFinal = (Color) {c.R, c.G, c.B};
        if(lightSource != NULL)
            cFinal = ShadeTriangleFromDiffuseLighting(mesh->triangles[i], cFinal);

        Triangle *t = mesh->triangles[i];

        //pipeline
        //camera view -> split at near -> skew to image -> 
        //   -> tile separation -> color
        Triangle *tCamera = GetCameraViewOfTriangle(cam, t);

        //Triangle **toRender = (Triangle **) malloc(2*sizeof(Triangle *));
        Triangle *toRender[2];
        int trianglesToRender = ClipTriangleCloseToCamera(cam, tCamera, toRender);


        for(int i = 0; i < trianglesToRender; i++){
            Triangle *tPrime = CreateSkewedTriangle(image, toRender[i], cam);
            AssignTriangleToTiles(tList, tileNumY, tileNumX, tPrime, cFinal);
            //AddColorToSkewedTriangle(image, tPrime, cFinal);
            //FreeTriangle(tPrime);
        }

    }
}

void *GeometryWorker(void *arg){
    GeometryThreadData *data = arg;

    TriangleGeometryPrerendering(
        data->image,
        data->mesh,
        data->camera,
        data->tiles,
        data->startTriangle,
        data->endTriangle,
        data->color,
        data->tileNumX,
        data->tileNumY
    );

    return NULL;
}

int MonochromeRasterParallel2(Image* image, Mesh* mesh, Camera* cam, Color c){
    srand(time(NULL));
    printf("Beginning thread allocation...\n");

    //tile separation
    int tileNumY = 8;
    int tileNumX = 8; 
    int subtilingNum = 8;

    //Tile *tiles = SplitScreenToTiles(image, tileNumY, tileNumX);
    Tile *tiles[subtilingNum];
    pthread_t geoThreads[subtilingNum];
    GeometryThreadData geoData[subtilingNum];

    int triangleChunkSize = ceil((float) mesh->triangleCount/ (float) subtilingNum);
    for(int i = 0; i < subtilingNum; i++){
        tiles[i] = SplitScreenToTiles(image, tileNumY, tileNumX);
        geoData[i].image = image;
        geoData[i].mesh = mesh;
        geoData[i].camera = cam;
        geoData[i].tiles = tiles[i];

        int start = i*triangleChunkSize;
        int end = fmin((i+1)*triangleChunkSize, mesh->triangleCount);

        geoData[i].startTriangle = start;
        geoData[i].endTriangle = end;
        geoData[i].color = c;

        geoData[i].tileNumX = tileNumX;
        geoData[i].tileNumY = tileNumY;

        int err = pthread_create(
            &geoThreads[i], NULL,
            GeometryWorker, &geoData[i]
        );
        //TriangleGeometryPrerendering(image, mesh, cam, tiles[i], start, end);
    }

    for (int i = 0; i < subtilingNum; i++)
        pthread_join(geoThreads[i], NULL);

    printf("Thread allocation complete\n");

    //MERGING TILES
    printf("Merging tiles..\n");

    // for each set of tiles -> for each tile
    Tile *finalTiles = SplitScreenToTiles(image, tileNumY, tileNumX);
    for(int i = 0; i < subtilingNum; i++){
        Tile *currentTiles = tiles[i];

        //iterate over all tiles from currentTiles
        int tileNum = tileNumX * tileNumY;
        for(int j = 0; j < tileNum; j++){
            Tile *currentTile = &currentTiles[j];
            int tileIndexX = currentTile->tileIndexX;
            int tileIndexY = currentTile->tileIndexY;
            int tileIndex = tileNumX * tileIndexY + tileIndexX;
            Tile *finalTile = &finalTiles[tileIndex];

            for(int k = 0; k < currentTile->triangleCount; k++){
                finalTile->colors[k + finalTile->triangleCount] = currentTile->colors[k];
                finalTile->toRender[k + finalTile->triangleCount] = currentTile->toRender[k];
            }

            finalTile->triangleCount += currentTile->triangleCount;
        }
    }

    //COLORING
    printf("Beginning coloring...\n");


    //THREAD CALL (COLORING)
    int threadCount = 8;
    pthread_t threads[8];
    TileThreadData data[8];

    for(int i = 0; i < threadCount; i++){
        data[i].image = image;
        data[i].tiles = finalTiles;
        //printf("\n\ncheck2\n\n");
        data[i].tileCount = tileNumX * tileNumY;

        data[i].threadID = i;
        data[i].threadCount = threadCount;

        int err = pthread_create(
            &threads[i],
            NULL,
            RenderWorker,
            &data[i]
        );

        if(err){
            fprintf(stderr, "Rasterizer: Worker creation error\n");

        }
    }

    //THREAD JOINING
    for(int i = 0; i < threadCount; i++)
        pthread_join(threads[i], NULL);

    printf("Coloring complete.\n");

    return 0;
}

double TimedMonochromeRasterParallel(Image* image, Mesh* mesh, Camera* cam, Color c){
    clock_t start = clock();
    MonochromeRasterParallel(image, mesh, cam, c);
    clock_t end = clock();
    double elapsedMs = 1000.0 * (end - start) / CLOCKS_PER_SEC;

    return elapsedMs;
}

double TimedMonochromeRasterParallel2(Image* image, Mesh* mesh, Camera* cam, Color c){
    clock_t start = clock();
    MonochromeRasterParallel2(image, mesh, cam, c);
    clock_t end = clock();
    double elapsedMs = 1000.0 * (end - start) / CLOCKS_PER_SEC;

    return elapsedMs;
}