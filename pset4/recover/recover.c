#include <cs50.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#define SIZE_OF_BLOCK 512

typedef unsigned char BYTE;

bool is_start_of_JPEG(BYTE buffer[])
{
    bool value = false;

    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
    {
        value = true;
    }

    return value;
}

int main(int argc, char *argv[]) // argc es la cant de parametros y argv es el vector con los parametros
{
    // verificar que pasan los parametros correctos
    if (argc != 2)
    {
        printf("Usage: ./recover image \n");
        return 1;
    }


    // abrir archivo de tarjeta de memoria
    FILE *file_pointer = fopen(argv[1], "r");


    // verificar que existe
    if (file_pointer == NULL)
    {
        printf("File %s could not be open\n", argv[1]);
        return 1;
    }


    // establecer numero de archivo que incrementara dependiendo del que se abra
    int fileID = 0;

    //declaracion del archivo temporal donde escribire
    BYTE buffer[SIZE_OF_BLOCK];

    FILE *output = NULL;

    bool image_found = false;

    while (fread(buffer, SIZE_OF_BLOCK, 1, file_pointer) == 1)
    {
        //verificar que es un archivo JPEG
        if (is_start_of_JPEG(buffer))
        {
            image_found = true;
            //si no es el primer JPEG encontrado
            if (fileID != 0)
            {
                //cerrar archivo anterior
                fclose(output);
            }

            //crear nuevo archivo para escribir
            char name[8];
            sprintf(name, "%03i.jpg", fileID);

            //abrir archivo creado para escribir
            output = fopen(name, "w");

            //especificaciones del archivo para escribir
            fwrite(buffer, SIZE_OF_BLOCK, 1, output);

            //incrementar el numero de archivo para cuando se consiga un nuevo jpg
            fileID++;
        }

        //cuando ya encontre un jpeg
        else if (image_found)
        {
            //especificaciones del archivo para escribir
            fwrite(buffer, SIZE_OF_BLOCK, 1, output);
        }
    }
    fclose(output);
    fclose(file_pointer);
}