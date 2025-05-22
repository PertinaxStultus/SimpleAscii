#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// ASCII karatärer för att representera olika ljusstyrkor
const char* ASCII_CHARS = " .:-=+*#%@";

#define CHAR_COUNT (sizeof(ASCII_CHARS) - 1)

void convertToAscii(const char* inputFile, const char* outputFile, int newWidth) {
    FILE* image = fopen(inputFile, "r");
    FILE* output = fopen(outputFile, "w");
    
    if (!image || !output) {
        perror("Failed to open file");
        exit(1);
    }
    
    // läser PGM-header
    int width, height, maxVal;
    fscanf(image, "P2\n%d %d\n%d\n", &width, &height, &maxVal);
    
    // beräknar ny höjd baserat på bredd
    float aspect = (float)height / width;
    int newHeight = (int)(aspect * newWidth * 0.55); // 0.55 adjusts for character aspect
    
    // läser bild data
    unsigned char* pixels = malloc(width * height);
    for (int i = 0; i < width * height; i++) {
        int val;
        fscanf(image, "%d", &val);
        pixels[i] = (unsigned char)val;
    }
    
    // genererar ASCII art
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            // Map koordinarter från den nya bilden till den ursprungliga
            int origX = x * width / newWidth;
            int origY = y * height / newHeight;
            
            // hämtar plixens ljusstyrka
            int brightness = pixels[origY * width + origX];
            
            // Map ljusstyrka till ASCII karaktär
            int charIndex = (brightness * (CHAR_COUNT - 1)) / maxVal;
            fputc(ASCII_CHARS[charIndex], output);
        }
        fputc('\n', output);
    }
    
    free(pixels);
    fclose(image);
    fclose(output);
}

int main() {
    convertToAscii("output.pgm", "printed.txt", 100);
    printf("DONE!\n");
    return 0;
}

/*****************************************************************************
                                HUR MAN GÖR
 Man behöver ladda ner imagemagick
 på linux så kör kommandot sudo apt-get install imagemagick
 eller
 Sudo pacman -S imagemagick (på arch)
 på Mac så kör kommandot brew install imagemagick 

 sen med imagemagick så kör kommandot:
 convert <orginalbild> -colorspace gray -compress none output.pgm

 Annars bara va säker att ha bilden i samma mapp som koden och kör programmet
******************************************************************************/