#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <ctime>
#include <cstring>
#include <vector>
//#include <stack>
using std::vector;
//using std::stack;
typedef int_fast32_t intf;

#define STACK_INIT_SIZE 30
#define REALLOC_FACTOR 2

struct range
{
    intf left;
    intf right;

    range() : left(0), right(0) {}
    range(intf left, intf right) : left(left), right(right) {}
};

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
    size_t heapSize = r - l;
    buildHeap(arr + l, heapSize, cmp);
    while(heapSize > 1)
    {
        arr[heapSize - 1] = extractTopElem(arr + l, heapSize, cmp);
        heapSize--;
    }
}

template <typename T>
class Stack_t
{
public:
    Stack_t();
    ~Stack_t();
    void push(T elem);
    T pop();
    T peek();
    intf getSize();
private:
    T *data;
    intf size;
    intf capacity;
    void realloc();
};

template <typename T>
Stack_t<T>::Stack_t() : size(0), capacity(STACK_INIT_SIZE)
{
    data = new T[STACK_INIT_SIZE];
}

template <typename T>
Stack_t<T>::~Stack_t()
{
    delete [] data;
}

template <typename T>
void Stack_t<T>::realloc()
{
    T *tmp = data;
    capacity *= REALLOC_FACTOR;
    data = new T[capacity];
    memcpy(data, tmp, size * sizeof(T));
    delete [] tmp;
}

template <typename T>
void Stack_t<T>::push(T elem)
{
    if(size == capacity)
    {
        realloc();
    }
    data[size++] = elem;
}

template <typename T>
T Stack_t<T>::pop()
{
    if(size == 0)
    {
        range out(0, 0);
    }
    return data[--size];
}

template <typename T>
T Stack_t<T>::peek()
{
    if(size == 0)
        return -1;
    return data[size - 1];
}

template <typename T>
intf Stack_t<T>::getSize()
{
    return size;
}

bool get_int(intf &x)
{
    register intf c = getchar_unlocked();

    for(; (c < 48 || c > 57); c = getchar_unlocked())
    {
        if(c == EOF)
        {
            return false;
        }
    }

    x = 0;
    for(; c > 47 && c < 58; c = getchar_unlocked())
    {
        x = (x<<1) + (x<<3) + c - 48;
    }

    return true;
}

template <typename T>
void insertionSort(T *arr, intf l, intf r) // limits inclusive
{
    for(intf k = l + 1; k <= r; k++)
    {
        T tmp = arr[k];
        intf i = k - 1;
        for(; i >= l && arr[i] > tmp; i--)
        {
            arr[i+1] = arr[i];
        }
        arr[i+1] = tmp;
    }
}

