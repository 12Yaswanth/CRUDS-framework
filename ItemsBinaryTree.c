// Binary search tree for items domain

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <psapi.h>

#define MAX_LENGTH_OF_ID 10
#define MAX_LENGTH_OF_DESCRIPTION 40
#define DATAFILE "binaryTree.dat"

struct item 
{
    char itemId[MAX_LENGTH_OF_ID];
    char itemDescription[MAX_LENGTH_OF_DESCRIPTION];
    int price;
};

struct treeNode 
{
    struct item itemData;
    struct treeNode *left;
    struct treeNode *right;
};

struct treeNode *root = NULL;
clock_t start_time, end_time;
double time_spent;

void loadData();
void saveTree();
void displayMenu();
void addRecord();
void createNode(struct item newItem);
void showAllItems(struct treeNode *root);
void updateItemPrice(struct treeNode *root);
void deleteItem(struct treeNode **root);
void searchItem(struct treeNode *root);
char *getItemId(char operation[]);
struct item getItemDetails();
struct treeNode *getMatchingNode(struct treeNode *root, char *itemId);
char getConfirmationToDelete();
struct treeNode* getSuccessor(struct treeNode* node);
char* generateRandomId();
void printItemDetails(struct treeNode* Node);
int getUpdatePrice(char itemId[]);
void saveTreeToFile(struct treeNode *root, FILE *fpDataFile);
void printItemNotFoundMessage(char *itemId);
void getMemoryUsage();
void measureExecutionTime(clock_t start_time, clock_t end_time, const char* operation);
void deleteNodeFromTree(struct treeNode **root, struct treeNode *deleteNode, struct treeNode *parent);
void handleNoChildCase(struct treeNode **root, struct treeNode *deleteNode, struct treeNode *parent);
void handleSingleChildCase(struct treeNode **root, struct treeNode *deleteNode, struct treeNode *parent, struct treeNode *child);
void handleTwoChildrenCase(struct treeNode **root, struct treeNode *deleteNode, struct treeNode *parent);

int main() 
{
    loadData();
    do 
    {
        displayMenu();
    } while (1);
    return 0;
}

void displayMenu() 
{
    int menuChoice;
    printf("\n\t==== Y MART ====\n");
    printf("\t1. Add item\n");
    printf("\t2. Show all items\n");
    printf("\t3. Update item\n");
    printf("\t4. Delete item\n");
    printf("\t5. Search item\n");
    printf("\t6. Get memory Usage\n");
    printf("\t0. Exit\n");
    printf("\t==================\n");
    printf("Enter your menu choice: ");
    scanf("%d", &menuChoice);

    switch (menuChoice) 
    {
        case 1:
            start_time = clock();
            addRecord();
            end_time = clock();
            measureExecutionTime(start_time, end_time, "Add item");
            break;
        case 2:
            printf("%s\n", root->itemData.itemId);
            start_time = clock();
            showAllItems(root);
            end_time = clock();
            measureExecutionTime(start_time, end_time, "Show all items");
            break;
        case 3:
            start_time = clock();
            updateItemPrice(root);
            saveTree();
            end_time = clock();
            measureExecutionTime(start_time, end_time, "Update item");
            break;
        case 4:
            start_time = clock();
            deleteItem(&root);
            saveTree();
            end_time = clock();
            measureExecutionTime(start_time, end_time, "Delete item");
            break;
        case 5:
            start_time = clock();
            searchItem(root);
            end_time = clock();
            measureExecutionTime(start_time, end_time, "Search item");
            break;
        case 6:
            getMemoryUsage();
            break;
        case 0:
            printf("Exiting the program. You entered '0'.\n");
            saveTree(); 
            exit(0);
        default:
            printf("Enter valid menu choice \n");
    }
}

void addRecord() 
{
    struct item newItem = getItemDetails();
    createNode(newItem);
    saveTree(root);
    printf("Item added successfully.\n");
}

