#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            RGBTRIPLE pixel = image[i][j];
            int avg = (round(pixel.rgbtRed + pixel.rgbtGreen + pixel.rgbtBlue) / 3);

            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;        
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Use variable as buffer to store pixel that is about to be swapped.
            RGBTRIPLE pixel = image[i][j];
            
            // Swap pixel with its pixel on the opposite side of the image.
            image[i][j] = image[i][abs(j - width)]; 
            image[i][abs(j - width)] = pixel; 
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE (*blurred)[width] = calloc(height, width * sizeof(RGBTRIPLE));    

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int e[9][2] = {
                {i - 1, j - 1},
                {i - 1, j},
                {i - 1, j + 1},
                {i, j - 1},
                {i, j},
                {i, j + 1},
                {i + 1, j - 1},
                {i + 1, j},
                {i + 1, j + 1}
            };

            int length = (sizeof(e) / 2)/sizeof(int);

            int avgRed = 0;
            int avgGreen = 0;
            int avgBlue = 0;

            for (int k = 0; k < length; k++) {
                int x = e[k][1];
                int y = e[k][0];

                if ((x >= 0 && x <= width) && (y >= 0 && y <= height)) {
                    avgRed += image[y][x].rgbtRed;
                    avgGreen += image[y][x].rgbtGreen;
                    avgBlue += image[y][x].rgbtBlue;
                }
            }

            RGBTRIPLE pixel;
            pixel.rgbtRed = round(avgRed / length);
            pixel.rgbtGreen = round(avgGreen / length);
            pixel.rgbtBlue = round(avgBlue / length);

            blurred[i][j] = pixel;
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j] = blurred[i][j];
        }
    }

    free(blurred);

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int (*edges)[width] = calloc(height, width * sizeof(int));

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int e[9][2] = {
                {i - 1, j - 1},
                {i - 1, j},
                {i - 1, j + 1},
                {i, j - 1},
                {i, j},
                {i, j + 1},
                {i + 1, j - 1},
                {i + 1, j},
                {i + 1, j + 1},
            };

            int gx[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
            int gy[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

            int length = (sizeof(e) / 2)/sizeof(int);

            int sumRedX = 0;
            int sumGreenX = 0;
            int sumBlueX = 0;
            
            int sumRedY = 0;
            int sumGreenY = 0;
            int sumBlueY = 0;

            if (i <= 1 || i == height - 1 || j <= 1 || j == width - 1) {
                printf("%s\n", "o.o");
            } else {
                for (int k = 0; k < length; k++) {
                    int x = e[k][1];
                    int y = e[k][0];

                    if (x > 0 && x < width && y > 0 && y < height) {
                        sumRedX += image[y][x].rgbtRed * gx[k];
                        sumGreenX += image[y][x].rgbtGreen * gx[k];
                        sumBlueX += image[y][x].rgbtBlue * gx[k];

                        sumRedY += image[y][x].rgbtRed * gy[k];
                        sumGreenY += image[y][x].rgbtGreen * gy[k];
                        sumBlueY += image[y][x].rgbtBlue * gy[k];
                    } 
                }
            }

            int fRed = round(sqrt((sumRedX * sumRedX) + (sumRedY * sumRedY)));
            int fGreen = round(sqrt((sumGreenX * sumGreenX) + (sumGreenY * sumGreenY)));
            int fBlue = round(sqrt((sumBlueX * sumBlueX) + (sumBlueY * sumBlueY)));

            int avg = round((fRed + fGreen + fBlue) / 3);

            edges[i][j] = avg;
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j].rgbtRed = edges[i][j];
            image[i][j].rgbtGreen = edges[i][j];
            image[i][j].rgbtBlue = edges[i][j];
        }
    }

    free(edges);

    return;
}
