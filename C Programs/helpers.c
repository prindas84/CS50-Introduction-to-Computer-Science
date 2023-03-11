#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Find the average of the values for Red, Green and Blue, for each pixel.
            double avg = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            //Assign the average value calculated, as the value of Red, Green and Blue, for each pixel.
            //This will create a representative greyscale colour for each pixel.
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
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //If the pixel is in the first half of the row, switch it with the corosponding reverse pixel in the second half of the row.
            RGBTRIPLE swap;
            if (j <= ((width - 1) / 2.0))
            {
                swap = image[i][j];
                image[i][j] = image[i][((width - 1) - j)];
                image[i][((width - 1) - j)] = swap;
            }
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];

    //For every row in the image.
    for (int i = 0; i < height; i++)
    {
        //For every element of every row of the image
        for (int j = 0; j < width; j++)
        {
            int count = 0, totalRed = 0, totalGreen = 0, totalBlue = 0;

            //For every row in the 9 square grid around the current cell.
            for (int row = -1; row < 2; row++)
            {
                //For every element of every row in the 9 square grid around the current cell.
                for (int col = -1; col < 2; col++)
                {
                    int rowPos = i + row, colPos = j + col;
                    
                    //If the cell is an active cell which is inside the boundaries of the image, add the Red, Green and Blue values to the total count for each.
                    //Increment the count to indicate this cell was included in the total so it can be used to calculate the average at the end.
                    if ((rowPos >= 0) && (rowPos <= (height - 1)) && (colPos >= 0) && (colPos <= (width - 1)))
                    {
                        totalRed += image[rowPos][colPos].rgbtRed;
                        totalGreen += image[rowPos][colPos].rgbtGreen;
                        totalBlue += image[rowPos][colPos].rgbtBlue;
                        count++;
                    }
                }
            }

            //Calculate the average for Red, Green, Blue over the 9 pixle grid, and then set the current pixel value to these averages.
            copy[i][j].rgbtRed = round((float)totalRed / (float)count);
            copy[i][j].rgbtGreen = round((float)totalGreen / (float)count);
            copy[i][j].rgbtBlue = round((float)totalBlue / (float)count);
        }
    }

    //Overwrite the original image values with the new image values.
    for (int x = 0; x < height; x++)
    {
        for (int y = 0; y < width; y++)
        {
            image[x][y] = copy[x][y];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    int gxValue[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gyValue[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    //For each row in the image.
    for (int i = 0; i < height; i++)
    {
        //For each cell in each row of the image.
        for (int j = 0; j < width; j++)
        {
            int totalGxRed = 0, totalGxGreen = 0, totalGxBlue = 0, totalGyRed = 0, totalGyGreen = 0, totalGyBlue = 0,
                rowCount = 0, colCount = 0;
            
            //For each row in the 9 element grid around the current cell.
            for (int row = (i - 1); row < ((i - 1) + 3); row++)
            {
                //For each cell in each rown of the 9 element grid around the current cell.
                for (int col = (j - 1); col < ((j - 1) + 3); col++)
                {
                    //If the cell is an active cell which is inside the boundaries of the image, calculate the Gx & Gy totals for Red, Green & Blue.
                    if ((row >= 0) && (row <= (height - 1)) && (col >= 0) && (col <= (width - 1)))
                    {
                        //Calculate Gx Totals - Active Pixel.
                        totalGxRed = totalGxRed + (gxValue[rowCount][colCount] * (image[(row)][col].rgbtRed));
                        totalGxGreen = totalGxGreen + (gxValue[rowCount][colCount] * (image[(row)][col].rgbtGreen));
                        totalGxBlue = totalGxBlue + (gxValue[rowCount][colCount] * (image[(row)][col].rgbtBlue));

                        //Calculate Gy Totals - Active Pixel.
                        totalGyRed = totalGyRed + (gyValue[rowCount][colCount] * (image[(row)][col].rgbtRed));
                        totalGyGreen = totalGyGreen + (gyValue[rowCount][colCount] * (image[(row)][col].rgbtGreen));
                        totalGyBlue = totalGyBlue + (gyValue[rowCount][colCount] * (image[(row)][col].rgbtBlue));
                    }
                    //Increment the column count to align the Gx & Gy value arrays correctly. 
                    colCount++;
                }
                //Increment the row count and reset the column count to align the Gx & Gy value arrays correctly. 
                colCount = 0;
                rowCount++;
            }

            //Calculate the new value of Red, Green and Blue for the pixel.
            //Ensure that the new value of Red, Green or Blue does not exceed the maximum value it can be.
            if ((round(sqrt((totalGxRed * totalGxRed) + (totalGyRed * (double)totalGyRed)))) <= 255)
            {
                copy[i][j].rgbtRed = round(sqrt((totalGxRed * totalGxRed) + (totalGyRed * (double)totalGyRed)));
            }
            else
            {
                copy[i][j].rgbtRed = 255;
            }
            if ((round(sqrt((totalGxGreen * totalGxGreen) + (totalGyGreen * (double)totalGyGreen)))) <= 255)
            {
                copy[i][j].rgbtGreen = round(sqrt((totalGxGreen * totalGxGreen) + (totalGyGreen * (double)totalGyGreen)));
            }
            else
            {
                copy[i][j].rgbtGreen = 255;
            }
            if ((round(sqrt((totalGxBlue * totalGxBlue) + (totalGyBlue * (double)totalGyBlue)))) <= 255)
            {
                copy[i][j].rgbtBlue = round(sqrt((totalGxBlue * totalGxBlue) + (totalGyBlue * (double)totalGyBlue)));
            }
            else
            {
                copy[i][j].rgbtBlue = 255;
            }
        }
    }
    
    //Overwrite the original image values with the new image values.
    for (int x = 0; x < height; x++)
    {
        for (int y = 0; y < width; y++)
        {
            image[x][y] = copy[x][y];
        }
    }
    return;
}
