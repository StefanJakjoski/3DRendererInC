#include "../include/image.h"

#define AA_THRESHOLD 80

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

Color GetPixelAAValue(Image* image, int y, int x, float centerVal, int AAWidth){
    float exteriorVal = 1.0f - centerVal;

    int start = -1*(AAWidth-1)/2;
    int end = AAWidth/2;

    Color initialPixel = image->pixels[y][x];

    int pixelAmount = 0;
    int maxDiff = 0;
    for(int i = start; i <= end; i++){
        for(int j = start; j <= end; j++){
            int yPrime = y + i;
            int xPrime = x + j;

            bool withinX = 0 <= xPrime && xPrime < image->width;
            bool withinY = 0 <= yPrime && yPrime < image->height;

            if(!withinX || !withinY)
                continue;

            Color currentPixel = image->pixels[yPrime][xPrime];
            pixelAmount++;
            int diff = sqrt(pow(initialPixel.R - currentPixel.R, 2) +
                        pow(initialPixel.G - currentPixel.G, 2) +
                        pow(initialPixel.B - currentPixel.B, 2));

            maxDiff = diff*(diff>maxDiff) + maxDiff*(diff<=maxDiff);
        }
    }

    if(maxDiff < AA_THRESHOLD)
        return initialPixel;

    //printf("Coordinates: %d %d: %d\n", y, x, pixelAmount);

    
    //Color cFinal = (Color) {0, 0, 0};
    exteriorVal /= (float) (pixelAmount-1);

    //Color initialPixel = image->pixels[y][x];
    //printf("Initial RGB: %d %d %d\n", start.R, start.G, start.B);

    float R = 0.0f;
    float G = 0.0f;
    float B = 0.0f;

    for(int i = start; i <= end; i++){
        for(int j = start; j <= end; j++){
            int yPrime = y + i;
            int xPrime = x + j;

            bool withinX = 0 <= xPrime && xPrime < image->width;
            bool withinY = 0 <= yPrime && yPrime < image->height;

            if(!withinX || !withinY)
                continue;

            Color initial = image->pixels[yPrime][xPrime];

            if(xPrime == x && yPrime == y){
                R = (float) R + (float) initial.R * centerVal;
                G = (float) G + (float) initial.G * centerVal;
                B = (float) B + (float) initial.B * centerVal;
                //cFinal = (Color) {R, G, B};
            }else{
                R = (float) R + (float) initial.R * exteriorVal;
                G = (float) G + (float) initial.G * exteriorVal;
                B = (float) B + (float) initial.B * exteriorVal;
                //cFinal = (Color) {R, G, B};
            }
        }
    }

    Color cFinal = (Color) {(int) R, (int) G, (int) B};


    //DELETE LATER
    //cFinal = (Color) {(int) 255, (int) 255, (int) 255};
    return cFinal;
}

void AntiAliasing(Image* image, float centerVal, int AAWidth){
    printf("Image: Applying Anti-Aliasing with %.2f central value bias...\n", centerVal);
    uint16_t w = image->width;
    uint16_t h = image->height;

    Color **newPixels = (Color **) malloc(h * sizeof(Color *));
    for(int i = 0; i < h; i++){
        newPixels[i] = (Color *) malloc(w * sizeof(Color));

        for(int j = 0; j < w; j++){
            Color AAPixel = GetPixelAAValue(image, i, j, centerVal, AAWidth);
            //printf("%d %d\n", i, j);
            newPixels[i][j] = AAPixel;
        }
    }

    //printf("check\n");
    for(int i = 0; i < h; i++){
        free(image->pixels[i]);
    }
    free(image->pixels);

    image->pixels = newPixels;
}