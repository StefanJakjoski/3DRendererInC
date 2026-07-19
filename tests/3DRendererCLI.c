#include "../include/objReader.h"
#include "../include/rasterizer.h"

#include <time.h>
#include <string.h>

void helpCommands(){

        //help
        printf("___CLI COMMANDS___\n");
        printf("01. parse <object name/relative location - parses .obj file\n");
        printf("02. push <x, y, z> - pushes mesh according to specified vector\n");
        printf("03. scale <scale factor> - scales mesh by scale factor\n");
        printf("04. rotate <x, y, z> - rotates mesh by specified degrees at the given axes\n");
        printf("05. light <x, y, z> - specifies source of light (provide vector direction of light, not physical source)\n");
        printf("06. image <width, height> - specifies the output image's width and height\n");
        printf("07. color <r, g, b> - specifies the default color for monochrome rendering\n");
        printf("08. render <type, name/location> - specifies the render type ('monochrome' or 'random') and the exported image's name (without file extension like .ppm)\n");
        printf("09. camera <x, y, z> - specify coordinates of camera\n");
        printf("10. camera_rotate <x, y, z> - rotate the camera by specified values on all axes\n");
        printf("11. exit - safely exits CLI and shuts down process\n");

        printf("\nCamera centered at [500,500] by default.\n");
}

int main(){
    Mesh* mesh = NULL;
    Image* image = NULL;
    int defaultWidth = 1000;
    int defaultHeight = 1000;

    Vector* light = CreateVector2(3);
    vSet(light, 0, 1.0); vSet(light, 1, -1.0); vSet(light, 2, 0.0);
    //MultiplyVector(light, 1.0);
    DefineLightSource(light);

    Color defaultColor = (Color) {0,20,200};

    Camera *camera = InitializeDefaultCamera();

    helpCommands();

    while(1){
        char buffer[128];
        printf("\n> ");
        fgets(buffer, sizeof(buffer), stdin);

        char command[16];
        sscanf(buffer, "%s", command);

        if(strcmp(command, "parse") == 0){
            char object[32] = "\0";
            sscanf(buffer, "%s %s", command, object);
            if(strlen(object) == 0){
                fprintf(stderr, "Specify object to parse\n");
                continue;
            }

            printf("Parsing %s...\n", object);
            mesh = ParseObjFile(object);
            printf("Parsing complete\n");

            continue;
        }

        if(strcmp(command, "push") == 0){
            double vector[3] = {0.0, 0.0, 0.0};
            sscanf(buffer, "%s %lf %lf %lf", command, &vector[0], &vector[1], &vector[2]);

            printf("Pushing mesh by x=%2.3f, y=%2.3f, z=%2.3f\n", vector[0], vector[1], vector[2]);

            Vector* pushVector = CreateVector2(3);
            vSet(pushVector, 0, vector[0]); 
            vSet(pushVector, 1, vector[1]);
            vSet(pushVector, 2, vector[2]); 
            int moveCheck = MoveMesh(mesh, pushVector);

            Vector* avg = FindMeshCenter(mesh);
            printf("Mesh Center: \n");
            VectorToString(avg);

            continue;
        }

        if(strcmp(command, "scale") == 0){
            double scaleFactor = 1.0;
            sscanf(buffer, "%s %lf", command, &scaleFactor);

            printf("Scaling mesh by scale factor of %lf\n", scaleFactor);
            ScaleMesh(mesh, scaleFactor);

            continue;
        }

        if(strcmp(command, "rotate") == 0){
            double vector[3] = {0.0, 0.0, 0.0};
            sscanf(buffer, "%s %lf %lf %lf", command, &vector[0], &vector[1], &vector[2]);

            printf("Rotating mesh by %2.3f, %2.3f, %2.3f\n", vector[0], vector[1], vector[2]);

            RotateMeshUsingAngles(mesh, vector[0], vector[1], vector[2]);

            continue;
        }

        if(strcmp(command, "light") == 0){
            double vector[3] = {0.0, 0.0, 0.0};
            sscanf(buffer, "%s %lf %lf %lf", command, &vector[0], &vector[1], &vector[2]);

            printf("Switching light to x=%2.3f, y=%2.3f, z=%2.3f\n", vector[0], vector[1], vector[2]);
            vSet(light, 0, vector[0]); vSet(light, 1, vector[1]); vSet(light, 2, vector[2]);
            DefineLightSource(light);
            
            continue;
        }

        if(strcmp(command, "image") == 0){
            int width, height;
            sscanf(buffer, "%s %d %d", command, &width, &height);

            defaultWidth = width; defaultHeight = height;
            
            printf("Setting image dimensions to width=%d and height=%d\n", width, height);
            image = ImageCreate(width, height);

            continue;
        }

        if(strcmp(command, "color") == 0){
            int vector[3] = {0, 0, 0};
            sscanf(buffer, "%s %d %d %d", command, &vector[0], &vector[1], &vector[2]);

            defaultColor = (Color) {vector[0], vector[1], vector[2]};
            printf("Setting default color to (%d, %d, %d)\n",
                defaultColor.R, defaultColor.G, defaultColor.B);
                
            continue;
        }

        if(strcmp(command, "render") == 0){
            if(mesh == NULL){
                fprintf(stderr, "Parse .obj file first\n");
                continue;
            }

            char type[16] = "\0";
            char name[32] = "\0";
            sscanf(buffer, "%s %s %s", command, type, name);

            if(strlen(type) == 0){
                fprintf(stderr, "Specify type of render\n");
                continue;
            }

            if(strlen(name) == 0){
                fprintf(stderr, "Specify name of render\n");
                continue;
            }

            image = ImageCreate(defaultWidth, defaultHeight);

            if(strcmp(type, "monochrome") == 0){
                //int check = AddMonochromeColorToAllTriangles(image, mesh, defaultColor);
                ClipSkewAllTrianglesAndAddMonochrome(image, mesh, camera, defaultColor);
            }else if(strcmp(type, "random") == 0){
                //AddColorToAllTriangles(image, mesh);
                ClipSkewAllTrianglesAndAddColor(image, mesh, camera);
            }else{
                fprintf(stderr, "Specify type 'monochrome' or 'random'\n");
                continue;
            }

            ImageToPPM(image, name);

            continue;
        }

        if(strcmp(command, "camera") == 0){
            double vector[3] = {0, 0, 0};
            sscanf(buffer, "%s %lf %lf %lf", command, &vector[0], &vector[1], &vector[2]);
            vSet(camera->position, 0, vector[0]);
            vSet(camera->position, 1, vector[1]);
            vSet(camera->position, 2, vector[2]);
            
            printf("Set camera position to:\n");
            VectorToString(camera->position);
            continue;
        }

        if(strcmp(command, "camera_rotate") == 0){
            double vector[3] = {0.0, 0.0, 0.0};
            sscanf(buffer, "%s %lf %lf %lf", command, &vector[0], &vector[1], &vector[2]);
            
            RotateCameraUsingAngles(camera, vector[0], vector[1], vector[2]);
            printf("Camera rotated to:\n");
            printf("Right: "); VectorToString(camera->right);
            printf("Up: "); VectorToString(camera->up);
            printf("Forward: "); VectorToString(camera->forward);

            continue;
        }

        if(strcmp(command, "exit") == 0){
            printf("Safe shutdown initiated...\n");
            printf("Cleaning up mesh...\n");
            FreeMesh(mesh);
            return 0;
        }

        helpCommands();
    }
}