template <typename T>
intf median(T *arr, intf left, intf right)
{
    intf l = left, m = left + (right - left) / 2, r = right;
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

template <typename T>
intf medianOfMedians(T *arr, intf left, intf right)
{
    intf len = right - left + 1;
    if(len < 30)
    {
        insertionSort(arr, left, right);
        return left + (right - left) / 2;
    }
    intf chunks = len / 5;
    for(intf i = 0; i < chunks; i++)
    {
        insertionSort(arr, left + 5*i,  left + 5*(i+1) - 1);
    }
    for(intf i = 0; i < chunks; i++)
    {
        std::swap(arr[left + i], arr[left + 2 + 5*i]);
    }
    medianOfMedians(arr, left, left + chunks - 1);
}

template <typename T>
intf partition(T *arr, intf left, intf right) // borders inclusive
{
    intf pivot = median(arr, left, right);

    std::swap(arr[pivot], arr[right]);
    pivot = right;
    intf i, j;
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

template <typename T>
intf partition_converging(T *arr, intf left, intf right) // borders inclusive
{
    intf pivot = median(arr, left, right);

    std::swap(arr[pivot], arr[right]);
    pivot = right;
    intf i, j;
    for(i = left, j = right - 1; i <= j;)
    {
        for(; arr[i] < arr[pivot]; i++);
        for(; j >= 0 && arr[j] >= arr[pivot]; j--);
        if(i < j)
        {
            std::swap(arr[i++], arr[j--]);
        }
    }
    std::swap(arr[i], arr[right]);
    return i;
}

template <typename T>
void partition_converging_v2(T *arr, intf left, intf right, intf &ret_i, intf &ret_j) // borders inclusive
{
    intf pivot = median(arr, left, right);

    std::swap(arr[pivot], arr[right]);
    pivot = right;
    intf i, j, p, q;
    for(i = left, j = right - 1, p = left-1, q = right; ;)
    {
        for(; arr[i] < arr[pivot]; i++);
        for(; j > left && arr[j] > arr[pivot]; j--);
        if(i >= j)
        {
            break;
        }
        std::swap(arr[i], arr[j]);
        if(arr[i] == arr[pivot])
        {
            p++;
            std::swap(arr[p], arr[i]);
        }
        if(arr[j] == arr[pivot])
        {
            q--;
            std::swap(arr[q], arr[j]);
        }
    }
    std::swap(arr[i], arr[right]);
    j = i + 1;
    i--;
    for(intf r = left; r <= p; r++, i--)
    {
        std::swap(arr[r], arr[i]);
    }
    for(intf r = right - 1; r >= q; r--, j++)
    {
        std::swap(arr[r], arr[j]);
    }
    ret_i = i;
    ret_j = j;
}

bool cmp(const intf &a, const intf &b)
{
    return a > b;
}

template <typename T>
void quickSort(T *arr, intf left, intf right)
{
    Stack_t<range> stack;
    stack.push(range(left, right));
    while(stack.getSize() != 0)
    {
        range lim = stack.pop();

        if(lim.left >= lim.right)
        {
            continue;
        }
        if(lim.right - lim.left < 28) // 28
        {
            insertionSort(arr, lim.left, lim.right);
            //HeapWork<intf>::heapSort(arr, lim.left, lim.right, cmp);
            continue;
        }
        intf pivot = partition_converging(arr, lim.left, lim.right);
        range toPush;
        if(pivot - 1 > lim.right - pivot)
        {
           toPush.left = lim.left;
           toPush.right = pivot - 1;
           stack.push(toPush);

           toPush.left = pivot + 1;
           toPush.right = lim.right;
           stack.push(toPush);
        }
        else
        {
            toPush.left = pivot + 1;
            toPush.right = lim.right;
            stack.push(toPush);

            toPush.left = lim.left;
            toPush.right = pivot - 1;
            stack.push(toPush);
        }
    }
}

template <typename T>
void quickSort_v2(T *arr, intf left, intf right)
{
    Stack_t<range> stack;
    stack.push(range(left, right));
    while(stack.getSize() != 0)
    {
        range lim = stack.pop();

        if(lim.left >= lim.right)
        {
            continue;
        }
        /*if(lim.right - lim.left < 28) // 28
        {
            insertionSort(arr, lim.left, lim.right);
            continue;
        }*/
        intf ret_i = 0, ret_j = 0;
        partition_converging_v2(arr, lim.left, lim.right, ret_i, ret_j);
        range toPush;
        if(ret_j - lim.left > lim.right - ret_i)
        {
           toPush.left = lim.left;
           toPush.right = ret_j;
           stack.push(toPush);

           toPush.left = ret_i;
           toPush.right = lim.right;
           stack.push(toPush);
        }
        else
        {
            toPush.left = ret_i;
            toPush.right = lim.right;
            stack.push(toPush);

            toPush.left = lim.left;
            toPush.right = ret_j;
            stack.push(toPush);
        }
    }
}

template <typename T>
void quickSort_recursive(T *arr, intf len)
{
    if(len < 100)
    {
        insertionSort(arr, 0, len);
        return;
    }
    intf pivot = partition(arr, 0, len - 1);
    if( pivot > 0 ) quickSort_recursive(arr, pivot);
    if( pivot + 1 < len ) quickSort_recursive(arr + pivot + 1, len - (pivot + 1));
}



int main()
{
//#define MAKE_SAMPLE
#ifdef MAKE_SAMPLE
    freopen("sample", "w", stdout);
    for(size_t i = 0; i < 0x17D7840; i++)
    {
        printf("%d\n", std::rand());
    }
#endif

#define DEATH_TO_BUGS
#ifdef DEATH_TO_BUGS

    /*intf size = 0x17D7840;
    intf *data = (intf*)malloc(size * sizeof(intf));
    freopen("sample", "r", stdin);
    for(intf i = 0; i < size; i++)
    {
        scanf("%ld", &data[i]);
    }*/
    vector<intf> data(20);
    size_t size = data.size();
    for(size_t i = 0; i < size; i++)
    {
        data[i] = std::rand() % size;
    }


#else
    intf *data = (intf*)malloc(0x17D7840 * sizeof(intf));
    intf size = 0;
    intf buff = 0;
    for(intf i = 0; get_int(buff); i++)
    {
        data[i] = buff;
        size++;
    }
#endif

    clock_t start_clock = std::clock();

    quickSort_v2(data.data(), 0, size - 1);

    clock_t end_clock = std::clock();
    clock_t time = end_clock - start_clock;

    for(intf i = 9; i < size; i += 10)
    {
        printf("%ld ", data[i]);
    }

    //delete[] data;
    return 0;
}

































