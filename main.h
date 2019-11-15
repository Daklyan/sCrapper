#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct action{
    char* name;
    char* url;
    int maxDepth;
    int versionning;
    char** type;
}action;

typedef struct task{
    char* name;
    short sec;
    short min;
    short hour;
    action* actionArray;
}task;

void skipComment(FILE* file);
long sizeOfFile(FILE* file);
int countAction(FILE* file);
int countTask(FILE* file);
void initActionArray(struct action* actionArray, int sizeArray, FILE* file);
void initTaskArray(struct task* taskArray, int sizeArray, FILE* file);
int countOccurrences(FILE* file, char* word);
void freeActionArray(action* array, int sizeArray);
void freeTaskArray(task* array, int sizeArray);

