#include <stdio.h>
#include <stdlib.h>

int fileSize(char* fileName) {
    FILE* file = fopen(fileName, "r");
    fseek(file, 0, SEEK_END);
    int fileSize = ftell(file);
    rewind(file);
    fclose(file);
    return fileSize;
}

char* loadFile(char* fileName) {
    FILE* file = fopen(fileName, "r");
    //get file size
    int fs = fileSize(fileName);
    char* fileContents = (char*)malloc(fs+1);
    int i = 0;
    for (i = 0; i < fs; i++) {
        fileContents[i] = fgetc(file);
    }
    fileContents[i+1] = '\0';
    //This is commented out because it's causing an error for some reason. I'm not sure why. I will uncomment it 
    //when I figure out how to fix it, since this does use up memory.
    //fclose(file);
    return fileContents;
}