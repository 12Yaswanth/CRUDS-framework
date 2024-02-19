// To test the primeOrNot.c program

#include <stdio.h>
#include <stdlib.h>
#define TEST_CASES_COUNT 20
#include "primeOrNot.c"

int main() 
{
    int TestCounter, PassCounter = 0;
    char TestCases[TEST_CASES_COUNT][10] = {"-5", "0", "11", "23", "31", "47", "89", "101", "127", "137", "abc", "R8", "Hello", "7eleven", "Batman7", "!", " ", "\t", "%", "ZZZ"};

    for (TestCounter = 0; TestCounter < TEST_CASES_COUNT; TestCounter++) 
    {
        if (isPrime(TestCases[TestCounter]) < 0)
        {
            printf("Test Failed: %s\n", TestCases[TestCounter]);
        }
        else
        {
            PassCounter++;
        }
    }
    
    if (TestCounter == PassCounter)
    {
        printf("All tests passed.\n");
    }
    return 0;
}
