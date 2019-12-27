#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

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
    char** actionsName;
    int sizeActionArray;
}task;

void skipComment(FILE* file);
long sizeOfFile(FILE* file);
void initActionArray(struct action* actionArray, int sizeArray, FILE* file);
void initTaskArray(task* taskArray, int nbTask, FILE* file);
int countOccurrences(FILE* file, char* word);
void storeActions(task* taskArray, int index, FILE* file);
