/* 7_2. Заявки на переговоры.
В большой IT-фирме есть только одна переговорная комната. Желающие посовещаться заполняют заявки с
желаемым временем начала и конца. Ваша задача определить максимальное количество заявок, которое
может быть удовлетворено.
Число заявок ≤ 100000.
Формат входных данных:
Вход содержит только пары целых чисел — начала и концы заявок.
Формат выходных данных:
Выход должен содержать натуральное число — максимальное число заявок. */

#include <cstdio>
#include <cstring>
#define BUFFERSIZE 100000

struct ticket_t
{
    int start;
    int end;
};

class Data
{
public:
    Data();
    ~Data();
    size_t getSize();
    void addElem(int a, int b);
    void sortByEnd(size_t, size_t);
    ticket_t getElem(size_t id);
private:
    void swap(size_t, size_t);
    void merge(size_t, size_t, size_t);
    ticket_t *arr;
    size_t size;
};

Data::Data() : size(0)
{
    arr = new ticket_t[BUFFERSIZE];
}
Data::~Data()
{
    delete [] arr;
}

size_t Data::getSize()
{
    return size;
}
void Data::addElem(int a, int b)
{
    arr[size].start = a;
    arr[size].end = b;
    size++;
}
void Data::swap(size_t id_1, size_t id_2)
{
    ticket_t tmp = arr[id_1];
    arr[id_1] = arr[id_2];
    arr[id_2] = tmp;
}
void Data::merge(size_t p, size_t q, size_t r)
{
    size_t n_1 = q - p + 1;
    size_t n_2 = r - q;
    ticket_t *L = new ticket_t[n_1];
    ticket_t *R = new ticket_t[n_2];
    memcpy(L, arr + p, n_1 *sizeof(ticket_t));
    memcpy(R, arr + q + 1, n_2 * sizeof(ticket_t));
    size_t i = 0, j = 0;

    for(size_t k = 0; k < n_1 + n_2; k++)
    {
        if(i < n_1 && j < n_2)
        {
            if(L[i].end < R[j].end)
            {
                arr[p+k] = L[i];
                i++;
            }
            else
            {
                arr[p+k] = R[j];
                j++;
            }
        }
        else if(i < n_1)
        {
            arr[p+k] = L[i];
            i++;
        }
        else
        {
            arr[p+k] = R[j];
            j++;
        }
    }
    delete [] L;
    delete [] R;
}

void Data::sortByEnd(size_t p, size_t r)
{
    if(p < r)
    {
        size_t q = (p + r) / 2;
        sortByEnd(p, q);
        sortByEnd(q + 1, r);
        merge(p, q, r);
    }
}

ticket_t Data::getElem(size_t id)
{
    return arr[id];
}

size_t findMaxTicketCount(Data *tickets)
{
    tickets->sortByEnd(0, tickets->getSize() - 1);

    size_t maxCount = 0;
    int cutOff = -1;
    for(size_t i = 0; i < tickets->getSize(); i++)
    {
        ticket_t tmp = tickets->getElem(i);
        if(tmp.start >= cutOff)
        {
            maxCount++;
            cutOff = tmp.end;
        }
    }
    return maxCount;
}

int main()
{
    Data tickets;
    int a, b;
    for(size_t i = 0; scanf("%d%d", &a, &b) == 2; i++)
    {
        tickets.addElem(a, b);
    }

    size_t maxCount = findMaxTicketCount(&tickets);

    printf("%lu", maxCount);

    return 0;
}
