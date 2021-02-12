#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

bool check_valid_key(string text)
{
    int n = strlen(text);
    bool tf = true;
    for (int i = 0; i < n; i++)
    {
        if (!isdigit(text[i]))
        {
            tf = false;
        }
    }
    return tf;
}


int main(int argc, string argv[]) // argc es la cant de parametros, argv es el vector con los parametros
{
    if ((argc == 2) && check_valid_key(argv[1]))
    {
        string plaintext = get_string("plaintext: ");  //ask for a text

        int n = strlen(plaintext);
        int key = atoi(argv[1]);
        char letter = 0;

        printf("ciphertext: ");

        for (int i = 0; i < n; i++)  //search for every character in the string
        {
            if (isalpha(plaintext[i]))  //check if is an alphabetic character
            {
                if (isupper(plaintext[i]))
                {
                    int ln = (plaintext[i]);          //ln es el caracter localizado o letra actual
                    char c = ln - 65 + key;           //ln se localiza desde cero
                    letter = (c % 26) + 65;           //ln = 97   k = 30  c = 62  letter = 10 + 97   letter = 107

                    if (letter > 90)    //letter = 107
                    {
                        letter = c - 90 + 65;         //letter = 107 - 90  letter = 17 + 65  letter = 82
                    }
                }

                else if (islower(plaintext[i]))
                {
                    int ln = (plaintext[i]);           //ln es el caracter localizado o letra actual
                    char c = ln - 97 + key;           //ln se localiza desde cero
                    letter = (c % 26) + 97;           //ln = 97   k = 30  c = 30  letter = 4 + 97   letter = 101

                    if (letter > 122)                 //letter = 123
                    {
                        letter = c - 122 + 97;        //letter = 123 - 122  letter = 1 + 65  letter = 67
                    }
                }
            }

            else
            {
                letter = plaintext[i];  //mantaining character as it is
            }

            printf("%c", letter);
        }
        printf("\n");
        return 0;
    }

    else
    {
        printf("Usage: ./caesar key \n");
        return 1;
    }
}