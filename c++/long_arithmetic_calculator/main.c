#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <string.h>

#define NUMSIZE 256
#define BUFFERSIZE 1024
#define BASE 10
#define STACK_INIT_SIZE 16
#define REALLOC_FACTOR 2
#define UNARY_MINUS_REPLACEMENT !
#define type_t int

typedef struct longNum
{
    int arr[NUMSIZE];
    size_t len;
    int sign;
} longNum;
typedef struct stack_t
{
    type_t *data;
    size_t size;
    size_t capacity;
} stack_t;
stack_t initStack(int *errorCode)
{
    stack_t tmp = {NULL, 0, STACK_INIT_SIZE};
    if((tmp.data = (type_t*)calloc(STACK_INIT_SIZE, sizeof(type_t))) == NULL)
    {
        *errorCode = 1;
    }
    return tmp;
}
void freeStack(stack_t *stack)
{
    free(stack->data);
}
void stack_push(stack_t *stack, int *errorCode, type_t elem)
{
    if(stack->size >= stack->capacity)
    {
        stack->capacity *= REALLOC_FACTOR;
        if((stack->data = realloc(stack->data, stack->capacity)) == NULL)
        {
            *errorCode = 1;
            return;
        }
    }
    stack->data[stack->size] = elem;
    stack->size++;
}
type_t stack_pop(stack_t *stack, int *errorCode)
{
    if(stack->size == 0)
    {
        *errorCode = 1;
        return 0;
    }
    stack->size--;
    return stack->data[stack->size];
}
type_t stack_peek(stack_t *stack, int *errorCode)
{
    if(stack->size == 0)
    {
        *errorCode = 1;
        return 0;
    }

    return stack->data[stack->size - 1];
}

