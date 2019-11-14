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
        exit(0);
    }
    action actionArray[nbAction];
    task taskArray[nbTask];
    printf("%d\n%d", nbAction, nbTask);
    // initActionArray(actionArray, nbAction, file);
    //initTaskArray(taskArray, nbTask, file);
    // printf("%s, %s\n", actionArray[0].name, actionArray[0].url);
    fclose(file);
    return 0;
}

//TODO Cf l'autre init
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
    int i;
    for (i = 0; i < sizeArray; ++i) {
        printf("%d : %s\n", i, word);
        if (strcmp(word, "=") == 0) {
            while (!feof(file)) {
                printf("%d : %s\n", i, word);
                fscanf(file, "%s", word);
                if (strcmp(word, "=") == 0 || strcmp(word, "==") == 0) { //Pas propre
                    fseek(file, -1, SEEK_CUR);
                    break;
                }
                if (strchr(word, '#') != NULL) {
                    skipComment(file);
                    fscanf(file, "%s", word);
                }
                if (strcmp(word, "{name") == 0) {
                    fscanf(file, " -> %s"/** %30[0-9a-zA-Z ]**/, tmp);
                    actionArray[i].name = tmp;
                }
                if (strcmp(word, "{url") == 0) {
                    fscanf(file, " -> %s"/**%256[0-9a-zA-Z/.&?$:=*+-\"\']"**/, tmp);
                    tmp[strlen(tmp) - 1] = '\0';
                    // printf("%s\n",tmp);
                    actionArray[i].url = tmp;
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
        }
        fscanf(file, "%s", word);
    }
    fseek(file, curPos, SEEK_SET);
    free(word);
    free(tmp);
}

//TODO Changer la condition du while + possiblement un if qui englobe le while
/**
 * Function to initialize the array of tasks following the values of the sconf
 * @param taskArray array of structures task
 * @param sizeArray
 * @param file sconf file
 */
void initTaskArray(task* taskArray, int sizeArray, FILE* file) {
    char* word = malloc(BUFFER_SIZE);
    char* tmp = malloc(2048);
    long curPos = ftell(file);
    fseek(file, 0, SEEK_SET);
    int i;
    for (i = 0; i < sizeArray; ++i) {
        fscanf(file, "%s", word);
        if (strcmp("==", word) == 0) {
            while (!feof(file)) {
                printf("%s", word);
                fscanf(file, "%s", word);
                if (strcmp(word, "=") == 0 || strcmp(word, "==") == 0) {
                    break;
                }
                if (strchr(word, '#') != NULL) {
                    skipComment(file);
                    fscanf(file, "%s", word);
                }
                if (strcmp(word, "name") == 0) {
                    fscanf(file, " -> %30[0-9a-zA-Z ]", tmp);
                    taskArray[i].name = tmp;
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
