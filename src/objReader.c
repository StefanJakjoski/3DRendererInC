#include "../include/objReader.h"

void AddOneOrMoreTriangles(FILE* in, Mesh* mesh){
    //printf("check\n");
    char c;
    int starting=1;
    char* buffer = (char *) calloc(16, sizeof(char));
    int bufferIt = 0;
    int *vectorArr = (int *) malloc(8*sizeof(int));
    int vectorIt = 0;
    int faceIt = 0;     // 0-vertex, 1-texture, 2-normal
    while((c = fgetc(in)) != '\n'){
        //printf("%c", c);
        if(starting){
            while(c == ' '){
                faceIt = 0;
                c = fgetc(in);
                if(c == '\n')
                    return;
            }
                

            starting = 0;
        }

        if(c == ' ' && bufferIt != 0){
            buffer[bufferIt] = '\0';
            //printf("Parsing: %s\n",buffer);
            starting = 1;
            bufferIt = 0;
            faceIt = 0;
            char *endptr;
            int val = atoi(buffer);
            vectorArr[vectorIt++] = val;
            buffer = (char *) calloc(16, sizeof(char));
            continue;
        }

        if(c == '/'){
            faceIt++;
            continue;
        }

        if(faceIt == 0 && c != ' ')     //pa    rse only vertex it
            buffer[bufferIt++] = c;
        //printf("%c", c);

    }

    if(bufferIt != 0){
        buffer[bufferIt] = '\0';
        starting = 1;
        bufferIt = 0;
        char *endptr;
        int val = atoi(buffer);
        if(val != 0){
            //printf("\nFinal Parsing: %s, Endpoint: '%s'\n", buffer, endptr);
            vectorArr[vectorIt++] = val;
        }
            
        
        //buffer = (char *) calloc(16, sizeof(char));
    }

    //printf("Number of vertices in line: %d\n", vectorIt);
    //printf("Vertex ITs parsed:");
    //for(int i = 0; i < vectorIt; i++){
    //    printf(" %d", vectorArr[i]);
    //}
    //printf("\n");

    if(vectorIt == 3){
        //printf("Parsing 3 vectors into triangle\n");
        //VectorToString(mesh->vertices[vectorArr[0] -1]);
        //VectorToString(mesh->vertices[vectorArr[1] -1]);
        //VectorToString(mesh->vertices[vectorArr[2] -1]);
        Vector *t[3] = {
            mesh->vertices[vectorArr[0] -1],
            mesh->vertices[vectorArr[1] -1],
            mesh->vertices[vectorArr[2] -1]
        };

        MeshAddTriangle(mesh, t);
    }else if(vectorIt >= 4){
        //printf("Parsing 4 vectors into 2 triangles\n");
        //VectorToString(mesh->vertices[vectorArr[0] -1]);
        //VectorToString(mesh->vertices[vectorArr[1] -1]);
        //VectorToString(mesh->vertices[vectorArr[2] -1]);
        //printf("\n");
        //VectorToString(mesh->vertices[vectorArr[0] -1]);
        //VectorToString(mesh->vertices[vectorArr[2] -1]);
        //VectorToString(mesh->vertices[vectorArr[3] -1]);
        //printf("CHECK2\n");

        Vector *t1[3] = {
            mesh->vertices[vectorArr[0] -1],
            mesh->vertices[vectorArr[1] -1],
            mesh->vertices[vectorArr[2] -1]
        };

        Vector *t2[3] = {
            mesh->vertices[vectorArr[0] -1],
            mesh->vertices[vectorArr[2] -1],
            mesh->vertices[vectorArr[3] -1]
        };

        MeshAddTriangle(mesh, t1);
        MeshAddTriangle(mesh, t2);
    }
}

Vector* ParseVertex(FILE* in){
    char c;
    int starting=1;
    char* buffer = (char *) calloc(16, sizeof(char));
    int bufferIt = 0;
    int vectorIt = 0;
    Vector* vertex = CreateVector2(3);
    while((c = fgetc(in)) != '\n'){
        //printf("%c", c);
        if(starting){
            while(c == ' '){
                c = fgetc(in);
            }
                

            starting = 0;
        }

        if(c == ' ' && bufferIt != 0){
            buffer[bufferIt] = '\0';
            //printf("Parsing: %s\n",buffer);
            starting = 1;
            bufferIt = 0;
            char *endptr;
            double val = strtod(buffer, &endptr);
            vSet(vertex, vectorIt, val);
            vectorIt++;
            buffer = (char *) calloc(16, sizeof(char));
            continue;
        }
        buffer[bufferIt++] = c;
        //printf("%c", c);

    }

    if(bufferIt != 0){
        starting = 1;
        bufferIt = 0;
        char *endptr;
        double val = strtod(buffer, &endptr);
        vSet(vertex, vectorIt, val);
        vectorIt++;
    }

    return vertex;
}

Mesh* ParseObjFile(char filename[32]){
    FILE *in = fopen(filename, "r");

    Mesh* mesh = CreateMesh(200000, 400000);

    int newline = 1;
    int currentVertexCount = 0;
    char c;
    while((c = fgetc(in)) != EOF){
        //printf("%c", c);
        if(newline){
            if((int) c == (int) 'v'){
                if((c = fgetc(in)) != ' ')
                    continue; 

                Vector* v = ParseVertex(in);
                //printf("Parsed vertex:\n");
                //VectorToString(v);
                //printf("\n");
                MeshAddVertex(mesh, v);
                currentVertexCount++;

                newline = 1;
                continue;
            }else if(c == 'f'){
                AddOneOrMoreTriangles(in, mesh);
                newline = 1;
                continue;
            }else{
                while(c != '\n' && c != EOF)
                    c = fgetc(in);
            }

            continue;
        }

        //not newline
        if((int) c == (int) '\n')
            newline = 1;
    }


    return mesh;
}