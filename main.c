#include "main.h"

#define PATH "example.sconf"
#define BUFFER_SIZE 50

int main(int argc, char** argv) {
    FILE* file = fopen(PATH, "r");
    //Verify if the file exists
    if (file == NULL) {
        fprintf(stderr, "Error #1: %s was not found", PATH);
        exit(EXIT_FAILURE);
    }

    int nbAction = countOccurrences(file, "=");
    int nbTask = countOccurrences(file, "==");

    if (nbAction <= 0 || nbTask <= 0) {
        fprintf(stderr, "Error #2: Not enough task or actions to execute sCrapper\n");
        fprintf(stderr, "action count: %d, task count: %d", nbAction, nbTask);
        exit(EXIT_FAILURE);
    }
    action actionArray[nbAction];
    task taskArray[nbTask];
    initActionArray(actionArray, nbAction, file);
    initTaskArray(taskArray, nbTask, file);

    //Free everything
    fclose(file);
    freeActionArray(actionArray, nbAction);
    freeTaskArray(taskArray, nbTask);
    return 0;
}


/**
 * Function to initialize the array of action following the values of the sconf
 * @param actionArray Array of structures action
 * @param sizeArray
 * @param file sconf file
 */
void initActionArray(action* actionArray, int sizeArray, FILE* file) {
    char* word = malloc(BUFFER_SIZE);
    char* tmp = malloc(256); // 256 in case the URL is long
    long curPos = ftell(file);
    fseek(file, 0, SEEK_SET);
    fscanf(file, "%s", word);
    int i = 0;
    while (!feof(file) && i < sizeArray) {  //Read all the file until it's the end of the file or i > the number of actions counted before
        if (strcmp(word, "=") == 0) {
            while (!feof(file)) {
                fscanf(file, "%s", word);
                printf("%d : %s\n", i, word);
                if (strcmp(word, "=") == 0 ||
                    strcmp(word, "==") == 0) { //If there is another task or action this action is finished
                    fseek(file, -2, SEEK_CUR);
                    break;
                }
                if (strchr(word, '#') != NULL) { //If a # is present in the string it's a comment so we skip it
                    skipComment(file);
                    fscanf(file, "%s", word);
                }
                if (strcmp(word, "{name") == 0) {
                    fscanf(file, " ->  %30[0-9a-zA-Z ]", tmp);
                    printf("%d - name : %s\n", i, tmp);
                    actionArray[i].name = malloc(strlen(tmp));
                    strcpy(actionArray[i].name, tmp);
                }
                if (strcmp(word, "{url") == 0) {
                    fscanf(file, " -> %s", tmp);
                    tmp[strlen(tmp) - 1] = '\0';
                    printf("%d - url : %s\n", i, tmp);
                    actionArray[i].url = malloc(strlen(tmp));
                    strcpy(actionArray[i].url, tmp);
                }
                if (strcmp(word, "{max-depth") == 0) {
                    fscanf(file, " -> %2[0-9]", tmp);
                    actionArray[i].maxDepth = atoi(tmp);
                }
                if (strcmp(word, "{versionning") == 0) {
                    fscanf(file, " -> %3[a-zA-Z]", tmp);
                    if (strcmp(tmp, "on") == 0 || strcmp(tmp, "ON") == 0) {
                        actionArray[i].versionning = 1;
                    } else {
                        actionArray[i].versionning = 0;
                    }
                }
            }
            ++i;
        }
        fscanf(file, "%s", word);
    }
    fseek(file, curPos, SEEK_SET);
    free(word);
    free(tmp);
}

//TODO même pb que pour initActionArray
/**
 * Function to initialize the array of tasks following the values of the sconf
 * @param taskArray array of structures task
 * @param sizeArray
 * @param file sconf file
 */
void initTaskArray(task* taskArray, int sizeArray, FILE* file) {
    char* word = malloc(BUFFER_SIZE);
    char* tmp = malloc(256);
    long curPos = ftell(file);
    fseek(file, 0, SEEK_SET);
    int i = 0;

    while (!feof(file) && i < sizeArray) { //Read all the file until it's the end of the file or i > the number of tasks counted before
        fscanf(file, "%s", word);
        if (strcmp(word, "==") == 0) {
            while (!feof(file)) {
                fscanf(file, "%s", word);
                if (strcmp(word, "==") == 0 ||
                    strcmp(word, "=") == 0) { //If there is another task or action this task is finished
                    fseek(file, -2, SEEK_CUR);
                    break;
                }
                if (strchr(word, '#') != NULL) { //If a # is present in the string it's a comment so we skip it
                    skipComment(file);
                    fscanf(file, "%s", word);
                }
                if (strcmp(word, "name") == 0) {
                    fscanf(file, " -> %30[0-9a-zA-Z ]", tmp);
                    taskArray[i].name = malloc(strlen(tmp));
                    strcpy(taskArray[i].name, tmp);
                }
                if (strcmp(word, "hour") == 0) {
                    fscanf(file, " -> %4[0-9]", tmp);
                    taskArray[i].hour = atoi(tmp);
                }
                if (strcmp(word, "minute") == 0) {
                    fscanf(file, " -> %2[0-9]", tmp);
                    taskArray[i].min = atoi(tmp);
                }
                if (strcmp(word, "second") == 0) {
                    fscanf(file, " -> %2[0-9]", tmp);
                    taskArray[i].sec = atoi(tmp);
                }
            }
            ++i;
        }
    }
    fseek(file, curPos, SEEK_SET);
    free(word);
    free(tmp);
}

/**
 * Move the cursor to the next line
 * @param file sconf file
 */
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

/**
 * Count the occurrences of a word in a file
 * @param file sconf file
 * @param string word we search for
 * @return total count of the word in a the file
 */
int countOccurrences(FILE* file, char* string) {
    int count = 0;
    long pos = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* str = malloc(BUFFER_SIZE);
    while (!feof(file)) {
        fscanf(file, "%s", str);
        if (strcmp(str, string) == 0) {
            ++count;
        }
    }
    fseek(file, pos, SEEK_SET);
    return count;
}

void freeActionArray(action* array, int sizeArray) {
    int i;
    for (i = 0; i < sizeArray; ++i) {
        if (array[i].name != NULL) {
            free(array[i].name);
        }
        if (array[i].url != NULL) {
            free(array[i].url);
        }
    }
}

void freeTaskArray(task* array, int sizeArray) {
    int i;
    for (i = 0; i < sizeArray; ++i) {
        if (array[i].name != NULL) {
            free(array[i].name);
        }
    }
}