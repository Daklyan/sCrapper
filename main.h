#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

typedef char* string;

typedef struct action{
    string name;
    string url;
    int maxDepth;
    int versionning;
    string* type;
}action;

typedef struct task{
    string name;
    short sec;
    short min;
    short hour;
    string* actionsName;
    int sizeActionArray;
}task;

void skipComment(FILE* file);
long sizeOfFile(FILE* file);
void initActionArray(struct action* actionArray, int sizeArray, FILE* file);
void initTaskArray(task* taskArray, int nbTask, FILE* file);
int countOccurrences(FILE* file, char* word);
void storeActions(task* taskArray, int index, FILE* file);
