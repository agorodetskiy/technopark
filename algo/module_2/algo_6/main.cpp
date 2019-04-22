/* 6_2. LSD для long long.
Дан массив неотрицательных целых 64-битных чисел. Количество чисел не больше 10^6
Отсортировать массив методом поразрядной сортировки LSD по байтам. */

#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
using std::vector;
typedef unsigned long long ull_t;
typedef unsigned char byte_t;

template <typename T>
size_t findMax(T *arr, size_t n)
{
    size_t max = 0;
    for(size_t i = 1; i < n; i++)
    {
        if(arr[i] > arr[max])
        {
            max = i;
        }
    }
    return max;
}

unsigned char getByte(ull_t num, size_t byte_id) // how many bits in byte?
{
    unsigned char a = (unsigned char)(num >> 8 * byte_id);
    return a;
}

void countingSort(ull_t *arr, size_t n, size_t byte_id)
{
    vector<byte_t> features(n);
    for(size_t i = 0; i < n; i++)
    {
        features[i] = getByte(arr[i], byte_id);
    }

    size_t k = features[ findMax(features.data(), n) ] + 1;
    vector<ssize_t> inclusions(k, 0);
    for(size_t i = 0; i < n; i++)
    {
        inclusions[features[i]]++;
    }

    inclusions[0]--;
    for(size_t i = 1; i < k; i++)
    {
        inclusions[i] += inclusions[i-1];
    }

    vector<ull_t> out(n, 0);
    for(ssize_t i = n - 1; i >= 0; i--)
    {
        out[inclusions[features[i]]--] = arr[i];
    }
    memcpy(arr, out.data(), n * sizeof(ull_t));
}

void lsd(ull_t *arr, size_t n)
{
    for(size_t i = 0; i < sizeof(ull_t); i++)
    {
        countingSort(arr, n, i);
    }
}

int main()
{
    size_t n = 0;
    if(scanf("%lu", &n) != 1)
    {
        return -1;
    }
    vector<ull_t> arr(n);
    for(size_t i = 0; i < n; i++)
    {
        if(scanf("%llu", arr.data() + i) != 1)
        {
            return -1;
        }
    }

    lsd(arr.data(), n);

    for(size_t i = 0; i < n; i++)
    {
        printf("%llu ", arr[i]);
    }

    return 0;
}
