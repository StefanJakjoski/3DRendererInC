#include "../include/rasterizer.h"

void main(){
    Mesh *mesh = CreateMesh(10,12);

    Vector *v1 = CreateVector2(3);
    Vector *v2 = CreateVector2(3);
    Vector *v3 = CreateVector2(3);
    Vector *v4 = CreateVector2(3);
    Vector *v5 = CreateVector2(3);
    Vector *v6 = CreateVector2(3);
    Vector *v7 = CreateVector2(3);
    Vector *v8 = CreateVector2(3);

    Vector *center = CreateVector2(3);
    vSet(center, 0, 450.0); vSet(center, 1, 450.0); vSet(center, 2, 450.0);

    vSet(v1,0,300.0); vSet(v1,1,300.0); vSet(v1,2,300.0); 
    vSet(v2,0,600.0); vSet(v2,1,300.0); vSet(v2,2,300.0);
    vSet(v3,0,300.0); vSet(v3,1,300.0); vSet(v3,2,600.0);
    vSet(v4,0,600.0); vSet(v4,1,300.0); vSet(v4,2,600.0);
    vSet(v5,0,300.0); vSet(v5,1,600.0); vSet(v5,2,300.0);
    vSet(v6,0,600.0); vSet(v6,1,600.0); vSet(v6,2,300.0);
    vSet(v7,0,300.0); vSet(v7,1,600.0); vSet(v7,2,600.0);
    vSet(v8,0,600.0); vSet(v8,1,600.0); vSet(v8,2,600.0);


    Vector *vertices[8] = {v1,v2,v3,v4,v5,v6,v7,v8};

    //rotational matrix creation
    
    Matrix *mBase = CreateMatrix2(3, 3);
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            mSet(mBase, i, j, rand());
        }
    }

    Matrix* Q; Matrix* R;
    QRDecomposition(&Q, &R, mBase);
    printf("Rotating using orthogonal rotation matrix:\n");
    //Matrix* QT = TransposeMatrix2(Q);
    //Matrix* QCheck = MultiplyMatrices2(Q,QT);
    //MatrixToString(QCheck);
    MatrixToString(Q);
    for(int i = 0; i < 8; i++){
        SubtractVectors(vertices[i], center);
        vertices[i] = MultiplyMatrixVector2(Q, vertices[i]);
        vertices[i] = SumVectors2(vertices[i], center);
        MeshAddVertex(mesh, vertices[i]);
        //VectorToString(vertices[i]);
    }
    
    
    //no rotations
    /*
    for(int i = 0; i < 8; i++){
        MeshAddVertex(mesh, vertices[i]);
    }
    */
    
    

    /*
    MeshAddVertex(mesh,v1);
    MeshAddVertex(mesh,v2);
    MeshAddVertex(mesh,v3);
    MeshAddVertex(mesh,v4);
    MeshAddVertex(mesh,v5);
    MeshAddVertex(mesh,v6);
    MeshAddVertex(mesh,v7);
    MeshAddVertex(mesh,v8);
    */

    //bottom
    Vector *t1[3] = {
        mesh->vertices[1],
        mesh->vertices[0],
        mesh->vertices[2]
    };
    Vector *t2[3] = {
        mesh->vertices[1],
        mesh->vertices[2],
        mesh->vertices[3]
    };

    //top
    Vector *t3[3] = {
        mesh->vertices[5],
        mesh->vertices[4],
        mesh->vertices[6]
    };
    Vector *t4[3] = {
        mesh->vertices[5],
        mesh->vertices[6],
        mesh->vertices[7]
    };

    //left
    Vector *t5[3] = {
        mesh->vertices[0],
        mesh->vertices[2],
        mesh->vertices[6]
    };
    Vector *t6[3] = {
        mesh->vertices[0],
        mesh->vertices[4],
        mesh->vertices[6]
    };

    //right
    Vector *t7[3] = {
        mesh->vertices[1],
        mesh->vertices[3],
        mesh->vertices[7]
    };
    Vector *t8[3] = {
        mesh->vertices[1],
        mesh->vertices[5],
        mesh->vertices[7]
    };

    //front
    Vector *t9[3] = {
        mesh->vertices[2],
        mesh->vertices[3],
        mesh->vertices[7]
    };
    Vector *t10[3] = {
        mesh->vertices[2],
        mesh->vertices[6],
        mesh->vertices[7]
    };

    //back
    Vector *t11[3] = {
        mesh->vertices[0],
        mesh->vertices[1],
        mesh->vertices[5]
    };
    Vector *t12[3] = {
        mesh->vertices[0],
        mesh->vertices[4],
        mesh->vertices[5]
    };

    

    MeshAddTriangle(mesh,t1);
    MeshAddTriangle(mesh,t2);
    MeshAddTriangle(mesh,t3);
    MeshAddTriangle(mesh,t4);
    MeshAddTriangle(mesh,t5);
    MeshAddTriangle(mesh,t6);
    MeshAddTriangle(mesh,t7);
    MeshAddTriangle(mesh,t8);
    MeshAddTriangle(mesh,t9);
    MeshAddTriangle(mesh,t10);
    MeshAddTriangle(mesh,t11);
    MeshAddTriangle(mesh,t12);

    Vector* pushVector = CreateVector2(3);
    vSet(pushVector, 0, 50.0); 
    vSet(pushVector, 1, 50.0);
    vSet(pushVector, 2, -500.0); 
    int moveCheck = MoveMesh(mesh, pushVector);
    moveCheck = ScaleMesh(mesh, 1.23);
    printf("Movement check: %d\n", moveCheck);

    Vector* avg = FindMeshCenter(mesh);
    printf("Mesh Center: \n");
    VectorToString(avg);

    

    /*
    for(int i = 0; i < mesh->triangleCount; i++){
        Triangle *t = mesh->triangles[i];
        Vector *tAvg = CreateVector2(3);
        for(int j = 0; j < 3; j++)
            SumVectors(tAvg, t->vertex[j]);

        MultiplyVector(tAvg, 1.0/3.0);
        SubtractVectors(tAvg, avg);
        Vector* calculatedNormal = UnitVector2(tAvg);
        printf("Calculated triangle normal: \n");
        VectorToString(calculatedNormal);
        Vector* tNormal = &mesh->triangles[i]->normal;
        double dirCheck = DotProduct2(calculatedNormal, tNormal);
        if(dirCheck < 0){
            MultiplyVector(&mesh->triangles[i]->normal, -1.0);
        }
    }
    */
    


    Image* image = ImageCreate(1000, 1000);
    int check = AddColorToAllTriangles(image, mesh);
    //check = DrawLinesBetweenVertices(image, mesh);
    //ZMapToString(image);

    char *filename = (char *) calloc(50, sizeof(char));
    strcat(filename, "cube_test");
    ImageToPPM(image, filename);
}