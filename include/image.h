#ifndef IMAGE_H
#define IMAGE_H

// Responsible for handling image creation, deletion, cleanup, editing and rendering

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct{
    uint8_t R;
    uint8_t G;
    uint8_t B;
} Color;

typedef struct{
    uint16_t width;
    uint16_t height;

    Color **pixels;

    //maybe temp variable
    double **zMap;
} Image;

Image* ImageCreate(uint16_t width, uint16_t height);

void ImageToString(Image* image);

int ImageFree(Image* image);

void ZMapToString(Image* image);

void ImageToPPM(Image* image, char *filename);

void AntiAliasing(Image* image, float centerVal, int AAWidth);

#endif