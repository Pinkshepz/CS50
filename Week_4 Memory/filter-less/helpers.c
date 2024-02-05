#include "helpers.h"

#include <stdio.h>
#include <math.h>

float min(float num1, float num2);
void swap(int height, int width, RGBTRIPLE image[height][width], int i, int j, int jx);
void blur_pixel(int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE copy[height][width], int i, int j, int left, int up, int right, int down);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // calulate greyscale values
            BYTE grey = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            // set RGB values
            image[i][j].rgbtRed = grey;
            image[i][j].rgbtGreen = grey;
            image[i][j].rgbtBlue = grey;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // calulate greyscale values
            BYTE sepiaRed = round(min((0.393 * image[i][j].rgbtRed) + (0.769 * image[i][j].rgbtGreen) + (0.189 * image[i][j].rgbtBlue), 255.0));

            BYTE sepiaGreen = round(min((0.349 * image[i][j].rgbtRed) + (0.686 * image[i][j].rgbtGreen) + (0.168 * image[i][j].rgbtBlue), 255.0));

            BYTE sepiaBlue = round(min((0.272 * image[i][j].rgbtRed) + (0.534 * image[i][j].rgbtGreen) + (0.131 * image[i][j].rgbtBlue), 255.0));

            // set RGB values
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            swap(height, width, image, i, j, width - j - 1);
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy image
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j].rgbtRed = image[i][j].rgbtRed;
            copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
            copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
        }
    }

    // Upper-left of the image
    blur_pixel(height, width, image, copy, 0, 0, 0, 0, 1, 1);

    // Upper-middle of the copy
    for (int j = 1; j < width - 1; j++)
    {
        blur_pixel(height, width, image, copy, 0, j, 1, 0, 1, 1);
    }

    // Upper-right of the copy
    blur_pixel(height, width, image, copy, 0, width - 1, 1, 0, 0, 1);

    // All pixels of the copy except the first and last row
    for (int i = 1; i < height - 1; i++)
    {
        blur_pixel(height, width, image, copy, i, 0, 0, 1, 1, 1);

        for (int j = 1; j < width - 1; j++)
        {
            blur_pixel(height, width, image, copy, i, j, 1, 1, 1, 1);
        }

        blur_pixel(height, width, image, copy, i, width - 1, 1, 1, 0, 1);
    }

    // Lower-left of the copy
    blur_pixel(height, width, image, copy, height - 1, 0, 0, 1, 1, 0);

    // Lower-middle of the copy
    for (int j = 1; j < width - 1; j++)
    {
        blur_pixel(height, width, image, copy, height - 1, j, 1, 1, 1, 0);
    }

    // Lower-right of the copy
    blur_pixel(height, width, image, copy, height - 1, width - 1, 1, 1, 0, 0);
    return;
}

//**Custom functions**//
// Find minimum number
float min(float num1, float num2)
{
    if (num1 > num2)
    {
        return num2;
    }
    return num1;
}

// Swap swap and swap
void swap(int height, int width, RGBTRIPLE image[height][width], int i, int j, int jx)
{
    int tempRed = image[i][j].rgbtRed;
    int tempGreen = image[i][j].rgbtGreen;
    int tempBlue = image[i][j].rgbtBlue;

    image[i][j].rgbtRed = image[i][jx].rgbtRed;
    image[i][j].rgbtGreen = image[i][jx].rgbtGreen;
    image[i][j].rgbtBlue = image[i][jx].rgbtBlue;

    image[i][jx].rgbtRed = tempRed;
    image[i][jx].rgbtGreen = tempGreen;
    image[i][jx].rgbtBlue = tempBlue;
}

// Blur
void blur_pixel(int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE copy[height][width], int i, int j, int left, int up, int right, int down)
{
    // if (i < 1)
    // {
    //     printf("{%i, %i }, ", i, j);
    //     printf("(%i ", image[i][j].rgbtRed);
    //     printf("%i ", image[i][j].rgbtGreen);
    //     printf("%i) -> ", image[i][j].rgbtBlue);
    // }

    BYTE blurRed = round((copy[i][j].rgbtRed + (copy[i][j - 1].rgbtRed * left) + (copy[i - 1][j].rgbtRed * up) + (copy[i][j + 1].rgbtRed * right) + (copy[i + 1][j].rgbtRed * down) + (copy[i - 1][j - 1].rgbtRed * (left * up)) + (copy[i - 1][j + 1].rgbtRed * (right * up)) + (copy[i + 1][j + 1].rgbtRed * (right * down)) + (copy[i + 1][j - 1].rgbtRed * (left * down))) / (left + up + right + down + (left * up) + (right * up) + (right * down) + (left * down) + 1.0));

    BYTE blurGreen = round((copy[i][j].rgbtGreen + (copy[i][j - 1].rgbtGreen * left) + (copy[i - 1][j].rgbtGreen * up) + (copy[i][j + 1].rgbtGreen * right) + (copy[i + 1][j].rgbtGreen * down) + (copy[i - 1][j - 1].rgbtGreen * (left * up)) + (copy[i - 1][j + 1].rgbtGreen * (right * up)) + (copy[i + 1][j + 1].rgbtGreen * (right * down)) + (copy[i + 1][j - 1].rgbtGreen * (left * down))) / (left + up + right + down + (left * up) + (right * up) + (right * down) + (left * down) + 1.0));

    BYTE blurBlue = round((copy[i][j].rgbtBlue + (copy[i][j - 1].rgbtBlue * left) + (copy[i - 1][j].rgbtBlue * up) + (copy[i][j + 1].rgbtBlue * right) + (copy[i + 1][j].rgbtBlue * down) + (copy[i - 1][j - 1].rgbtBlue * (left * up)) + (copy[i - 1][j + 1].rgbtBlue * (right * up)) + (copy[i + 1][j + 1].rgbtBlue * (right * down)) + (copy[i + 1][j - 1].rgbtBlue * (left * down))) / (left + up + right + down + (left * up) + (right * up) + (right * down) + (left * down) + 1.0));

    image[i][j].rgbtRed = blurRed;
    image[i][j].rgbtGreen = blurGreen;
    image[i][j].rgbtBlue = blurBlue;

    // if (i < 1)
    // {
    //     printf("%i %i %i %i %i %i %i %i %i ", copy[i][j].rgbtRed, (copy[i][j - 1].rgbtRed * left), (copy[i - 1][j].rgbtRed * up), (copy[i][j + 1].rgbtRed * right), (copy[i + 1][j].rgbtRed * down), (copy[i - 1][j - 1].rgbtRed * (left * up)), (copy[i - 1][j + 1].rgbtRed * (right * up)), (copy[i + 1][j + 1].rgbtRed * (right * down)), (copy[i + 1][j - 1].rgbtRed * (left * down)));
    //     printf("(%i ", image[i][j].rgbtRed);
    //     printf("%i ", image[i][j].rgbtGreen);
    //     printf("%i)\n", image[i][j].rgbtBlue);
    // }

    return;
}