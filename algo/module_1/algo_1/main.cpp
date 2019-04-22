/* 1_3. ​Даны две рациональные дроби: a/b и c/d. Сложить их и результат представьте в виде несократимой
дроби m/n. Вывести числа m и n.
a, b, c, d ≤ 1000.
T(n) = O(lg(max(c, d)))
Расходуемая память: O(1)
*/

#include <cstdio>

struct fraction
{
    int numerator;
    int denominator;
};

int gcd(int a, int b) // great common divisor, Euclid descent
{
    while(b != 0)
    {
        int tmp = a % b;
        a = b;
        b = tmp;
    }
    return a;
}
int lcm(int a, int b) // least common multiple
{
    return a / gcd(a, b) * b;
}
fraction reduceFraction(fraction cancellable)
{
    int gcDivisor = gcd(cancellable.denominator, cancellable.numerator);
    if(gcDivisor == 0)
    {
        return cancellable;
    }
    fraction result;
    result.denominator = cancellable.denominator / gcDivisor;
    result.numerator = cancellable.numerator / gcDivisor;

    return result;
}

fraction sumFractions(fraction fraction_1, fraction fraction_2)
{
    fraction result;
    result.denominator = lcm(fraction_1.denominator, fraction_2.denominator);
    result.numerator = fraction_1.numerator * (result.denominator / fraction_1.denominator) +
                          fraction_2.numerator * (result.denominator / fraction_2.denominator);
    return result;
}

int main()
{
    fraction fraction_1, fraction_2;
    if(scanf("%d%d%d%d", &fraction_1.numerator, &fraction_1.denominator, &fraction_2.numerator, &fraction_2.denominator) < 4)
    {
        printf("[wrong input]");
        return 0;
    }

    fraction result = sumFractions(fraction_1, fraction_2);
    result = reduceFraction(result);

    printf("%d %d", result.numerator, result.denominator);
    return 0;
}
