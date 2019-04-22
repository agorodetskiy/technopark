/* 2_4. Скользящий максимум.
Дан массив натуральных чисел A[0..n), n не превосходит 10^8. Так же задан размер некотрого окна
(последовательно расположенных элементов массива) в этом массиве k, k<=n. Требуется для
каждого положения окна (от 0 и до n-k) вывести значение максимума в окне. Скорость работы O(nlog n), память O(k). */

#include <cstdio>
#include <vector>
#include <cstring>
#include <cassert>

using std::vector;

struct elem_t
{
    size_t id;
    int value;
    elem_t() : id(0), value(0) {}
    bool operator< (const elem_t &rv) const
    {
        return this->value < rv.value;
    }
    bool operator> (const elem_t &rv) const
    {
        return this->value > rv.value;
    }
};

template <typename T>
class BinaryHeap_t
{
public:
    BinaryHeap_t(T *data, size_t len) : buffer(data, data + len) { this->buildHeap(); }

    void addElem(T& data);
    T getMax();
    T extractMax();
private:
    vector<T> buffer;

    void siftDown(size_t i);
    void siftUp(size_t i);
    void buildHeap();
};

template <typename T>
void BinaryHeap_t<T>::siftDown(size_t i)
{
    size_t left = 2*i + 1;
    size_t right = 2*i + 2;
    size_t largest = i;
    if(left < buffer.size() && buffer[left] > buffer[largest])
    {
        largest = left;
    }
    if(right < buffer.size() && buffer[right] > buffer[largest])
    {
        largest = right;
    }
    if(largest != i)
    {
        std::swap(buffer[i], buffer[largest]);
        siftDown(largest);
    }
}

template <typename T>
void BinaryHeap_t<T>::siftUp(size_t i)
{
    while(i > 0)
    {
        size_t parent = (i - 1) / 2;
        if( !(buffer[i] > buffer[parent]) )
        {
            return;
        }
        std::swap(buffer[i], buffer[parent]);
        i = parent;
    }
}

template <typename T>
void BinaryHeap_t<T>::buildHeap()
{
    for(ssize_t i = buffer.size() / 2 - 1; i >= 0; i--)
    {
        siftDown(i);
    }
}

template <typename T>
void BinaryHeap_t<T>::addElem(T& data)
{
    buffer.push_back(data);
    siftUp(buffer.size() - 1);
}

template <typename T>
T BinaryHeap_t<T>::getMax()
{
    assert(buffer.size() != 0);
    return buffer[0];
}

template <typename T>
T BinaryHeap_t<T>::extractMax()
{
    assert(buffer.size() != 0);
    T out = buffer[0];
    buffer[0] = buffer.back();
    buffer.pop_back();
    if(buffer.size() != 0)
    {
        siftDown(0);
    }
    return out;
}

vector<int> findMaxElems(vector<elem_t> &input, size_t n, size_t k)
{
    vector<int> out(n - k + 1);
    BinaryHeap_t<elem_t> gap(input.data(), k);
    out[0] = gap.getMax().value;
    for(size_t i = 1; i <= n - k; i++)
    {
        gap.addElem(input[i+k-1]);
        elem_t max = gap.getMax();
        while (max.id < i)
        {
            max = gap.extractMax();
        }
        out[i] = max.value;
    }
    return out;
}

int main()
{
    size_t n = 0, k = 0;
    scanf("%lu", &n);
    vector<elem_t> input(n);
    for(size_t i = 0; i < n; i++)
    {
        input[i].id = i;
        scanf("%d", &input[i].value);
    }
    scanf("%lu", &k);

    vector<int> out( findMaxElems(input, n, k) );
    for(size_t i = 0; i < out.size(); i++)
    {
        printf("%d ", out[i]);
    }
    return 0;
}