void createNode(struct item newItem)
{
    struct treeNode* newNode = (struct treeNode*)malloc(sizeof(struct treeNode));
    newNode->itemData = newItem;
    newNode->left = NULL;
    newNode->right = NULL;

    if (root == NULL) 
    {
        root = newNode;
    } 
    else 
    {
        struct treeNode* current = root;
        struct treeNode* parent = NULL;

        while (1) 
        {
            parent = current;
            if (strcmp(newItem.itemId, current->itemData.itemId) < 0) 
            {
                current = current->left;
                if (current == NULL) 
                {
                    parent->left = newNode;
                    return;
                }
            } 
            else 
            {
                current = current->right;
                if (current == NULL) 
                {
                    parent->right = newNode;
                    return;
                }
            }
        }
    }
}

void loadData() 
{
    FILE *fpDataFile = fopen(DATAFILE, "rb");
    if (fpDataFile == NULL) 
    {
        printf("Error opening file.");
        exit(1);
    }

    struct item newItem;
    while (fread(&newItem, sizeof(struct item), 1, fpDataFile)) 
    {
        createNode(newItem);
    }

    fclose(fpDataFile);
}

void showAllItems(struct treeNode *root) 
{
    if (root != NULL) 
    {
        showAllItems(root->left);
        printItemDetails(root);
        showAllItems(root->right);
    }
}

void updateItemPrice(struct treeNode *root) 
{
    char *itemId = getItemId("update");
    struct treeNode *updateNode = getMatchingNode(root, itemId);
    if (updateNode != NULL) 
    {
        int newPrice = getUpdatePrice(itemId);
        updateNode->itemData.price = newPrice;
        printf("Item price for id %s updated successfully. \n", itemId);
    } 
    else 
    {
       printItemNotFoundMessage(itemId); 
    }
}

void deleteItem(struct treeNode **root) 
{
    char *itemId = getItemId("delete");
    struct treeNode *parent = NULL;
    struct treeNode *current = *root;
    struct treeNode *deleteNode = getMatchingNode(*root, itemId);
    
    if (deleteNode == NULL) 
    {
        printItemNotFoundMessage(itemId);
        return;
    }

    char confirmation = getConfirmationToDelete(itemId);
    
    if (confirmation != 'Y' && confirmation != 'y') 
    {
        printf("Deletion canceled for item with id %s.\n", itemId);
        return;
    }

    while (current != NULL && current != deleteNode) 
    {
        parent = current;
        if (strcmp(itemId, current->itemData.itemId) < 0) 
        {
            current = current->left;
        } 
        else 
        {
            current = current->right;
        }
    }
    
    deleteNodeFromTree(root, deleteNode, parent);

    printf("Item with id %s deleted successfully.\n", itemId);
}

void deleteNodeFromTree(struct treeNode **root, struct treeNode *deleteNode, struct treeNode *parent) 
{
    if (deleteNode->left == NULL && deleteNode->right == NULL) 
    {
        handleNoChildCase(root, deleteNode, parent);
    }
    else if (deleteNode->right == NULL) 
    {
        handleSingleChildCase(root, deleteNode, parent, deleteNode->left);
    } 
    else if (deleteNode->left == NULL) 
    {
        handleSingleChildCase(root, deleteNode, parent, deleteNode->right);
    } 
    else 
    {
        handleTwoChildrenCase(root, deleteNode, parent);
    }

    free(deleteNode);
}

void handleNoChildCase(struct treeNode **root, struct treeNode *deleteNode, struct treeNode *parent) 
{
    if (deleteNode == *root) 
    {
        *root = NULL;
    } 
    else if (deleteNode == parent->left) 
    {
        parent->left = NULL;
    } 
    else 
    {
        parent->right = NULL;
    }
}

void handleSingleChildCase(struct treeNode **root, struct treeNode *deleteNode, struct treeNode *parent, struct treeNode *child) 
{
    if (deleteNode == *root) 
    {
        *root = child;
    } 
    else if (deleteNode == parent->left) 
    {
        parent->left = child;
    } 
    else 
    {
        parent->right = child;
    }
}

void handleTwoChildrenCase(struct treeNode **root, struct treeNode *deleteNode, struct treeNode *parent) 
{
    struct treeNode* successor = getSuccessor(deleteNode);
    if (deleteNode == *root) 
    {
        *root = successor;
    } 
    else if (deleteNode == parent->left) 
    {
        parent->left = successor;
    } 
    else 
    {
        parent->right = successor;
    }
    successor->left = deleteNode->left;
}



