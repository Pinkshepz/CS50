#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // get input for credit card number
    long long credit = get_long_long("Number: ");

    // create variable for cumulative sum
    int sum = 0;
    int len = 0;
    long long clone = credit;

    // iterate over every other digits
    while (credit > 0)
    {
        // collect value, sum and cut
        if (credit > 0)
        {
            sum += credit % 10;
        }

        credit /= 10;

        // sum of the product of digits and add to the cumulative sum
        int digit = (credit % 10) * 2;
        int product_sum = (digit % 10) + (digit / 10);
        sum += product_sum;

        // cut
        credit /= 10;
    }

    // get first two digits of credit
    while (clone >= 100)
    {
        len += 1;
        clone /= 10;
    }

    // return whether credit number is vaild or not
    if (sum % 10 == 0)
    {
        if ((clone == 34 || clone == 37) && len == 15 - 2)
        {
            printf("AMEX\n");
        }

        else if (clone / 10 == 4 && (len == 16 - 2 || len == 13 - 2))
        {
            printf("VISA\n");
        }

        else if ((clone > 50 && clone < 56) && (len == 16 - 2))
        {
            printf("MASTERCARD\n");
        }

        else
        {
            printf("INVALID\n");
        }
    }

    else
    {
        printf("INVALID\n");
    }
}