#include "helpers.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float originalRed = image[i][j].rgbtRed;
            float originalGreen = image[i][j].rgbtGreen;
            float originalBlue = image[i][j].rgbtBlue;

            image[i][j].rgbtRed = round((originalRed + originalGreen + originalBlue) / 3);
            image[i][j].rgbtBlue = round((originalRed + originalGreen + originalBlue) / 3);
            image[i][j].rgbtGreen = round((originalRed + originalGreen + originalBlue) / 3);
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int originalRed = image[i][j].rgbtRed;
            int originalGreen = image[i][j].rgbtGreen;
            int originalBlue = image[i][j].rgbtBlue;


            image[i][j].rgbtRed = round(fmin(255.0, .393 * originalRed + .769 * originalGreen + .189 * originalBlue));
            image[i][j].rgbtBlue = round(fmin(255.0, .272 * originalRed + .534 * originalGreen + .131 * originalBlue));
            image[i][j].rgbtGreen = round(fmin(255.0, .349 * originalRed + .686 * originalGreen + .168 * originalBlue));
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            RGBTRIPLE temp = image[i][j];

            image[i][j] = image[i][width - j - 1]; //we saved the opposite pixel

            image[i][width - j - 1] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    memcpy(temp, image, sizeof(RGBTRIPLE) * height * width); //make copy of the image




    //seccion de los pixeles del medio empieza aca
    for (int i = 1; i < (height - 1); i++)
    {
        for (int j = 1; j < (width - 1); j++)
        {
            float counter = 9.0;
            float tempRed = 0;
            float tempBlue = 0;
            float tempGreen = 0;

            for (int k = (i - 1); k <= (i + 1); k++) // revisar k < i + 1
            {
                for (int l = (j - 1); l <= (j + 1); l++) // revisar
                {
                    tempRed += temp[k][l].rgbtRed;
                    tempBlue += temp[k][l].rgbtBlue;
                    tempGreen += temp[k][l].rgbtGreen;
                }
            }

            tempRed = tempRed / counter;
            tempBlue = tempBlue / counter;
            tempGreen = tempGreen / counter;

            image[i][j].rgbtRed = round(tempRed);
            image[i][j].rgbtBlue = round(tempBlue);
            image[i][j].rgbtGreen = round(tempGreen);
        }
    }
    //seccion de los pixeles del medio termina aca




    //seccion de los pixeles del borde SUPERIOR empieza aca
    for (int j = 1; j < width - 1; j++)
    {
        int i = 0; //si mi fila i siempre es cero, me muevo en la columna j
        {
            float counter = 0;
            float tempRed = 0;
            float tempBlue = 0;
            float tempGreen = 0;


            for (int k = i; k <= (i + 1); k++)
            {
                for (int l = (j - 1); l <= (j + 1); l++)
                {
                    tempRed += temp[k][l].rgbtRed;
                    tempBlue += temp[k][l].rgbtBlue;
                    tempGreen += temp[k][l].rgbtGreen;

                    counter += 1;
                }
            }

            tempRed = tempRed / counter;
            tempBlue = tempBlue / counter;
            tempGreen = tempGreen / counter;

            image[i][j].rgbtRed = round(tempRed);
            image[i][j].rgbtBlue = round(tempBlue);
            image[i][j].rgbtGreen = round(tempGreen);
        }
    }
    //seccion de los pixeles del borde SUPERIOR termina aca




    //seccion de los pixeles del borde INFERIOR empieza aca
    for (int j = 1; j < width - 1; j++)
    {
        int i = width - 1; //si mi fila i siempre es el maximo menos uno, me muevo en la columna j
        {
            float counter = 0;
            float tempRed = 0;
            float tempBlue = 0;
            float tempGreen = 0;


            for (int k = i - 1; k <= i; k++) //fila
            {
                for (int l = (j - 1); l <= (j + 1); l++) //columna
                {
                    tempRed += temp[k][l].rgbtRed;
                    tempBlue += temp[k][l].rgbtBlue;
                    tempGreen += temp[k][l].rgbtGreen;

                    counter += 1;
                }
            }

            tempRed = tempRed / counter;
            tempBlue = tempBlue / counter;
            tempGreen = tempGreen / counter;

            image[i][j].rgbtRed = round(tempRed);
            image[i][j].rgbtBlue = round(tempBlue);
            image[i][j].rgbtGreen = round(tempGreen);
        }
    }
    //seccion de los pixeles del borde INFERIOR termina aca




    //seccion de los pixeles del borde IZQUIERDO empieza aca
    for (int i = 1; i < width - 1; i++)
    {
        int j = 0; //si mi columna j siempre es 0, me muevo en la fila i
        {
            float counter = 0;
            float tempRed = 0;
            float tempBlue = 0;
            float tempGreen = 0;


            for (int k = i - 1; k <= i + 1; k++) //fila
            {
                for (int l = j; l <= j + 1; l++) //columna
                {
                    tempRed += temp[k][l].rgbtRed;
                    tempBlue += temp[k][l].rgbtBlue;
                    tempGreen += temp[k][l].rgbtGreen;

                    counter += 1;
                }
            }

            tempRed = tempRed / counter;
            tempBlue = tempBlue / counter;
            tempGreen = tempGreen / counter;

            image[i][j].rgbtRed = round(tempRed);
            image[i][j].rgbtBlue = round(tempBlue);
            image[i][j].rgbtGreen = round(tempGreen);
        }
    }
    //seccion de los pixeles del borde IZQUIERDO termina aca




    //seccion de los pixeles del borde DERECHO empieza aca
    for (int i = 1; i < width - 1; i++)
    {
        int j = height - 1; //si mi columna j siempre es 0, me muevo en la fila i
        {
            float counter = 0;
            float tempRed = 0;
            float tempBlue = 0;
            float tempGreen = 0;


            for (int k = i - 1; k <= i + 1; k++) //fila
            {
                for (int l = j - 1; l <= j; l++) //columna
                {
                    tempRed += temp[k][l].rgbtRed;
                    tempBlue += temp[k][l].rgbtBlue;
                    tempGreen += temp[k][l].rgbtGreen;

                    counter += 1;
                }
            }

            tempRed = tempRed / counter;
            tempBlue = tempBlue / counter;
            tempGreen = tempGreen / counter;

            image[i][j].rgbtRed = round(tempRed);
            image[i][j].rgbtBlue = round(tempBlue);
            image[i][j].rgbtGreen = round(tempGreen);
        }
    }
    //seccion de los pixeles del borde DERECHO termina aca



    //seccion de los pixeles de la ESQUINA SUPERIOR IZQUIERDA empieza aca
    float tempRed = temp[0][0].rgbtRed + temp[0][1].rgbtRed + temp[1][0].rgbtRed + temp[1][1].rgbtRed;
    float tempBlue = temp[0][0].rgbtBlue + temp[0][1].rgbtBlue + temp[1][0].rgbtBlue + temp[1][1].rgbtBlue;
    float tempGreen = temp[0][0].rgbtGreen + temp[0][1].rgbtGreen + temp[1][0].rgbtGreen + temp[1][1].rgbtGreen;

    tempRed = tempRed / 4.0;
    tempBlue = tempBlue / 4.0;
    tempGreen = tempGreen / 4.0;

    image[0][0].rgbtRed = round(tempRed);
    image[0][0].rgbtBlue = round(tempBlue);
    image[0][0].rgbtGreen = round(tempGreen);
    //seccion de los pixeles de ESQUINA SUPERIOR IZQUIERDA termina aca




    //seccion de los pixeles de la ESQUINA SUPERIOR DERECHA empieza aca
    tempRed = temp[0][width - 1].rgbtRed + temp[0][width - 2].rgbtRed + temp[1][width - 1].rgbtRed + temp[1][width - 2].rgbtRed;
    tempBlue = temp[0][width - 1].rgbtBlue + temp[0][width - 2].rgbtBlue + temp[1][width - 1].rgbtBlue + temp[1][width - 2].rgbtBlue;
    tempGreen = temp[0][width - 1].rgbtGreen + temp[0][width - 2].rgbtGreen + temp[1][width - 1].rgbtGreen + temp[1][width -
                2].rgbtGreen;

    tempRed = tempRed / 4.0;
    tempBlue = tempBlue / 4.0;
    tempGreen = tempGreen / 4.0;

    image[0][width - 1].rgbtRed = round(tempRed);
    image[0][width - 1].rgbtBlue = round(tempBlue);
    image[0][width - 1].rgbtGreen = round(tempGreen);
    //seccion de los pixeles de ESQUINA SUPERIOR DERECHA termina aca




    //seccion de los pixeles de la ESQUINA INFERIOR IZQUIERDA empieza aca
    tempRed = temp[height - 1][0].rgbtRed + temp[height - 1][1].rgbtRed + temp[height - 2][0].rgbtRed + temp[height - 2][1].rgbtRed;
    tempBlue = temp[height - 1][0].rgbtBlue + temp[height - 1][1].rgbtBlue + temp[height - 2][0].rgbtBlue + temp[height -
               2][1].rgbtBlue;
    tempGreen = temp[height - 1][0].rgbtGreen + temp[height - 1][1].rgbtGreen + temp[height - 2][0].rgbtGreen + temp[height -
                2][1].rgbtGreen;

    tempRed = tempRed / 4.0;
    tempBlue = tempBlue / 4.0;
    tempGreen = tempGreen / 4.0;

    image[height - 1][0].rgbtRed = round(tempRed);
    image[height - 1][0].rgbtBlue = round(tempBlue);
    image[height - 1][0].rgbtGreen = round(tempGreen);
    //seccion de los pixeles de ESQUINA INFERIOR IZQUIERDA termina aca




    //seccion de los pixeles de la ESQUINA INFERIOR DERECHA empieza aca
    tempRed = temp[height - 1][width - 1].rgbtRed + temp[height - 1][width - 2].rgbtRed + temp[height - 2][width - 1].rgbtRed +
              temp[height - 2][width - 2].rgbtRed;
    tempBlue = temp[height - 1][width - 1].rgbtBlue + temp[height - 1][width - 2].rgbtBlue + temp[height - 2][width - 1].rgbtBlue +
               temp[height - 2][width - 2].rgbtBlue;
    tempGreen = temp[height - 1][width - 1].rgbtGreen + temp[height - 1][width - 2].rgbtGreen + temp[height - 2][width - 1].rgbtGreen +
                temp[height -  2][width - 2].rgbtGreen;

    tempRed = tempRed / 4.0;
    tempBlue = tempBlue / 4.0;
    tempGreen = tempGreen / 4.0;

    image[height - 1][width - 1].rgbtRed = round(tempRed);
    image[height - 1][width - 1].rgbtBlue = round(tempBlue);
    image[height - 1][width - 1].rgbtGreen = round(tempGreen);
    //seccion de los pixeles de ESQUINA INFERIOR DERECHA termina aca
}