/* 2_3. ​Даны два строго возрастающих массива целых чисел A[0..n) и B[0..m) и число k. Найти количество
таких пар индексов (i, j), что A[i] + B[j] = k. Время работы O(n + m).
n, m ≤ 100000.*/

#include <cstdio>
#include <cstdlib>

size_t findMatchIndexes(int *a, size_t n, int *b, size_t m, int k)
{
    size_t count = 0;
    for(ssize_t i = 0, j = m - 1; i < (ssize_t)n && j >= 0; )
    {
        if(a[i] + b[j] < k)
        {
            i++;
        }
        else if(a[i] + b[j] > k)
        {
            j--;
        }
        else
        {
            count++;
            i++;
            j--;
        }
    }
    return count;
}

int main()
{
    size_t n = 0;
    if(scanf("%lu", &n) < 1)
    {
        printf("[wrong input]");
        return 0;
    }
    int *a = new int[n];
    for(size_t i = 0; i < n; i++)
    {
        if(scanf("%d", a+i) < 1)
        {
            printf("[wrong input]");
            delete [] a;
            return 0;
        }
    }
    size_t m = 0;
    if(scanf("%lu", &m) < 1)
    {
        printf("[wrong input]");
        delete [] a;
        return 0;
    }
    int *b = new int[m];
    for(size_t i = 0; i < m; i++)
    {
        if(scanf("%d", b+i) < 1)
        {
            printf("[wrong input]");
            delete [] a;
            delete [] b;
            return 0;
        }
    }
    int k = 0;
    if(scanf("%d", &k) < 1)
    {
        printf("[wrong input]");
        delete [] a;
        delete [] b;
        return 0;
    }

    printf("%lu", findMatchIndexes(a, n, b, m, k));

    delete [] a;
    delete [] b;

    return 0;
}
