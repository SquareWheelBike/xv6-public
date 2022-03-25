#include "types.h"
#include "user.h"

int isPrime(const int number)
{
    if (number <= 1)
        return 0;
    int prime = 1;
    if (number != 2 && number % 2 == 0)
        prime = 0;
    else
        for (int i = 3; i*i <= number && prime; i += 2)
            if (number % i == 0)
                prime = 0;
    return prime;
}

int main(void)
{
    // calculate all primes up to beeg
    int count = 0;
    int max = 20000000;
    for (int i = 0; i < max; i++)
    {
        count += isPrime(i);
    }
    printf(1, "%d says %d primes up to %d\n", getpid(), count, max);

    exit();
}