#include "main.h"

#define PATH "/home/void/CLionProjects/sCrapper/exConf.sconf"
#define BUFFER_SIZE 4

int main(int argc, char **argv) {
    FILE *file = fopen(PATH, "r");
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

//TODO initActionArray function, removing the last character may provoke a SIGSEGV core dumped
void initActionArray(action *actionArray, int sizeArray, FILE *file) {
    char *word = malloc(24);
    char *tmp = malloc(1024);
    long curPos = ftell(file);
    fseek(file, 0, SEEK_SET);
    int i;
    for (i = 0; i < sizeArray; ++i) {
        fscanf(file, "%s", word);
        while (strcmp(word, "=") != 0) {
            fscanf(file, "%s", word);
            // printf("%s",word);
            if (strchr(word, '#') != NULL) {
                skipComment(file);
                fscanf(file, "%s", word);
            }
            if (strcmp(word, "{name") == 0) {
                fscanf(file, " -> %s", tmp);
                tmp[strlen(tmp) - 1] = '\0';
                actionArray[i].name = tmp;
            }
            if (strcmp(word, "{url") == 0) {
                fscanf(file, " -> %s", tmp);
                tmp[strlen(tmp) - 1] = '\0';
                actionArray[i].url = tmp;
            }
            if (strcmp(word, "{max-depth") == 0) {
                fscanf(file, " -> %s", tmp);
                printf("%s", tmp);
                actionArray[i].maxDepth = atoi(tmp);
            }
            if (strcmp(word, "{versioning") == 0) {
                fscanf(file, " -> %s", tmp);
                printf("%s", tmp);
                actionArray[i].versioning = atoi(tmp);
            }
        }
    }
}

void initTaskArray(task *taskArray, int sizeArray, FILE *file) {
    char *word = malloc(24);
    char *tmp = malloc(1024);
    long curPos = ftell(file);
    fseek(file, 0, SEEK_SET);
    int i;
    for (i = 0; i < sizeArray; ++i) {
        fscanf(file, "%s", word);
        while (strcmp(word, "==") != 0 && !feof(file)) {
            fscanf(file, "%s", word);
            if (strchr(word, '#') != NULL) {
                skipComment(file);
                fscanf(file, "%s", word);
            }
            if (strcmp(word, "name") == 0) {
                fscanf(file, " -> %s", tmp);
                tmp[strlen(tmp) - 1] = '\0';
                taskArray[i].name = tmp;
            }
            if (strcmp(word, "hour") == 0) {
                fscanf(file, " -> %s", tmp);
                taskArray[i].hour = atoi(tmp);
            }
            if (strcmp(word, "minute") == 0) {
                fscanf(file, " -> %s", tmp);
                taskArray[i].min = atoi(tmp);
            }
            if (strcmp(word, "second") == 0) {
                fscanf(file, " -> %s", tmp);
                taskArray[i].sec = atoi(tmp);
            }
        }
    }
    fseek(file, curPos, SEEK_SET);
}

void skipComment(FILE *file) {
    char c;
    while (c != '\n' && c != EOF) {
        c = fgetc(file);
    }
}

long sizeOfFile(FILE *file) {
    long res;
    fseek(file, 0, SEEK_END);
    res = ftell(file);
    fseek(file, 0, SEEK_SET);
    return res;
}

int countAction(FILE *file) {
    int count = countOccurrences(file, "=");
    fseek(file, 0, SEEK_SET);
    return count;
}

int countTask(FILE *file) {
    int count = countOccurrences(file, "==");
    fseek(file, 0, SEEK_SET);
    return count;
}

int countOccurrences(FILE *file, char *string) {
    char str[BUFFER_SIZE];
    char *pos;
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

