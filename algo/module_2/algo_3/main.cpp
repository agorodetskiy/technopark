/* 3_1. Реклама.
В супермаркете решили оптимизировать показ рекламы. Известно расписание прихода и ухода
покупателей (два целых числа). Каждому покупателю необходимо показать минимум 2 рекламы.
Рекламу можно транслировать только в целочисленные моменты времени. Покупатель может
видеть рекламу от момента прихода до момента ухода из магазина.
В каждый момент времени может показываться только одна реклама. Считается, что реклама
показывается мгновенно. Если реклама показывается в момент ухода или прихода, то считается,
что посетитель успел её посмотреть. Требуется определить минимальное число показов рекламы. */

#include <cstdio>
#include <vector>
#include <cassert>
#include <algorithm>

using std::vector;
typedef long long ll;

template <typename T>
class HeapWork
{
public:
    static void heapSort(T *arr, size_t l, size_t r, bool (*)(const T&, const T&));
    static T extractTopElem(T *arr, size_t len, bool (*)(const T&, const T&));
private:
    HeapWork() = delete;
    static void siftDown(T *arr, size_t len, size_t i, bool (*)(const T&, const T&));
    static void buildHeap(T*, size_t, bool (*)(const T &, const T &));
};

template <typename T>
void HeapWork<T>::siftDown(T *arr, size_t len, size_t i, bool (*cmp)(const T&, const T&)) // maxHeap - isLarger | if first arg of cmp is larger : true
{
    size_t left = 2*i + 1;
    size_t right = 2*i + 2;
    size_t alien = i;
    if(left < len && cmp(arr[left],  arr[alien]))
    {
        alien = left;
    }
    if(right < len && cmp(arr[right], arr[alien]))
    {
        alien = right;
    }
    if(alien != i)
    {
        std::swap(arr[i], arr[alien]);
        siftDown(arr, len, alien, cmp);
    }
}

template <typename T>
void HeapWork<T>::buildHeap(T *arr, size_t len, bool (*cmp)(const T&, const T&))
{
    for(ssize_t i = len / 2 - 1; i >= 0; i--)
    {
        siftDown(arr, len, i, cmp);
    }
}

template <typename T>
T HeapWork<T>::extractTopElem(T *arr, size_t len, bool (*cmp)(const T&, const T&))
{
    assert(len != 0);
    T out = arr[0];
    arr[0] = arr[len - 1];
    if(len - 1 > 1)
    {
        siftDown(arr, len - 1, 0, cmp);
    }
    return out;
}

template <typename T>
void HeapWork<T>::heapSort(T *arr, size_t l, size_t r, bool (*cmp)(const T&, const T&))
{
    assert (r >= l);
    size_t heapSize = r - l;
    buildHeap(arr + l, heapSize, cmp);
    while(heapSize > 1)
    {
        arr[heapSize - 1] = extractTopElem(arr + l, heapSize, cmp);
        heapSize--;
    }
}

struct subject
{
    ll start;
    ll end;
    subject() : start(0), end(0) {}

    static bool isLessStart(const subject& lv, const subject& rv)
    {
        return lv.start < rv.start;
    }
    static bool isLargerStart(const subject& lv, const subject& rv)
    {
        return lv.start > rv.start;
    }
    static bool isLessEnd(const subject& lv, const subject& rv)
    {
        return lv.end < rv.end;
    }
    static bool isLargerEnd(const subject& lv, const subject& rv)
    {
        return lv.end > rv.end;
    }
};

size_t evalAdvertisements(vector<subject> &sample)
{
    size_t advCount = 0;
    HeapWork<subject>::heapSort(sample.data(), 0, sample.size(), subject::isLargerEnd);
    ll lastAdvTime = 0, penultAdvTime = 0;
    for(size_t i = 0; i < sample.size(); i++)
    {
        if(advCount == 0 || sample[i].start > lastAdvTime)
        {
            lastAdvTime = sample[i].end;
            penultAdvTime = sample[i].end - 1;
            advCount += 2;
        }
        else if(sample[i].start > penultAdvTime)
        {
            if(lastAdvTime == sample[i].end)
            {
                penultAdvTime = sample[i].end - 1;
            }
            else
            {
                penultAdvTime = lastAdvTime;
                lastAdvTime = sample[i].end;
            }
            advCount++;
        }
    }
    return advCount;
}

int main()
{
    size_t n = 0;
    if(scanf("%lu", &n) != 1)
    {
        return -1;
    }
    vector<subject> input(n);
    for(size_t i = 0; i < n; i++)
    {
        if(scanf("%lld %lld", &input[i].start, &input[i].end) != 2 || input[i].start >= input[i].end)
        {
            return -1;
        }
    }
    size_t out = evalAdvertisements(input);
    printf("%lu\n", out);
    return 0;
}
