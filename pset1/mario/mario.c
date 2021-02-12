#include <stdio.h>
#include <cs50.h>

int main(void)
//Ask user for height
{
    int height = get_int("Height: ");

    while (height < 1 || height >= 9)
    {
        height = get_int("Height: ");
    }

    for (int i = 0; i < height; i++)
    {
        for (int d = i; d < height - 1; d++) //print dots before hashes
        {
            printf(" ");
        }

        for (int r = 0; r < i + 1; r++) //print every repetition of hashes in a new line
        {
            printf("#");
        }
        for (int d = 0; d < 2; d++) //print dots before hashes
        {
            printf(" ");
        }

        for (int r = 0; r < i + 1; r++) //print every repetition of hashes in a new line
        {
            printf("#");
        }
        printf("\n");
    }
}