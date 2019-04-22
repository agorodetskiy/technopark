/* 1_1. Ящики.
На склад привезли много пустых ящиков. Все ящики пронумерованы по порядку поступления от 0.
Известно, что их все можно сложить один в один (то есть так, что каждый следующий помещается
в предыдущий). Один ящик можно вложить в другой, если его можно перевернуть так, что размеры
одного ящика по всем осям станут строго меньше размеров другого ящика по соответствующим
осям. Требуется определить, в какой последовательности они будут вложены друг в друга.
Вывести номера ящиков. */

#include <cstdio>
#include <vector>

#define DIMENSIONS_COUNT 3

struct box_t
{
    size_t id;
    int dimensions[DIMENSIONS_COUNT];
    box_t () : id(0), dimensions{ 0 } {}
};

template <typename T>
void insertionSort(T *arr, size_t l, size_t r, bool (*cmp)(const T, const T))
{
    size_t size = r - l + 1;
    for(size_t k = l + 1; k < size; k++)
    {
        T tmp = arr[k];
        ssize_t i = k - 1;
        for(; i >= 0 && cmp(arr[i], tmp); i--)
        {
            arr[i+1] = arr[i];
        }
        arr[i+1] = tmp;
    }
}

bool isLess(const int a, const int b)
{
    return a < b;
}

template <size_t dimToCmp>
bool isBigger(const box_t a, const box_t b)
{
    return a.dimensions[dimToCmp] > b.dimensions[dimToCmp];
}

void sortBoxesBySize(std::vector<box_t> &boxes) //DO I ACTUALLY NEED TO USE THIS LINK??!?!?!!?!?!
{
    for(size_t i = 0; i < boxes.size(); i++)    //sort dimensions descendingly
    {
        insertionSort<int>(boxes[i].dimensions, 0, DIMENSIONS_COUNT - 1, isLess);
    }

    insertionSort<box_t>(boxes.data(), 0, boxes.size() - 1, isBigger<2>);
    insertionSort<box_t>(boxes.data(), 0, boxes.size() - 1, isBigger<1>);
    insertionSort<box_t>(boxes.data(), 0, boxes.size() - 1, isBigger<0>);
}

int main()
{
    size_t n = 0;
    scanf("%lu", &n);
    std::vector<box_t> boxes(n);
    for(size_t i = 0; i < n; i++)
    {
        boxes[i].id = i;
        scanf("%d %d %d", boxes[i].dimensions, boxes[i].dimensions + 1, boxes[i].dimensions + 2);
    }

    sortBoxesBySize(boxes);

    for(size_t i = 0; i < n; i++)
    {
        printf("%lu ", boxes[i].id);
    }
    return 0;
}
























