// To know whether a given number is prime or not

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

int isPrime(char *value) 
{
    int divisor = 0;
    int number = atoi(value);

    if (!isdigit(value[0]) || number <= 1) 
    {
        return 0;
    }
    if(number % 2 == 0 && number != 2)
    {
    	return 0;
    }

    else
    {
        for (divisor = 3; divisor * divisor <= number; divisor +=2) 
        {
            if (number % divisor == 0) 
            {
                return 0;
            }
        }
        return 1;
    }
}
