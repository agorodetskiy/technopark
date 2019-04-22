/* Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] размера n. Требуется
найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло на позиции с индексом
k (0..n-1) в отсортированном массиве. Напишите нерекурсивный алгоритм.
Требования к дополнительной памяти: O(n). Требуемое среднее время работы: O(n).
Функцию Partition следует реализовывать методом прохода двумя итераторами в одном
направлении. */

#include <cstdio>
#include <algorithm>
#include <vector>

using std::vector;

size_t partition(size_t pivot, int *arr, size_t left, size_t right) // borders inclusive
{
    std::swap(arr[pivot], arr[right]);
    pivot = right;
    size_t i, j;
    for(i = left, j = left; j <= right;)
    {
        if(arr[j] > arr[pivot])
        {
            j++;
        }
        else
        {
            std::swap(arr[j], arr[i]);
            i++;
            j++;
        }
    }
    pivot = i-1;
    return pivot;
}

size_t median(int *arr, size_t left, size_t right)
{
    size_t l = left, m = left + (right - left) / 2, r = right;
    if(arr[l] < arr[r])
    {
        if(arr[l] > arr[m])
        {
            return l;
        }
        else
        {
            return arr[m] < arr[r] ? m : r;
        }
    }
    else
    {
        if(arr[r] > arr[m])
        {
            return r;
        }
        else
        {
            return arr[l] < arr[m] ? l : m;
        }
    }
}

int ordinalStat(int *arr, size_t len, size_t k)
{
    size_t left = 0, right = len - 1;  // borders inclusive
    while(true)
    {
        size_t pivot = median(arr, left, right);
        pivot = partition(pivot, arr, left, right);

        if(pivot == k)
        {
            return arr[k];
        }
        else if(k < pivot)
        {
            right = pivot - 1;
        }
        else
        {
            left = pivot + 1;
        }
    }
}

int main()
{
    size_t n = 0, k = 0;
    if(scanf("%lu %lu", &n, &k) != 2)
    {
        return -1;
    }
    vector<int> arr(n);
    for(size_t i = 0; i < n; i++)
    {
        if(scanf("%d", arr.data() + i) != 1)
        {
            return -1;
        }
    }
    int out = ordinalStat(arr.data(), arr.size(), k);
    printf("%d", out);

    return 0;
}
