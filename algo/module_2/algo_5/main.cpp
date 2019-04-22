/* Дана последовательность целых чисел из диапазона (-10^9 .. 10^9). Длина последовательности не
больше 10^6. Числа записаны по одному в строке. Количество чисел не указано.
Пусть количество элементов n, и числа записаны в массиве a = a[i]: i из [0..n-1].
Требуется напечатать количество таких пар индексов (i,j) из [0..n-1], что (i < j и a[i] > a[j]). */

#include <cstdio>
#include <vector>
using std::vector;

static long long inversionsCount = 0;

template <typename T>
class MergeSort
{
public:
    MergeSort() = delete;
    static void exec(T*, size_t, size_t);
private:
    static void merge(T*, size_t, size_t, size_t);
};

template<typename T>
void MergeSort<T>::merge(T *arr, size_t p, size_t q, size_t r)
{
    size_t n_left = q - p + 1;
    size_t n_right = r - q;
    size_t len = n_left + n_right;
    vector<T> left(arr + p, arr + q+1);
    vector<T> right(arr + q+1, arr + r+1);
    size_t i = 0, j = 0;
    for(size_t k = 0; k < len; k++)
    {
        if(i < left.size() && j < right.size())
        {
            if(left[i] <= right[j])
            {
                arr[p+k] = left[i++];
            }
            else
            {
                arr[p+k] = right[j++];
                inversionsCount += left.size() - i;
            }
        }
        else if(i < left.size())
        {
            arr[p+k] = left[i++];
        }
        else
        {
            arr[p+k] = right[j++];
        }
    }
}

template<typename T>
void MergeSort<T>::exec(T* arr, size_t p, size_t r)
{
    if(p >= r)
        return;

    size_t q = p + (r - p) / 2;
    exec(arr, p, q);
    exec(arr, q+1, r);
    merge(arr, p, q, r);
}

int main()
{
    vector<int> sample;
    int tmp = 0;
    while (scanf("%d", &tmp) == 1)
    {
        sample.push_back(tmp);
    }
    MergeSort<int>::exec(sample.data(), 0, sample.size() - 1);
    printf("%lld", inversionsCount);

    return 0;
}
