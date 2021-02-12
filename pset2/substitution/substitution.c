#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <string.h>


bool check_alphabetic_characters(string key)
{
    //check if is an alphabetic character
    int n = strlen(key);                // reviso el largo de la key
    bool isValid = true;

    for (int i = 0; i < n; i++)
    {
        if (!isalpha(key[i]))
        {
            isValid = false;
        }
    }
    return isValid;
}


bool check_lenght(string key)
{
    //check if lenght is equal to 26,
    int n = strlen(key);                // reviso el largo de la key
    bool isValid = true;                // por defecto es valido y buscamos un caso para el que sea falso
    if (n != 26)
    {
        isValid = false;
    }
    return isValid;
}


bool check_repeated_characters(string key)
{
    //check that any character is repeated it
    int n = strlen(key);
    bool isValid = true;

    for (int i = 0; i < n; i++)         //go through every character
    {
        for (int j = 0; j < n; j++)    //go through every character
        {
            if ((key[i] == key[j]) && (i != j))
            {
                isValid = false;
            }
        }
    }
    return isValid;
}

int get_letter_position(char letter)
{
    int position = 0;

    if (isalpha(letter))
    {
        if (isupper(letter))
        {
            position = (letter - 'A');
        }

        else
        {
            position = (letter - 'a');
        }
    }
    return position;
}


int main(int argc, string argv[]) // argc es la cant de parametros y argv es el vector con los parametros
{
    if ((argc == 2) && check_alphabetic_characters(argv[1]) && check_lenght(argv[1]) && check_repeated_characters(argv[1]))
    {
        string plaintext = get_string("plaintext: ");  //ask for a text

        int n = strlen(plaintext);
        string key = argv[1];
        char letter = 0;

        printf("ciphertext: ");

        for (int i = 0; i < n; i++)             //search for every character in the string
        {
            if (isalpha(plaintext[i]))          //check if is an alphabetic character
            {
                if (isupper(plaintext[i]))
                {
                    int let = get_letter_position(plaintext[i]);        //ln es el caracter localizado o letra actual del plaintext
                    char new_cipher_letter = key[let];                  //cipher es la letra actual del nuevo abecedario dado por la llave

                    letter = new_cipher_letter;     //la letra en posicion i del plaintext va a ser la letra en posicion j de la llave
                    letter = toupper(letter);       //si originalmente era mayuscula, independientemente de lo q diga la llave se mantedra en mayuscula
                }

                else if (islower(plaintext[i]))
                {
                    int let = get_letter_position(plaintext[i]);        //ln es el caracter localizado o letra actual del plaintext
                    char new_cipher_letter = key[let];                  //cipher es la letra actual del nuevo abecedario dado por la llave

                    letter = new_cipher_letter;     //la letra en posicion i del plaintext va a ser la letra en posicion j de la llave
                    letter = tolower(letter);       //si originalmente era minuscula, independientemente de lo q diga la llave se mantedra en minuscula
                }
            }

            else
            {
                letter = plaintext[i];              //mantaining character as it is
            }

            printf("%c", letter);
        }
        printf("\n");
        return 0;
    }


    else if (argc != 2)
    {
        printf("Usage: ./caesar key \n");
        return 1;
    }

    else if (!check_alphabetic_characters(argv[1]))
    {
        printf("key must only contain alphabetic characters\n");
        return 1;
    }

    else if (!check_lenght(argv[1]))
    {
        printf("key must contain 26 alphabetic characters\n");
        return 1;
    }

    else if (!check_repeated_characters(argv[1]))
    {
        printf("key must not contain repeated characters\n");
        return 1;
    }
}
