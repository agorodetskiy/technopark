#include <cstdio>
#include <cstring>
#define STACK_INIT_SIZE 16
#define REALLOC_FACTOR 2

class Stack_t
{
public:
    Stack_t();
    ~Stack_t();
    void push(int elem);
    int pop();
    int getSize() const;
private:
    int *data;
    size_t size;
    size_t capacity;
    void realloc();
};

class Queue_t
{
public:
    Queue_t() {}
    ~Queue_t() {}
    void enqueue(int elem);
    int dequeue();
    int getSize() const;
private:
    Stack_t L;
    Stack_t R;
};

struct query_t
{
    int a;
    int b;
    query_t() : a(0), b(0) {}
};

Stack_t::Stack_t() : size(0), capacity(STACK_INIT_SIZE)
{
    data = new int[STACK_INIT_SIZE];
}

Stack_t::~Stack_t()
{
    delete [] data;
}

void Stack_t::realloc()
{
    int *tmp = data;
    capacity *= REALLOC_FACTOR;
    data = new int[capacity];
    memcpy(data, tmp, size * sizeof(int));
    delete [] tmp;
}

void Stack_t::push(int elem)
{
    if(size == capacity)
    {
        realloc();
    }
    data[size++] = elem;
}

int Stack_t::pop()
{
    if(size == 0)
        return -1;
    return data[--size];
}

int Stack_t::getSize() const
{
    return size;
}

void Queue_t::enqueue(int elem)
{
    L.push(elem);
}

int Queue_t::dequeue()
{
    if(L.getSize() != 0 && R.getSize() == 0)
    {
        while(L.getSize() != 0)
        {
            R.push(L.pop());
        }
    }
    else if(R.getSize() == 0)
    {
        return -1;
    }
    return R.pop();
}

int Queue_t::getSize() const
{
    return L.getSize() + R.getSize();
}

bool isQueryValid(const query_t *queries, size_t n)
{
    Queue_t queue;
    bool isSuccess = true;
    for(size_t i = 0; i < n && isSuccess == true; i++)
    {
        if(queries[i].a == 3)
        {
            queue.enqueue(queries[i].b);
        }
        else
        {
            if(queue.dequeue() != queries[i].b)
            {
                isSuccess = false;
            }
        }
    }

    return isSuccess;
}

int main()
{
    size_t n = 0;
    if(scanf("%lu", &n) == 0)
    {
        printf("[wrong input]");
        return 0;
    }
    query_t *queries = new query_t[n];
    for(size_t i = 0; i < n; i++)
    {
        if(scanf("%d%d", &queries[i].a, &queries[i].b) != 2)
        {
            printf("[wrong inpur]");
            return 0;
        }
    }

    bool isSuccess = isQueryValid(queries, n);

    isSuccess == true ? printf("YES") : printf("NO");

    delete [] queries;
    return 0;
}
