/* Реализуйте функцию
char** div_format(char **s)
{
    ...
}
и программу, использующую эту функцию для форматирования текстовой разметки состоящей из <div>, </div> и ASCII строк.
Правильное форматирование подразумевает расстановку 4х пробелов согласно вложенности. То есть в исходном тексте необходимо
игнорировать все "ведущие" и "закрывающие" пробелы, и форматировать вывод строк простановкой нового соответствующего
количества пробелов. В случае некорректной последовательности тэгов, выведите в поток вывода [error]. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>

#define BUFFERSIZE 1024
#define INDENTATION 4
#define REALLOC_FACTOR 2
#define TAG_OPENING "<div>"
#define TAG_CLOSING "</div>"
#define TAG_OPENING_LEN 5
#define TAG_CLOSING_LEN 6

enum tagType
{
    opening,
    closing
};
typedef struct tag
{
    char *p_tag;
    size_t tag_len;
    enum tagType type;
} tag;

void printData(char **data, size_t strNum)
{
    for(size_t i = 0; i < strNum; i++)
    {
        fputs(data[i], stdout);
    }
}
char** inputData(size_t *strNum, size_t *capacity, int *errorCode, char **data)
{
    char buffer[BUFFERSIZE];
    for(*strNum = 0; fgets(buffer, BUFFERSIZE, stdin); (*strNum)++)
    {
        if(*strNum == *capacity)
        {
            *capacity *= REALLOC_FACTOR;
            if((data = realloc(data, *capacity * sizeof(char*))) == NULL)
            {
                *errorCode = 1;
                return data;
            }
        }

        if((data[*strNum] = (char*)malloc(BUFFERSIZE * sizeof(char))) == NULL)
        {
            *errorCode = 1;
            return data;
        }

        strcpy(data[*strNum], buffer); // no need strncpy and tricks with '\0', fgets manages this stuff
    }
    return data;
}
char** transferLine(size_t *strNum, size_t *capacity, int *errorCode, char **data, size_t strId, char *position)
{
    if(*position == '\n' || *position == '\0' || *(position + 1) == '\0')  // no need to transfer empty lines
        return data;

    if(*strNum == *capacity)
    {
        *capacity *= REALLOC_FACTOR;
        if((data = realloc(data, *capacity * sizeof(char*))) == NULL)
        {
            *errorCode = 1;
            return data;
        }
    }

    for(size_t i = *strNum; i > strId + 1; i--)
    {
        data[i] = data[i-1];
    }

    if((data[strId+1] = (char*)malloc(BUFFERSIZE * sizeof(char))) == NULL)
    {
        *errorCode = 1;
        return data;
    }

    strcpy(data[strId+1], position);
    (*strNum)++;

    *position = '\n';
    *(position + 1) = '\0';
    return data;
}
void deleteLine(size_t *strNum, char **data, size_t strId)
{
    free(data[strId]);
    for(size_t i = strId; i < *strNum - 1; i++)
    {
        data[i] = data[i+1];
    }
    (*strNum)--;
}
size_t isCharsHere(char *head, char *tail) // tail points at the char AFTER last
{
    if (head > tail)
    {
        return 0;
    }
    for(; head != tail; head++)
    {
        if(isspace(*head) == 0)
        {
            return 1;
        }
    }
    return 0;
}
tag findTag(char *haystack)
{
    char *tag1_entry = strstr(haystack, TAG_OPENING);
    char *tag2_entry = strstr(haystack, TAG_CLOSING);

    tag firstTag = {NULL, 0, opening};
    if((tag1_entry && tag2_entry && tag1_entry < tag2_entry) || (tag1_entry && tag2_entry == NULL))
    {
        firstTag.p_tag = tag1_entry;
        firstTag.tag_len = TAG_OPENING_LEN;
        firstTag.type = opening;
    }
    else if((tag1_entry && tag2_entry && tag1_entry > tag2_entry) || (tag1_entry == NULL && tag2_entry))
    {
        firstTag.p_tag = tag2_entry;
        firstTag.tag_len = TAG_CLOSING_LEN;
        firstTag.type = closing;
    }
    return firstTag;
}
void makeIndentation(char *str, int *errorCode, ssize_t nestingLvl)
{
    char buffer[BUFFERSIZE] = {'\0'};
    for(ssize_t i = 0; i < nestingLvl && strlen(buffer) + INDENTATION < BUFFERSIZE; i++)
    {
        strcat(buffer, "    ");
    }
    char *strPtr;
    for(strPtr = str; isspace(*strPtr); strPtr++);
    if(strlen(buffer) + strlen(strPtr) + 1 > BUFFERSIZE)
    {
        *errorCode = 1;
        return;
    }
    strcat(buffer, strPtr);
    strcpy(str, buffer);
}
void lineIndentation(char **data, const size_t *strNum, int *errorCode)
{
    size_t indentLvl = 0;
    ssize_t nestLvl = 0;
    for(size_t strId = 0; strId < *strNum; strId++)
    {
        tag firstTag = findTag(data[strId]);

        if(firstTag.p_tag != NULL)
        {
            firstTag.type == opening ? nestLvl++ : nestLvl--;
            if(nestLvl < 0)
            {
                *errorCode = 1;
                return;
            }
            if(firstTag.type == closing)
                indentLvl--;
            makeIndentation(data[strId], errorCode, indentLvl);
            if(firstTag.type == opening)
                indentLvl++;
        }
        else
        {
            makeIndentation(data[strId], errorCode, indentLvl);
        }
    }
    if(nestLvl != 0)
    {
        *errorCode = 1;
        return;
    }
}

char** div_format(size_t *strNum, size_t *capacity, int *errorCode, char **data)
{
    for(size_t strId = 0; strId < *strNum; strId++)
    {
        tag firstTag = findTag(data[strId]);

        if(firstTag.p_tag != NULL)
        {
            if(isCharsHere(data[strId], firstTag.p_tag) == 1) // some chars before <tag>
            {
                data = transferLine(strNum, capacity, errorCode, data, strId, firstTag.p_tag);
                if(*errorCode != 0)
                    return data;
            }
            else
            {
                data = transferLine(strNum, capacity, errorCode, data, strId, firstTag.p_tag + firstTag.tag_len);
                if(*errorCode != 0)
                    return data;
            }
        }
        else
        {
            char *strPtr;
            for(strPtr = data[strId]; isspace(*strPtr); strPtr++);
            if(*strPtr == '\0')
            {
                deleteLine(strNum, data, strId);
                strId--;
            }
        }
    }

    lineIndentation(data, strNum, errorCode);

    return data;
}
void memFree(char **data, size_t strNum)
{
    for(size_t strId = 0; strId < strNum; strId++)
    {
        free(data[strId]);
    }
    free(data);
}

int main()
{
    int errorCode = 0;
    size_t capacity = 16;
    size_t strNum = 0;

    char **data;
    if((data = (char**)calloc(capacity, sizeof(char*))) == NULL)
    {
        printf("[error]");
        memFree(data, strNum);
        return 0;
    }
    data = inputData(&strNum, &capacity, &errorCode, data);
    if(errorCode != 0)
    {
        printf("[error]");
        memFree(data, strNum);
        return 0;
    }

    data = div_format(&strNum, &capacity, &errorCode, data);
    if(errorCode != 0)
    {
        printf("[error]");
        memFree(data, strNum);
        return 0;
    }

    printData(data, strNum);

    memFree(data, strNum);

    return 0;
}
