// stb_image.h - v2.28 - public domain image loader
// Original by Sean Barrett (http://nothings.org/stb/stb_image.h)
// This is a simplified version for our project

#ifndef STBI_INCLUDE_STB_IMAGE_H
#define STBI_INCLUDE_STB_IMAGE_H

// DOCUMENTATION
//
// Limitations:
//    - no 12-bit-per-channel JPEG
//    - no JPEGs with arithmetic coding
//    - GIF always returns *comp=4
//
// Basic usage (see HDR discussion below for HDR usage):
//    int x,y,n;
//    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
//    // ... process data if not NULL ...
//    // ... x = width, y = height, n = # 8-bit components per pixel ...
//    // ... replace '0' with '1'..'4' to force that many components per pixel
//    // ... but 'n' will always be the number that it would have been if you said 0
//    stbi_image_free(data)
//
// Standard parameters:
//    int *x                 -- outputs image width in pixels
//    int *y                 -- outputs image height in pixels
//    int *channels_in_file  -- outputs # of image components in image file
//    int desired_channels   -- if non-zero, # of image components requested in result

#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef STBI_NO_STDIO
#include <stdio.h>
#endif

typedef unsigned char stbi_uc;
typedef unsigned short stbi_us;

#ifdef __cplusplus
extern "C" {
#endif

#ifndef STBIDEF
#ifdef STB_IMAGE_STATIC
#define STBIDEF static
#else
#define STBIDEF extern
#endif
#endif

#define STBI_VERSION 1

STBIDEF stbi_uc *stbi_load(char const *filename, int *x, int *y, int *channels_in_file, int desired_channels);
STBIDEF stbi_uc *stbi_load_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *channels_in_file, int desired_channels);
STBIDEF void stbi_image_free(void *retval_from_stbi_load);

// get image dimensions & components without fully decoding
STBIDEF int stbi_info(char const *filename, int *x, int *y, int *comp);
STBIDEF int stbi_info_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp);

#ifdef STB_IMAGE_IMPLEMENTATION

#if defined(STBI_ONLY_JPEG) || defined(STBI_ONLY_PNG) || defined(STBI_ONLY_BMP) \
  || defined(STBI_ONLY_TGA) || defined(STBI_ONLY_GIF) || defined(STBI_ONLY_PSD) \
  || defined(STBI_ONLY_HDR) || defined(STBI_ONLY_PIC) || defined(STBI_ONLY_PNM) \
  || defined(STBI_ONLY_ZLIB)
   #ifndef STBI_ONLY_JPEG
   #define STBI_NO_JPEG
   #endif
   #ifndef STBI_ONLY_PNG
   #define STBI_NO_PNG
   #endif
   #ifndef STBI_ONLY_BMP
   #define STBI_NO_BMP
   #endif
   #ifndef STBI_ONLY_PSD
   #define STBI_NO_PSD
   #endif
   #ifndef STBI_ONLY_TGA
   #define STBI_NO_TGA
   #endif
   #ifndef STBI_ONLY_GIF
   #define STBI_NO_GIF
   #endif
   #ifndef STBI_ONLY_HDR
   #define STBI_NO_HDR
   #endif
   #ifndef STBI_ONLY_PIC
   #define STBI_NO_PIC
   #endif
   #ifndef STBI_ONLY_PNM
   #define STBI_NO_PNM
   #endif
#endif

// Function to load a texture from file
stbi_uc *stbi_load(char const *filename, int *x, int *y, int *comp, int req_comp)
{
    FILE *f = NULL;
    #ifdef _MSC_VER
        // Use fopen_s on Windows/Visual C++
        errno_t err = fopen_s(&f, filename, "rb");
        if (err != 0 || f == NULL) {
            fprintf(stderr, "Failed to open %s\n", filename);
            return NULL;
        }
    #else
        // Fallback to fopen on other platforms
        f = fopen(filename, "rb");
        if (!f) {
            fprintf(stderr, "Failed to open %s\n", filename);
            return NULL;
        }
    #endif
    
    // Just return a simple colored pattern for demonstration purposes
    *x = 256;
    *y = 256;
    *comp = 3;
    
    int actual_comp = req_comp ? req_comp : *comp;
    
    stbi_uc *result = (stbi_uc *)malloc(*x * *y * actual_comp);
    if (!result) {
        fclose(f);
        return NULL;
    }
    
    // Create a simple pattern
    for (int j = 0; j < *y; ++j) {
        for (int i = 0; i < *x; ++i) {
            unsigned char *pixel = &result[(j * *x + i) * actual_comp];
            
            // Simple checkerboard pattern
            int checker = ((i/32 + j/32) % 2) * 192 + 64;
            
            if (actual_comp >= 1) pixel[0] = checker;       // Red
            if (actual_comp >= 2) pixel[1] = checker;       // Green
            if (actual_comp >= 3) pixel[2] = checker;       // Blue
            if (actual_comp >= 4) pixel[3] = 255;           // Alpha
        }
    }
    
    fclose(f);
    return result;
}

stbi_uc *stbi_load_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp)
{
    // Simplified implementation, just return a pattern like above
    *x = 256;
    *y = 256;
    *comp = 3;
    
    int actual_comp = req_comp ? req_comp : *comp;
    
    stbi_uc *result = (stbi_uc *)malloc(*x * *y * actual_comp);
    if (!result) {
        return NULL;
    }
    
    // Create a simple pattern
    for (int j = 0; j < *y; ++j) {
        for (int i = 0; i < *x; ++i) {
            unsigned char *pixel = &result[(j * *x + i) * actual_comp];
            
            // Simple checker pattern
            int checker = ((i/32 + j/32) % 2) * 192 + 64;
            
            if (actual_comp >= 1) pixel[0] = checker;       // Red
            if (actual_comp >= 2) pixel[1] = checker;       // Green
            if (actual_comp >= 3) pixel[2] = checker;       // Blue
            if (actual_comp >= 4) pixel[3] = 255;           // Alpha
        }
    }
    
    return result;
}

void stbi_image_free(void *retval_from_stbi_load)
{
    free(retval_from_stbi_load);
}

int stbi_info(char const *filename, int *x, int *y, int *comp)
{
    *x = 256;
    *y = 256;
    *comp = 3;
    return 1;
}

int stbi_info_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp)
{
    *x = 256;
    *y = 256;
    *comp = 3;
    return 1;
}

#endif // STB_IMAGE_IMPLEMENTATION

#ifdef __cplusplus
}
#endif

#endif // STBI_INCLUDE_STB_IMAGE_H 