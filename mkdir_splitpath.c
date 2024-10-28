#include "types.h"
#include <string.h>

extern struct NODE* root;
extern struct NODE* cwd;

char* trim(char*str) { 

    char *end;
    while(*str == ' ') str++;
    
    if(*str == '\0') return str;

    end = str + strlen(str) - 1;
    
    while(end > str && *end == ' ') end--;

    *(end + 1) = '\0';

    return str; 

}

void addChildNode(struct NODE* parent, struct NODE* newNode) { 
    
    // Case that parent has no children
    if(parent->childPtr == NULL) {
        parent->childPtr = newNode;

    // Case that parent has children
    // Iterate until child has no sibling
    } else {
        struct NODE* current = parent->childPtr;
        while(current->siblingPtr != NULL) {
            current = current->siblingPtr;
        }
    current->siblingPtr = newNode;
    }
}

struct NODE* findChildNode(struct NODE* parent, const char* name) {
    
    struct NODE* current = parent->childPtr;

    while( current != NULL ) {
        if( strcmp(current->name, name) == 0 ) { 
            return current;
        }
        current = current->siblingPtr;
    }
    
    return NULL;
}

void mkdir(char pathName[]) { 

    char dirName[256];
    char baseName[64]; 
    char* tempPath = trim(pathName);

    if( (tempPath[0] == '\0') | (tempPath[0] == '/') ) { 
        if( tempPath[1] == '\0' ) { 
            printf("MKDIR ERROR: no path provided\n");
            return;
        }
    }

    struct NODE* parent = splitPath(pathName, baseName, dirName); 
    
    if( parent == NULL ) { 
        return;
    }

    if( findChildNode(parent,baseName) != NULL ) { 
        printf("MKDIR ERROR: directory %s already exists\n", pathName);
        return;
    }

    struct NODE* newNode = (struct NODE*)malloc(sizeof(struct NODE));
    if( newNode == NULL ) {
        printf("MKDIR ERROR: mem alloc failed\n");
        return;
    }

    strcpy(newNode->name, baseName);
    newNode->fileType = 'D';
    newNode->childPtr = NULL;
    newNode->siblingPtr = NULL;
    newNode->parentPtr = parent;
    
    addChildNode(parent, newNode);

    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
}


// will handle tokenizing and absolute and relative path options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName) { 
    
    if( strcmp(pathName, "/") == 0 ) {
        strcpy(dirName, "/"); 
        strcpy(baseName, "");

        return root;

    }
    
    // finds the last occurance of '/'
    char* final = strrchr(pathName, '/');
    
    //no '/' in path name, file is in cwd
    if( final == NULL ) { 
        strcpy(baseName, pathName);
        strcpy(dirName, "");
        
        return cwd;

    } else { 
        int directoryLen = final - pathName;
        if( directoryLen == 0 ) { 
            strcpy(dirName, "/");
        } else {
            strncpy(dirName, pathName, directoryLen);
            dirName[directoryLen] = '\0'; 
        } 
        strcpy(baseName, final + 1);
        struct NODE* current = (directoryLen == 0) ? root : cwd;
        char tempPath[256];
        strncpy(tempPath, dirName, sizeof(tempPath));

        char* token = strtok(tempPath, "/");
        while( token != NULL && current != NULL ) { 
            current = findChildNode(current, token); 
            if( current == NULL ) { 
                
                printf("ERROR: directory %s does not exist\n", token);
                return NULL;
            }
            token = strtok(NULL, "/");
        }
        return current;

    }

}