longNum getLongNum(const char *src, int *errorCode)
{
    longNum tmp = {{0}, 0, 0};
    const char *head = src;
    if(*head == '-')
    {
        tmp.sign = 1;
        head++;
    }
    const char *tail;
    for(tail = head; isdigit(*(tail+1)); tail++);
    for(size_t i = 0; tail >= head; tail--, i++)
    {
        tmp.arr[i] = *tail - '0';
        tmp.len++;
    }
    if(tmp.len > 1 && tmp.arr[tmp.len - 1] == 0)
    {
        *errorCode = 1;
    }
    return tmp;
}
void printLongNum(const longNum src)
{
    if(src.sign == 1)
    {
        printf("-");
    }
    for(ssize_t i = src.len - 1; i >= 0; i--)
    {
        printf("%d", src.arr[i]);
    }
}
int compareAbs(const longNum num_1, const longNum num_2)
{
    if(num_1.len > num_2.len)
    {
        return 1;
    }
    else if(num_1.len < num_2.len)
    {
        return -1;
    }
    for(ssize_t i = num_1.len - 1; i >= 0; i--)
    {
        if(num_1.arr[i] > num_2.arr[i])
        {
            return 1;
        }
        else if(num_1.arr[i] < num_2.arr[i])
        {
            return -1;
        }
    }
    return 0;
}
long long longNumToLongLong(longNum num, size_t leftBorder, size_t rightBorder)
{
    long long result = 0;
    long long powerOfBase = 1;
    for(;leftBorder <= rightBorder; leftBorder++)
    {
        result += num.arr[leftBorder] * powerOfBase;
        powerOfBase *= BASE;
    }
    return result;
}
size_t longLongToLongNum(longNum *dst, long long src, size_t tail)
{
    for(;src != 0; src /= BASE, tail++)
    {
        dst->arr[tail] = src % BASE;
    }
    return tail;
}
longNum add(longNum num_1, longNum num_2)
{
    longNum tmp;
    size_t carry = 0;
    tmp.len = num_1.len > num_2.len ? num_1.len : num_2.len;
    for(size_t i = 0; i < tmp.len || carry; i++)
    {
        if(i >= tmp.len)
        {
            tmp.len++;
        }
        tmp.arr[i] = (i < num_1.len ? num_1.arr[i] : 0) +
                     (i < num_2.len ? num_2.arr[i] : 0) + carry;
        if(tmp.arr[i] >= BASE)
        {
            tmp.arr[i] -= BASE;
            carry = 1;
        }
        else
        {
            carry = 0;
        }
    }
    return tmp;
}
longNum subtract(longNum num_1, longNum num_2)
{
    longNum tmp;
    size_t carry = 0;
    for(size_t i = 0; i < num_1.len || carry; i++)
    {
        tmp.arr[i] = num_1.arr[i] - (i < num_2.len ? num_2.arr[i] : 0) - carry;
        if(tmp.arr[i] < 0)
        {
            tmp.arr[i] += BASE;
            carry = 1;
        }
        else
        {
            carry = 0;
        }
    }

    for(tmp.len = num_1.len; tmp.len > 1 && tmp.arr[tmp.len - 1] == 0; tmp.len--); // delete leading zeroes EXCEPT first

    return tmp;
}
longNum makeSubtract(longNum num_1, longNum num_2)
{
    longNum tmp;
    if(num_1.sign == 1 && num_2.sign == 1)
    {
        tmp = makeSubtract(num_2, num_1);
        tmp.sign = compareAbs(num_2, num_1) == -1;
    }
    else if(num_1.sign == 1 && num_2.sign == 0)
    {
        tmp = add(num_1, num_2);
        tmp.sign = 1;
    }
    else if(num_1.sign == 0 && num_2.sign == 1)
    {
        tmp = add(num_1, num_2);
        tmp.sign = 0;
    }
    else if(compareAbs(num_1, num_2) == -1)
    {
        tmp = makeSubtract(num_2, num_1);
        tmp.sign = 1;
    }
    else
    {
        tmp = makeSubtract(num_1, num_2);
        tmp.sign = 0;
    }

    return tmp;
}
longNum multiply(longNum num_1, longNum num_2, int *errorCode) // num_1.len must be >= num_2.len
{
    longNum result = {{0}, 0, 0};
    if(num_1.len + num_2.len >= NUMSIZE)
    {
        *errorCode = 1;
        return result;
    }
    for(size_t j = 0; j < num_2.len; j++)
    {
        size_t carry = 0;
        for(size_t i = 0; i < num_1.len || carry; i++)
        {
            size_t tmp = (i < num_1.len ? num_2.arr[j] * num_1.arr[i] : 0) + carry;
            result.arr[j+i] += tmp % BASE;
            carry = tmp / BASE;
        }
    }
    for(result.len = num_1.len + num_2.len; result.len > 1 && result.arr[result.len - 1] == 0; result.len--);

    return result;
}
longNum makeMultiply(longNum num_1, longNum num_2, int *errorCode)
{
    longNum tmp;
    if(compareAbs(num_1, num_2) == -1)
    {
        tmp = multiply(num_2, num_1, errorCode);
    }
    else
    {
        tmp = multiply(num_1, num_2, errorCode);
    }
    if(num_1.sign + num_2.sign == 1)
    {
        tmp.sign = 1;
    }
    else
    {
        tmp.sign = 0;
    }

    return tmp;
}
/*long long findIntFactor(long long dividend, long long divisor) // contract: dividend % divisor != 0
{
    size_t left = 0, right = dividend;
    long long result = 0;
    while (left < right)
    {
        long long mid = 1+ left + (right - left) / 2; // to avoid overflow, "+1" to make left-hand search

        if(mid * divisor < dividend)
        {
            left = mid;
            result = mid;
        }
        else
        {
            right = mid - 1;
        }
    }
    return result;
}*/
longNum divide(longNum dividend, long long divisor, int *errorCode) // dividend.len must be > divisor.len
{
    longNum result = {{0}, 0, 0};
    if(divisor == 0)
    {
        *errorCode = 1;
        return result;
    }

    size_t head = dividend.len - 1;
    long long incompleteQuotient = 0, remainder = 0;
    for(ssize_t i = head; i >= 0; i--)
    {
        incompleteQuotient = longNumToLongLong(dividend, i, head);

        if(incompleteQuotient > divisor)
        {
            result.arr[result.len] = incompleteQuotient / divisor;
            if((remainder = incompleteQuotient % divisor) != 0)
            {
                head = longLongToLongNum(&dividend, remainder, i);
            }
            else
            {
                head = i - 1;
            }
            result.len++;
        }
    }
    return result;
}
void replaceUnaryMinus(char* str)
{
    for(char *c = str; *c != '\0'; c++)
    {
        if(*c == '-' && (c == str || (c != str && *(c-1) == '(')))
        {
            *c = '!';
        }
    }
}
void makeReverseNotation(char *dst, char *src, int *errorCode)
{
    stack_t stack = initStack(errorCode);
    for(char *c = src; *c != '\0'; c++)
    {
        if(isdigit(*c)) //digit
        {
            char *head = c;
            for(;isdigit(*(c+1)); c++);
            strncat(dst, src, c - head + 1);
        }
        else if(*c == '*' || *c == '/')
        {
            type_t top_elem = stack_peek(&stack, errorCode);
            while(top_elem == '*' || top_elem == '/')
            {
                top_elem = stack_peek(&stack, errorCode);
                strncat(dst, &(char)stack_pop(&stack, errorCode), 1);
            }
            stack_push(&stack, errorCode, *c);
        }
        else if(*c == '+' || *c == '-' || )
        {

        }
        else if(*c == '(')
        {
            stack_push(&stack, errorCode, *c);
        }
        else if(*c == ')')
        {
            for(char oper; (oper = stack_pop(&stack, errorCode)) != ')'; c++)
            {
                *c = oper;
            }
        }
    }
}

int main()
{
    int errorCode = 0;
    char inputStr[BUFFERSIZE];
    if(fgets(inputStr, BUFFERSIZE, stdin) == NULL)
    {
        printf("[error]");
        return 0;
    }
    replaceUnaryMinus(inputStr);


    //longNum tmp1 = getLongNum(inputStr, &errorCode);
    if(errorCode != 0)
    {
        printf("[error]");
        return 0;
    }

    return 0;
}