struct treeNode* getSuccessor(struct treeNode* node) 
{
    struct treeNode* successorParent = node;
    struct treeNode* successor = node;
    struct treeNode* current = node->right;

    while (current != NULL) 
    {
        successorParent = successor;
        successor = current;
        current = current->left;
    }

    if (successor != node->right) 
    {
        successorParent->left = successor->right;
        successor->right = node->right;
    }

    return successor;
}

void searchItem(struct treeNode *root) 
{
    char *itemId = getItemId("search");
    struct treeNode *searchNode = getMatchingNode(root, itemId);
    if (searchNode != NULL) 
    {
        printItemDetails(searchNode);
    } 
    else 
    {
        printItemNotFoundMessage(itemId);
    }
}

struct item getItemDetails() 
{
    struct item newItem;
    strcpy(newItem.itemId, generateRandomId());
    printf("Enter item Description: ");
    scanf(" %[^\n]s", newItem.itemDescription);
    printf("Enter item price: ");
    scanf("%d", &newItem.price);
    return newItem;
}

char *getItemId(char operation[]) 
{
    static char itemId[MAX_LENGTH_OF_ID];
    printf("Enter item id to %s: ", operation);
    scanf("%s", itemId);
    return itemId;
}

struct treeNode *getMatchingNode(struct treeNode *root, char *itemId) 
{
    if (root == NULL || strcmp(itemId, root->itemData.itemId) == 0) 
    {
        return root;
    }

    if (strcmp(itemId, root->itemData.itemId) < 0) 
    {
        return getMatchingNode(root->left, itemId);
    } 
    else 
    {
        return getMatchingNode(root->right, itemId);
    }
}

void saveTree() 
{
    FILE *fpDataFile = fopen(DATAFILE, "wb");
    if (fpDataFile == NULL) 
    {
        printf("Error opening file.");
        exit(1);
    }

    saveTreeToFile(root, fpDataFile);

    fclose(fpDataFile);
}

void saveTreeToFile(struct treeNode *root, FILE *fpDataFile) 
{
    if (root != NULL) 
    {
        fwrite(&(root->itemData), sizeof(struct item), 1, fpDataFile);
        saveTreeToFile(root->left, fpDataFile);
        saveTreeToFile(root->right, fpDataFile);
    }
}
int getUpdatePrice(char itemId[])
{
    int price;
    printf("Enter new price for item %s: ", itemId);
    scanf("%d", &price);
    return price;
}
char getConfirmationToDelete(char itemId[])
{
    char confirmation;
    printf("Are you sure you want to delete item with id %s? (Y/N): ", itemId);
    scanf(" %c", &confirmation);
    fflush(stdin);
    return confirmation;
}
void printItemDetails(struct treeNode* Node)
{
    printf("==============================\n");
    printf("Item Id: %s\n", Node->itemData.itemId);
    printf("Item Description: %s\n", Node->itemData.itemDescription);
    printf("Item price: %d\n", Node->itemData.price);
    printf("==============================\n");
}
char* generateRandomId() 
{
    static char randomId[MAX_LENGTH_OF_ID]; 
    srand(time(NULL));
    int Counter;
    for (Counter = 0; Counter < MAX_LENGTH_OF_ID; Counter++) 
    {
        randomId[Counter] = '0' + rand() % 10;
    }

    randomId[MAX_LENGTH_OF_ID - 1] = '\0';
    return randomId;
}
void printItemNotFoundMessage(char* itemId)
{
    printf("Item not found with id %s\n", itemId);
}
void getMemoryUsage() 
{
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) 
    {
        SIZE_T virtualMemUsedByMe = pmc.PrivateUsage / (1024 * 1024); 
        SIZE_T physicalMemUsedByMe = pmc.WorkingSetSize / (1024 * 1024); 
        printf("Virtual memory used by this process: %lu MB\n", virtualMemUsedByMe);
        printf("Physical memory used by this process: %lu MB\n", physicalMemUsedByMe);
    } 
    else 
    {
        printf("Failed to retrieve memory information.\n");
    }
}
void measureExecutionTime(clock_t start_time, clock_t end_time, const char* operation) 
{
    double time_spent = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Execution time for %s: %f seconds\n", operation, time_spent);
}

