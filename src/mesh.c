#include "../include/mesh.h"

Mesh* CreateMesh(int maxVertexCount, int maxTriangleCount){
    Mesh* res = (Mesh *) malloc(sizeof(Mesh));
    res->maxVertexCount = maxVertexCount;
    res->vertexCount = 0;
    res->vertices = (Vector **) malloc(maxVertexCount * sizeof(Vector *));

    res->maxTriangleCount = maxTriangleCount;
    res->triangleCount = 0;
    res->triangles = (Triangle **) malloc(maxTriangleCount * sizeof(Triangle *));
    for(int i = 0; i < maxTriangleCount; i++){
        res->triangles[i] = (Triangle *) malloc(sizeof(Triangle));
    }
    
    return res;
}

Vector CalculateTriangleNormal(Vector* a, Vector* b, Vector* c){
    Vector *e1 = SubtractVectors2(b, a);
    Vector *e2 = SubtractVectors2(c, a);

    Vector* cross;
    CrossProduct(&cross, (Vector *) e1, (Vector *) e2);
    Vector* res = UnitVector2(cross);
    FreeVector(e1); FreeVector(e2); FreeVector(cross);

    return *res;
}

int MeshAddVertex(Mesh* mesh, Vector* vertex){
    if(mesh->vertexCount >= mesh->maxVertexCount){
        fprintf(stderr, "Mesh: Vertex cap reached");
        return 1;
    }

    mesh->vertices[mesh->vertexCount++] = DuplicateVector2(vertex);
    return 0;
}

int MeshAddTriangle(Mesh* mesh, Vector** vertices){
    if(mesh->triangleCount >= mesh->maxTriangleCount){
        fprintf(stderr, "Mesh: Triangle cap reached");
        return 1;
    }

    Triangle *t = mesh->triangles[mesh->triangleCount++];
    t->vertex[0] = vertices[0];
    t->vertex[1] = vertices[1];
    t->vertex[2] = vertices[2];
    t->normal = CalculateTriangleNormal(vertices[0], vertices[1], vertices[2]);

    return 0;
}

void FreeMesh(Mesh* mesh){
    if(mesh == NULL){
        return;
    }

    for(int i = 0; i < mesh->vertexCount; i++){
        FreeVector(mesh->vertices[i]);
    }
}

Vector* FindMeshCenter(Mesh* mesh){
    if(mesh == NULL){
        fprintf(stderr, "Mesh: Null mesh pointer\n");
        return NULL;
    }

    int vNum = mesh->vertexCount;
    Vector* avg = CreateVector2(3); 
    for(int i = 0; i < vNum; i++){
        SumVectors(avg, mesh->vertices[i]);
    }

    MultiplyVector(avg, 1.0/(double) vNum);
    return avg;
}

int MoveMesh(Mesh* mesh, Vector* pushVector){
    if(mesh == NULL){
        fprintf(stderr, "Mesh: Null mesh pointer\n");
        return 1;
    }

    int vCount = mesh->vertexCount;
    for(int i = 0; i < vCount; i++){
        Vector* v = mesh->vertices[i];
        if(!SumVectors(v, pushVector))
            return 2;

        //VectorToString(v);
    }

    for(int i = 0; i < mesh->triangleCount; i++){
        Triangle *t = mesh->triangles[i];
        //VectorToString(t->vertex[0]);
        //VectorToString(t->vertex[1]);
        //VectorToString(t->vertex[2]);
        //printf("\n");
    }

    return 0;
}

int ScaleMesh(Mesh* mesh, double scaleFactor){
    Vector* center = FindMeshCenter(mesh);
    int vCount = mesh->vertexCount;

    for(int i = 0; i < vCount; i++){
        Vector* v = mesh->vertices[i];
        if(!SubtractVectors(v, center))
            return 2;

        if(!MultiplyVector(v, scaleFactor))
            return 3;

        if(!SumVectors(v, center))
            return 2;

        //VectorToString(v);
    }

    return 0;
}

int RotateMesh(Mesh* mesh, Matrix* Q){
    if(mesh == NULL || Q == NULL){
        fprintf(stderr, "Mesh: Null pointer error\n");
        return 1;
    }

    Vector* avg = FindMeshCenter(mesh);
    //printf("Mesh Center: \n");
    //VectorToString(avg);

    for(int i = 0; i < mesh->vertexCount; i++){
        //printf("Transforming vertex:\n");
        //VectorToString(mesh->vertices[i]);
        SubtractVectors(mesh->vertices[i], avg);
        *mesh->vertices[i] = *MultiplyMatrixVector2(Q, mesh->vertices[i]);
        SumVectors(mesh->vertices[i], avg);

        //printf("To new vertex:\n");
        //VectorToString(mesh->vertices[i]);
        //printf("Normal %p\n", mesh->vertices[i]);
        //printf("\n");
        //VectorToString(vertices[i]);
    }

    for(int i = 0; i < mesh->triangleCount; i++){
        Triangle *t = mesh->triangles[i];
        //printf("Transforming triangle normal with vertices:\n");
        //VectorToString(t->vertex[0]);
        //VectorToString(t->vertex[1]);
        //VectorToString(t->vertex[2]);
        //printf("%p, %p, %p\n", t->vertex[0], t->vertex[1], t->vertex[2]);
        t->normal = CalculateTriangleNormal(t->vertex[0], t->vertex[1], t->vertex[2]);
        
        //t->normal = newNormal;
        //printf("New normal:\n");
        //VectorToString(&newNormal);
    }

    return 0;
}

int RotateMeshUsingAngles(Mesh* mesh, double xTheta, double yTheta, double zTheta){
    if(mesh == NULL){
        fprintf(stderr, "Mesh: Null pointer error\n");
        return 1;
    }

    if(xTheta != 0.0){
        Matrix *xRotation = CreateIdentityMatrix2(3);
        double sinX = sin((PI/180.0) * xTheta); 
        double cosX = cos((PI/180.0) * xTheta);
        mSet(xRotation, 1, 1, cosX); mSet(xRotation, 1, 2, -1.0*sinX);
        mSet(xRotation, 2, 1, sinX); mSet(xRotation, 2, 2, cosX);

        RotateMesh(mesh, xRotation);
        FreeMatrix(xRotation);
    }

    if(yTheta != 0.0){
        Matrix *yRotation = CreateIdentityMatrix2(3);
        double sinY = sin((PI/180.0) * yTheta); 
        double cosY = cos((PI/180.0) * yTheta);
        mSet(yRotation, 0, 0, cosY); mSet(yRotation, 0, 2, sinY);
        mSet(yRotation, 2, 0, -1.0*sinY); mSet(yRotation, 2, 2, cosY);

        RotateMesh(mesh, yRotation);
        FreeMatrix(yRotation);
    }

    if(zTheta != 0.0){
        Matrix *zRotation = CreateIdentityMatrix2(3);
        double sinZ = sin((PI/180.0) * zTheta); 
        double cosZ = cos((PI/180.0) * zTheta);
        mSet(zRotation, 0, 0, cosZ); mSet(zRotation, 0, 1, -1.0*sinZ);
        mSet(zRotation, 1, 0, sinZ); mSet(zRotation, 1, 1, cosZ);

        RotateMesh(mesh, zRotation);
        FreeMatrix(zRotation);
    }
}