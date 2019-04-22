#include <cstdio>
#include <cstring>

#define STACK_INIT_SIZE 256
#define REALLOC_FACTOR 2
#define BUFFERSIZE 20000

class Stack_t
{
public:
    Stack_t();
    ~Stack_t();
    void push(char elem);
    char pop();
    char peek();
    int getSize();
private:
    char *data;
    size_t size;
    size_t capacity;
    void realloc();
};

Stack_t::Stack_t() : size(0), capacity(STACK_INIT_SIZE)
{
    data = new char[STACK_INIT_SIZE];
}

Stack_t::~Stack_t()
{
    delete [] data;
}

void Stack_t::realloc()
{
    char *tmp = data;
    capacity *= REALLOC_FACTOR;
    data = new char[capacity];
    memcpy(data, tmp, size * sizeof(char));
    delete [] tmp;
}

void Stack_t::push(char elem)
{
    if(size == capacity)
    {
        realloc();
    }
    data[size++] = elem;
}

char Stack_t::pop()
{
    if(size == 0)
        return -1;
    return data[--size];
}

char Stack_t::peek()
{
    if(size == 0)
        return -1;
    return data[size - 1];
}

int Stack_t::getSize()
{
    return size;
}
bool isAnagramms(char *word_1, char *word_2)
{
    Stack_t stack;
    char *c1, *c2;
    bool isSuccess = false;
    for(c1 = word_1, c2 = word_2; *c2;)
    {
        if(*c1 != *c2)
        {
            if(*c2 == stack.peek())
            {
                stack.pop();
                c2++;
            }
            else
            {
                stack.push(*c1);
                c1++;
            }
        }
        else
        {
            c1++;
            c2++;
        }
    }
    if(*c1 == '\0' && *c2 == '\0' && stack.getSize() == 0)
        isSuccess = true;

    return isSuccess;
}

int main()
{
    char *word_1 = new char[BUFFERSIZE];
    char *word_2 = new char[BUFFERSIZE];
    if(scanf("%s", word_1) == 0)
        return 0;
    if(scanf("%s", word_2) == 0)
        return 0;

    bool isSuccess = isAnagramms(word_1, word_2);

    if(isSuccess == true)
        printf("YES");
    else
        printf("NO");

    delete [] word_1;
    delete [] word_2;

    return 0;
}









































