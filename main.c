#include "main.h"

#define PATH "example.sconf"
#define BUFFER_SIZE 4

int main(int argc, char** argv) {
    FILE* file = fopen(PATH, "r");
    //Verify if the file exists
    if (file == NULL) {
        fprintf(stderr, "Error #1: %s was not found", PATH);
        exit(EXIT_FAILURE);
    }

    int nbTask = countTask(file);
    int nbAction = countAction(file) - (nbTask * 2);

    if (nbAction <= 0 || nbTask <= 0) {
        fprintf(stderr, "Error #2: Not enough task or actions to execute sCrapper\n");
        fprintf(stderr, "action count: %d, task count: %d", nbAction, nbTask);
        exit(0);
    }
    action actionArray[nbAction];
    task taskArray[nbTask];
    initActionArray(actionArray, nbAction, file);
    initTaskArray(taskArray, nbTask);
    return 0;
}

//TODO initActionArray function
void initActionArray(action* actionArray, int sizeArray, FILE* file) {
    char* word = malloc(24);
    char* test = malloc(1024);
    long curPos = ftell(file);
    fseek(file, 0, SEEK_SET);
    int i;
    for (i = 0; i < sizeArray; ++i) {
        fscanf(file, "%s", word);
        while (strcmp(word, "=") != 0) {
            fscanf(file, "%s", word);
            if (strchr(word, '#') != NULL) {
                skipComment(file);
                fscanf(file, "%s", word);
            }
            if (strcmp(word, "name") == 0) {
                fscanf(file, "-> %s", test);
                printf("%s", test);
                //actionArray[i].name = "smthing";
            }
            if (strcmp(word, "url") == 0) {
                actionArray[i].url = "url";
            }
            if (strcmp(word, "max-depth") == 0) {
                actionArray[i].maxDepth = -1;
            }
            if (strcmp(word, "versionning") == 0) {
                actionArray[i].versionning = 0;
            }
        }
    }
}

//TODO initTaskArray function
void initTaskArray(task* taskArray, int sizeArray) {

}

void skipComment(FILE* file) {
    char c;
    while (c != '\n' && c != EOF) {
        c = fgetc(file);
    }
}

long sizeOfFile(FILE* file) {
    long res;
    fseek(file, 0, SEEK_END);
    res = ftell(file);
    fseek(file, 0, SEEK_SET);
    return res;
}

int countAction(FILE* file) {
    int count = countOccurrences(file, "=");
    fseek(file, 0, SEEK_SET);
    return count;
}

int countTask(FILE* file) {
    int count = countOccurrences(file, "==");
    fseek(file, 0, SEEK_SET);
    return count;
}

int countOccurrences(FILE* file, char* string) {
    char str[BUFFER_SIZE];
    char* pos;
    int i;
    int count = 0;

    //Read the file
    while ((fgets(str, BUFFER_SIZE, file)) != NULL) {
        i = 0;
        //Skip the comments
        if (fgetc(file) == '#') {
            skipComment(file);
        }
        //Find the next occurrence of the string we want
        while ((pos = strstr(str + i, string)) != NULL) {
            i = (pos - str) + 1;
            ++count;
        }
    }
    return count;
}

