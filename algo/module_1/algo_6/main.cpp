/* Дано N кубиков. Требуется определить каким количеством способов можно выстроить из этих кубиков пирамиду.
Формат входных данных: На вход подается количество кубиков N.
Формат выходных данных: Вывести число различных пирамид из N кубиков.
Широкая пирамида.​ Каждый вышележащий слой пирамиды должен быть строго меньше
нижележащего.
N ≤ 300. */

#include <cstdio>

size_t myPrettyPiramydka(size_t cubeNum)
{
    size_t **count_table = new size_t*[cubeNum + 1];
    for(size_t i = 0; i < cubeNum + 1; i++)
    {
        count_table[i] = new size_t[cubeNum + 1];
    }

    for(size_t k = 0; k < cubeNum + 1; k++)
    {
        for(size_t n = k; n < cubeNum + 1; n++)
        {
            if(n == k)
            {
                count_table[k][n] = 1;
            }
            else if(k == 0 || k > n)
            {
                count_table[k][n] = 0;
            }
            else
            {
                count_table[k][n] = count_table[k-1][n-k] + count_table[k-1][n-1];
            }
        }
    }
    size_t combinations = 0;
    for(size_t i = 0; i < cubeNum + 1; i++)
    {
        combinations += count_table[i][cubeNum];
    }
    for(size_t i = 0; i < cubeNum + 1; i++)
    {
        delete [] count_table[i];
    }
    delete [] count_table;
    return combinations;
}

int main()
{
    size_t cubeNum;
    if(scanf("%lu", &cubeNum) != 1)
    {
        printf("[wrong input]");
        return 0;
    }
    size_t combinations = myPrettyPiramydka(cubeNum);

    printf("%lu", combinations);
    return 0;
}
