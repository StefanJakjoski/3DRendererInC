#include "../include/image.h"

Image* ImageCreate(uint16_t width, uint16_t height){
    if(width <= 0 || height <= 0){
        fprintf(stderr, "Improper image dimensions.\n");
        return NULL;
    }

    Image* res = (Image *) malloc(sizeof(Image));
    if(res == NULL){
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }
    res->width = width;
    res->height = height;
    res->pixels = (Color **) malloc(height * sizeof(Color *));
    res->zMap = (double **) malloc(height * sizeof(double *));
    for(int i = 0; i < height; i++){
        res->pixels[i] = (Color *) malloc(width * sizeof(Color));
        res->zMap[i] = (double *) calloc(width, sizeof(double));

        for(int j = 0; j < width; j++){
            res->pixels[i][j] = (Color) {0,0,0};
        }
    }

    return res;
}

int ImageFree(Image* image){
    if(image == NULL){
        fprintf(stderr, "Null image.\n");
        return 1;
    }

    uint16_t height = image->height;

    for(int i = 0; i < height; i++){
        free(image->pixels[i]);
        free(image->zMap[i]);
    }
    free(image->zMap);
    free(image);

    return 0;
}

void ImageToString(Image* image){
    if(image == NULL){
        fprintf(stderr, "Null image.\n");
        return;
    }

    uint16_t width = image->width;
    uint16_t height = image->height;

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            Color *pixel = &image->pixels[i][j];
            printf("(%3d,%3d,%3d) ", pixel->R, pixel->G, pixel->B);
        }
        printf("\n");
    }
    printf("\n");
}

void ZMapToString(Image* image){
    if(image == NULL){
        fprintf(stderr, "Null image.\n");
        return;
    }

    uint16_t width = image->width;
    uint16_t height = image->height;

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            printf("%2.2f", image->zMap[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void ImageToPPM(Image* image, char* filename){
    strcat(filename, ".ppm");
    printf("Exporting to %s...\n", filename);
    FILE* out = fopen(filename, "w");
    if(out == NULL){
        fprintf(stderr, "Error opening file.\n");
        return;
    }
    
    fprintf(out, "P3\n");
    fprintf(out, "%d %d\n255\n", image->width, image->height);

    uint16_t width = image->width;
    uint16_t height = image->height;

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            Color *pixel = &image->pixels[i][j];
            fprintf(out, "%d %d %d\n", pixel->R, pixel->G, pixel->B);
        }
    }

    fclose(out);
    printf("Export complete.\n");
}

/*
int main(){
    Image* test = ImageCreate(5,5);
    ImageToString(test);

    char name[50] = "penis";
    ImageToPPM(test, name);
    return ImageFree(test);
}
*/