#include <stdio.h>
#include <cs50.h>
#include <math.h>

#define QUARTERS 25
#define DIMES 10
#define NICKELS 5
#define PENNIES 1


int main(void)
//ask user for change owed in dollars
{
    float dollars = get_float("Change owed: ");

//ask again while change owed is under or equal to 0
    while (dollars <= 0)
    {
        dollars = get_float("Change owed: ");
    }
//convert dollars to cents
    int cents = round(dollars * 100);

    int total1 = 0;
    int total2 = 0;
    int total3 = 0;
    int total4 = 0;

//increment totals
    while (QUARTERS <= cents)
    {
        cents = cents - QUARTERS;
        total1 += 1;
    }
    while (DIMES <= cents)
    {
        cents = cents - DIMES;
        total2 += 1;
    }
    while (NICKELS <= cents)
    {
        cents = cents - NICKELS;
        total3 += 1;
    }
    while (PENNIES <= cents)
    {
        cents = cents - PENNIES;
        total4 += 1;
    }

//sum totals and print total coins
    int total_coins = total1 + total2 + total3 + total4;
    printf("%d\n", total_coins);
}

