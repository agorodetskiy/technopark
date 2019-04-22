/* 3_1. ​Дан отсортированный массив целых чисел A[0..n-1] и массив целых чисел B[0..m-1]. Для каждого
элемента массива B[i] найдите минимальный индекс k минимального элемента массива A, равного или
превосходящего B[i]: A[k] >= B[i]. Если такого элемента нет, выведите n. Время работы поиска k для каждого
элемента B[i]: O(log(k)).
n, m ≤ 10000.
Формат входных данных.
В первой строчке записаны числа n и m. Во второй и третьей массивы A и B соответственно. */

#include <cstdio>
#include <cstdlib>

int binarySearch(const int *start, const int *end, int key) // inclusive pointers
{
    size_t left = 0, mid = 0;
    size_t right = end - start;

    while(left < right)
    {
        mid = left + (right - left) / 2; // to avoid overflow
        if(key <= start[mid])
        {
            right = mid;
        }
        else
        {
            left = mid + 1;
        }
    }
    return start[left] >= key ? left : -1;
}
void setSearchRange(size_t &leftSearchBorder, size_t &rightSearchBorder, int *a, size_t n, int key) // find range of search between indexes
{                                                                                                   // that are powers of two
    size_t powerOfTwo;
    for(powerOfTwo = 1; powerOfTwo < n && a[powerOfTwo] < key; powerOfTwo *= 2);
    leftSearchBorder = powerOfTwo / 2;
    rightSearchBorder = powerOfTwo < n ? powerOfTwo : n - 1;
}
int findIndex(int *a, size_t n, int comparisonElement)
{
    size_t leftSearchBorder = 0, rightSearchBorder = n - 1;
    int result = 0;
    setSearchRange(leftSearchBorder, rightSearchBorder, a, n, comparisonElement);
    result = binarySearch(a + leftSearchBorder, a + rightSearchBorder, comparisonElement);
    if(result != -1)
    {
        result += leftSearchBorder;
    }
    else
    {
        result = n;
    }

    return result;
}

int main()
{
    size_t n = 0, m = 0;
    if(scanf("%lu%lu", &n, &m) < 2)
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

    for(size_t i = 0; i < m; i++)
    {
        printf("%d ", findIndex(a, n, b[i]));
    }

    delete [] a;
    delete [] b;

    return 0;
}
