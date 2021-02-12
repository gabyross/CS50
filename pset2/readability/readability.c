#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text)
{
    int letters = 0;
    int n = strlen(text);

    for (int i = 0; i < n; i++)
    {
        if (isalpha(text[i]))         //check if is an alphabetic character
        {
            letters++;                      //increment letters wether is opper or lowercase
        }
    }
    return letters;
}


int count_words(string text)
{
    int words = 0;
    int n = strlen(text);

    for (int i = 0; i < n; i++)
    {
        if (isgraph(text[i]) && isspace(text[i + 1]))         //check if is an alphabetic character with an space
        {
            words++;                                            //increment words if is a letter followed by a space
        }
    }

    // si la posicion del largo del string menos uno es un caracter, incrementar la variable words
    if (isgraph(text[n - 1]))
    {
        words++;
    }
    return words;
}


int count_sentences(string text)
{
    int sentences = 0;
    int n = strlen(text);

    for (int i = 0; i < n; i++)
    {
        if ((text[i] == '!') || (text[i] == '.') || (text[i] == '?'))   //check if is a character used to end sentences
        {
            sentences++;                                                            //increment sentences if is a !, . or a ?
        }
    }
    return sentences;
}


int main(void)
{
    //ask user for text
    string text = get_string("Text: ");

    //count number of letters, words, and sentences in the text, and print it
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    float L = (100 * letters) / (float)words;
    float S = (100 * sentences) / (float)words;

    int grade = round(0.0588 * L - 0.296 * S - 15.8);         //round grade to the closest int number

    if (grade > 16)
    {
        printf("Grade 16+\n");
    }

    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }

    else
    {
        printf("Grade %i\n", grade);
    }
}