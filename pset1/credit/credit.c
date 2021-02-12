#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>

// function that counts digits
// numbers will be numbers from input
int count_digits(long numbers)
{
    int digits_amount = 0;      // starting number of digits
    while (numbers != 0)
    {
        numbers = numbers / 10;

        digits_amount ++;
    }

    return digits_amount;
}


int add_digits(int num)
{
    int sum = 0;

    while (num != 0)
    {
        int last_digit = num % 10;
        num = (num / 10);
        sum += last_digit;
    }

    return sum;
}


int odd_digits(long num)
{
    int odds_sum = 0;

    num = num / 10;        //elimino el ultimo digito para empezar con el penultimo

    while (num != 0)
    {
        int odd_num = num % 10;     // obtengo el penultimo digito
        int mul = odd_num * 2;        // asumo que es impar y lo multiplico por dos
        odds_sum += add_digits(mul);     // sumo los digitos impares

        num = num / 100;   // divido entrre 100 para actualizar el numero
    }
    return odds_sum;
}


int even_digits(long num)
{
    int even_sum = 0;

    while (num != 0)
    {
        int even_num = num % 10;     // obtengo el ultimo digito una vez dividido
        even_sum += even_num;        // lo sumo

        long new_num = num / 100;    // elimino dos digitos para sumar los no mltiplicados

        num = new_num;               // actualizo el numero
    }
    return even_sum;
}



int main(void)
//Ask user for credit number

{
    long credit = get_long("Credit number: ");

    //calculate number of digits
    int number_of_digits = count_digits(credit);

    //verifying odd and even digits to sum
    int odd_sum = odd_digits(credit);
    int even_sum = even_digits(credit);

    int total_sum = odd_sum + even_sum;

//checksum
    bool is_valid = false; // (total_sum % 10 != 0);
    if (total_sum % 10 == 0)
    {
        is_valid = true;
    }

    //chech what kind of credit card is given
    int first_2_numbers = credit / (pow(10, (number_of_digits - 2)));
    int first_number = credit / (pow(10, (number_of_digits - 1)));
    string type_c;

    if ((is_valid) && (number_of_digits == 13 || number_of_digits == 16) && first_number == 4)
    {
        type_c = "VISA";
    }

    else if ((is_valid) && number_of_digits == 15 && (first_2_numbers == 34 || first_2_numbers == 37))
    {
        type_c = "AMEX";
    }

    else if ((is_valid) && number_of_digits == 16 && (first_2_numbers == 51 || first_2_numbers == 52
             || first_2_numbers == 53 || first_2_numbers == 54 || first_2_numbers == 55))
    {
        type_c = "MASTERCARD";
    }

    else
    {
        type_c = "INVALID";
    }

    printf("%s\n", type_c);
    return 0;
}
