#include <stdio.h>
#include <cs50.h>

int main(void)
//style a string is displayed on screen asking for input, and it is saved as a variable
{
    string name = get_string("What is your name?\n");
    printf("Hello, %s\n", name);
}