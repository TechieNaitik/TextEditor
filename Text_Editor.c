#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_TEXT_LENGTH 1000

// Define a structure for a stack node
struct StackNode {
    char data;
    struct StackNode* next;
};

struct StackNode* createStackNode(char data);
void SetColor(int ForgC);
int isEmpty(struct StackNode* root);
void push(struct StackNode** root, char data);
char pop(struct StackNode** root);
void saveTextToFile(const char *filename, const char *text);
void textEditor();
void programEnd();

// Function to create a new stack node
struct StackNode* createStackNode(char data) {
    struct StackNode* newNode = (struct StackNode*)malloc(sizeof(struct StackNode));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void SetColor(int ForgC) {
     WORD wColor;
                          //We will need this handle to get the current background attribute
     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;

                           //We use csbi for the wAttributes word.
     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
                     //Mask out all but the background attribute, and add in the forgournd color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
}

// Function to check if the stack is empty
int isEmpty(struct StackNode* root) {
    return (root == NULL);
}

// Function to push a character onto the stack
void push(struct StackNode** root, char data) {
    struct StackNode* newNode = createStackNode(data);
    newNode->next = *root;
    *root = newNode;
}

// Function to pop a character from the stack
char pop(struct StackNode** root) {
    if (isEmpty(*root)) {
        printf("Stack is empty.\n");
        return '\0';
    }
    struct StackNode* temp = *root;
    *root = (*root)->next;
    char popped = temp->data;
    free(temp);
    return popped;
}

void saveTextToFile(const char *filename, const char *text) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening the file for writing");
    } else {
        fprintf(file, "%s", text);
        fclose(file);
    }
}

// Function to implement a simple text editor using a stack
void textEditor() {
    struct StackNode* undoStack = NULL;
    struct StackNode* redoStack = NULL;
    char text[MAX_TEXT_LENGTH] = "";  // Initial text is empty

    while (1) {
        system("cls"); menu:
        printf("Text: %s\n", text);
        printf("Options:\n");
        printf("1. Append text\n");
        printf("2. Undo\n");
        printf("3. Redo\n");
        printf("4. Save to file\n");
        printf("5. Load from file\n");
        printf("6. Clear and Save to File\n");
        printf("7. Change Output Text Color\n");
        printf("8. Exit\n");

        int choice;
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                char newText[MAX_TEXT_LENGTH];
                printf("Enter text to append: ");
                scanf(" %[^\n]s", newText);
                for (int i = 0; i < strlen(newText); i++) {
                    push(&undoStack, text[i]);
                }
                strcat(text, newText);
                break;
            }
            case 2: {
                if (!isEmpty(undoStack)) {
                    char removed = pop(&undoStack);
                    push(&redoStack, text[strlen(text) - 1]);
                    text[strlen(text) - 1] = '\0'; // Remove the last character
                }
                break;
            }
            case 3: {
                if (!isEmpty(redoStack)) {
                    char redoChar = pop(&redoStack);
                    push(&undoStack, redoChar);
                    int len = strlen(text);
                    text[len] = redoChar;
                    text[len + 1] = '\0';
                }
                break;
            }
            case 4: {
                char filename[100];
                printf("Enter the filename to save to (e.g., filename.txt): ");
                scanf("%s", filename);
                saveTextToFile(filename, text); system("cls");
                printf("Text saved to %s successfully!!\n", filename); goto menu;
                break;
            }
            case 5: {
                char filename[100];
                printf("Enter the filename to load from: ");
                scanf("%s", filename);
                FILE* file = fopen(filename, "r");
                if (file == NULL)
                    printf("Error opening the file for reading.\n"); 
                else {
                    fgets(text, MAX_TEXT_LENGTH, file);
                    fclose(file);
                } system("cls");
                printf("Text loaded from File successfully!!\n"); goto menu;
                break;
            }
            case 6: {
                char filename[100];
                printf("Enter the filename to clear and save: ");
                scanf("%s", filename);
                FILE* file = fopen(filename, "w");
                if (file == NULL)
                    printf("Error opening the file for writing.\n"); 
                else {
                    fprintf(file, "");
                    fclose(file);
                    strcpy(text, ""); // Clear the text
                } system("cls");
                printf("Text cleared from the File!!\n"); goto menu;
                break;
            }
            case 7: {
                system("cls"); char str[10];
                hello:
                strcpy(str, "0. Black"); printf("%-20s\t", str);
                strcpy(str, "1. Blue"); printf("%-20s\n", str);
                strcpy(str, "2. Green"); printf("%-20s\t", str);
                strcpy(str, "3. Cyan"); printf("%-20s\n", str);
                strcpy(str, "4. Red"); printf("%-20s\t", str);
                strcpy(str, "5. Magenta"); printf("%-20s\n", str);
                strcpy(str, "6. Yellow"); printf("%-20s\t", str);
                strcpy(str, "7. White"); printf("%-20s\n", str);
                printf("Enter the color you want to set: ");
                int ch; scanf("%d", &ch);
                switch(ch) {
                    case 0: SetColor(0); break;
                    case 1: SetColor(1); break;
                    case 2: SetColor(2); break;
                    case 3: SetColor(3); break;
                    case 4: SetColor(4); break;
                    case 5: SetColor(5); break;
                    case 6: SetColor(6); break;
                    case 7: SetColor(7); break;
                    default: printf("\nWrong Choice!! TRY AGAIN\n"); goto hello;
                }
                break;
            }
            case 8:
                programEnd();
            default: {
                printf("Invalid choice.\n");
                printf("Press any key to start the program again!\nPress ESC key to exit the program!!\n");
                char ch = getch();
                if((int)ch == 27)
                    programEnd();
            }
        }
    }
}

void programEnd() {
    system("cls");
    SetColor(1);
    printf("Project Made By: Naitik Shah (122032130501081)\n");
    printf("                 Nandish Shah (12202130501083)\n");
    printf("                 Shivansh Srivastava (12202130501084)\n");
    printf("Project Guided By: Prof. Maulika Patel (HOD)\n");
    exit(1);
}

int main() {
    printf("Press any key to start the program except ESC!\nPress ESC key to exit the program!!\n");
    char ch = getch();
    if((int)ch == 27)
        programEnd();
    textEditor();
    return 0;
}
