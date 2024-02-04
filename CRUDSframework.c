// CRUDS framework programme

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MENU_CONFIGURATION "Menu.cfg"
#define FIELDS_CONFIGURATION "Fields.cfg"
#define DATAFILE "framework.dat"
#define MESSAGES_CONFIGURATION "Messages.cfg"
#define MAX_FIELD_LENGTH 60
#define MENU_LINE_LENGTH 100
#define RECORD_ACTIVE  "ACTIVE"
#define RECORD_INACTIVE "INACTIVE"

FILE *fpMenuFile;
FILE *fpFieldsFile;
FILE *fpDataFile;
FILE *fpmessagesFile;
char fieldName[MAX_FIELD_LENGTH];
char fieldValue[MAX_FIELD_LENGTH];
char recordId[MAX_FIELD_LENGTH];
int fieldsCount;
int messagesCount;
char** messagesArray;
char** fieldNames;

void showMenu();
char* removeNewLine(char field[]);
void addRecord();
char** getFieldValues();
void showAllRecords();
int getFieldsCount();
char** getFieldsArray(FILE *fpFieldsFile, int fieldsCount);
void modifyRecord(char event[]);
void searchRecord();
int getChoice();
int getRecordPosition(char recordId[]);
char* getFieldValue(char field[]);
void printNotFoundMessage(char recordId[]);

int main()
{
    fpFieldsFile = fopen(FIELDS_CONFIGURATION, "r");
    if (!fpFieldsFile) 
    {
        printf("Error opening %s file.\n", FIELDS_CONFIGURATION);
        fclose(fpFieldsFile);
        exit(0);
    }
    fpmessagesFile = fopen(MESSAGES_CONFIGURATION, "r");
    if(!fpmessagesFile)
    {
        printf("Error opening %s file. \n", MESSAGES_CONFIGURATION);
        fclose(fpmessagesFile);
        exit(0);
    }
    messagesCount = getFieldsCount(fpmessagesFile);
    fieldsCount = getFieldsCount(fpFieldsFile);
    messagesArray = getFieldsArray(fpmessagesFile, messagesCount);
    fieldNames = getFieldsArray(fpFieldsFile, fieldsCount);
    do 
    {
        showMenu();
    } while(1);

    return 0;
}

void showMenu()
{
    char menuLine[MENU_LINE_LENGTH];
    fpMenuFile = fopen(MENU_CONFIGURATION, "r");
    if (!fpMenuFile)
    {
        printf("Error opening %s file.\n", MENU_CONFIGURATION);
        fclose(fpMenuFile);
        exit(0);
    }

    while (fgets(menuLine, sizeof(menuLine), fpMenuFile)) 
    {
        printf("%s", menuLine);
    }

    fclose(fpMenuFile);

    switch (getChoice()) 
    {
        case 1:
            addRecord(getFieldValues());
            break;
        case 2:
            showAllRecords();
            break;
        case 3:
            modifyRecord("Update");
            break;
        case 4:
            modifyRecord("Delete");
            break;
        case 5:
            searchRecord();
            break;
        case 0:
            fclose(fpmessagesFile);
            fclose(fpFieldsFile);
            printf("Thank you for using.\n");
            exit(0);
        default:
            printf("Enter a valid choice. \n");
    }
}


int getChoice()
{
    int choice;
    printf("Please enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

char** getFieldValues() 
{
    char **fieldValues = (char **)calloc(fieldsCount, sizeof(char *));
    int counter = 0;

    while (counter < fieldsCount) 
    {
        fieldValues[counter] = (char *)calloc(MAX_FIELD_LENGTH, sizeof(char));
        strcpy(fieldValues[counter], getFieldValue(fieldNames[counter]));
        if(counter == 0)
        {
            strcpy(recordId, fieldValues[counter]);
        }
        counter++;
    }

        return fieldValues;

}

void addRecord(char** fieldValues)
{ 
    int counter;
    fpDataFile = fopen(DATAFILE, "a");

    if (!fpDataFile)
    {
        printf("Error opening %s file.\n", DATAFILE);
        fclose(fpDataFile);
        exit(1);
    }

    for (counter = 0; counter < fieldsCount; counter++)
    {
        if (counter == 0) 
        {
            char status[MAX_FIELD_LENGTH] = RECORD_ACTIVE;
            fwrite(status, MAX_FIELD_LENGTH, 1, fpDataFile);
        }
        fwrite(fieldValues[counter], MAX_FIELD_LENGTH, 1, fpDataFile);
    }

    printf("%s %s\n", messagesArray[0], recordId);
    fclose(fpDataFile);
}

void showAllRecords()
{
    fpDataFile = fopen(DATAFILE, "r");

    if (!fpDataFile) 
    {
        printf("Error opening %s file.\n", DATAFILE);
        fclose(fpDataFile);
        exit(0);
    }

    char status[MAX_FIELD_LENGTH];
    int counter = 0;
    int recordsCount = 0;

    while (fread(status, MAX_FIELD_LENGTH, 1, fpDataFile)) 
    {
        if (strcmp(status, RECORD_ACTIVE) == 0) 
        {
            for (counter = 0; counter < fieldsCount; counter++) 
            {
                fread(fieldValue, MAX_FIELD_LENGTH, 1, fpDataFile);
                printf("%s: %s\n", fieldNames[counter], fieldValue);
            }
            printf("----------------------\n");
            recordsCount++;
        }
        else
        {
            fseek(fpDataFile, MAX_FIELD_LENGTH * fieldsCount, SEEK_CUR);
        }
    }

    printf("%d %s. \n", recordsCount, messagesArray[1]);
    fclose(fpDataFile);
}



int getRecordPosition(char recordId[])
{
    char status[MAX_FIELD_LENGTH];
    int recordPosition = -1;
    fpDataFile = fopen(DATAFILE, "r");

    if (!fpDataFile) 
    {
        printf("Error opening %s file.\n", DATAFILE);
        fclose(fpDataFile);
        exit(0);
    }

    while (fread(fieldValue, MAX_FIELD_LENGTH, 1, fpDataFile)) 
    {
        if (strcmp(fieldValue, RECORD_ACTIVE) == 0) 
        {
            fread(fieldValue, MAX_FIELD_LENGTH, 1, fpDataFile);
            if (strcmp(fieldValue, recordId) == 0) 
            {
                fseek(fpDataFile, -MAX_FIELD_LENGTH, SEEK_CUR);
                recordPosition = ftell(fpDataFile);
                break;
            }
        }
    }

    fclose(fpDataFile);
    return recordPosition;
}

char ** getFieldsArray(FILE *fpFieldsFile, int fieldsCount)
{
    char** fieldsArray;
    int counter = 0;
   

    if (!fpFieldsFile) 
    {
        printf("Error opening %s file.\n", FIELDS_CONFIGURATION);
        fclose(fpFieldsFile);
        exit(0);
    }

    fieldsArray = (char**)malloc(sizeof(char*) * fieldsCount);
    while (fgets(fieldName, sizeof(fieldName), fpFieldsFile)) 
    {
        fieldsArray[counter] = (char*)malloc(MAX_FIELD_LENGTH);
        strcpy(fieldName, removeNewLine(fieldName));
        strcpy(fieldsArray[counter], fieldName);
        counter++;
    }

    rewind(fpFieldsFile);
    fclose(fpFieldsFile);
    return fieldsArray;
}

char* removeNewLine(char field[])
{
    int stringLength = strlen(field);
    if (stringLength > 0 && field[stringLength - 1] == '\n') 
    {
        field[stringLength - 1] = '\0';
    }
    return field;
}


int getFieldsCount(FILE* fpFieldsFile)
{
    int fieldsCount = 0;
    while (fgets(fieldName, sizeof(fieldName), fpFieldsFile)) 
    {
        fieldsCount++;
    }
    rewind(fpFieldsFile);
    return fieldsCount;
}

void modifyRecord(char event[])
{
    char updatingValue[MAX_FIELD_LENGTH];
    int updateFieldNumber;
    strcpy(recordId, getFieldValue(fieldNames[0]));
    int recordPosition = getRecordPosition(recordId);

    if (recordPosition != -1) 
    {
        if (strcmp(event, "Update") == 0) 
        {
            int counter;
            for (counter = 1; counter < fieldsCount; counter++) 
            {
                printf("%d. %s\n", counter,fieldNames[counter]);
            }
            do {
                printf("Enter field to update (0 to cancel): ");
                scanf("%d", &updateFieldNumber);

                if (updateFieldNumber == 0) {
                    fclose(fpFieldsFile);
                    return;  // User canceled the operation
                }

                if (updateFieldNumber < 1 || updateFieldNumber >= fieldsCount) {
                    printf("Invalid field selection. Please enter a valid field.\n");
                }

            } while (updateFieldNumber < 1 || updateFieldNumber >= fieldsCount);
            strcpy(updatingValue, getFieldValue(fieldNames[updateFieldNumber]));

            fpDataFile = fopen(DATAFILE, "r+");
            if (!fpDataFile) 
            {
                printf("Error opening %s file.\n", DATAFILE);
                fclose(fpDataFile);
                exit(0);
            }
                if (updateFieldNumber > 0 && updateFieldNumber < fieldsCount) 
                {
                    fseek(fpDataFile, recordPosition + (updateFieldNumber * MAX_FIELD_LENGTH), SEEK_CUR);
                    fread(fieldValue, MAX_FIELD_LENGTH, 1, fpDataFile);
                    printf("%s %s %s\n",fieldNames[updateFieldNumber], messagesArray[2], recordId);
                }     
        }

        if (strcmp(event, "Delete") == 0) 
        {
            char status[MAX_FIELD_LENGTH];
            fpDataFile = fopen(DATAFILE, "r+");

            if (!fpDataFile) 
            {
                printf("Error opening %s file.\n", DATAFILE);
                fclose(fpDataFile);
                exit(0);
            }

            fseek(fpDataFile, recordPosition, SEEK_SET);
            fseek(fpDataFile, -MAX_FIELD_LENGTH, SEEK_CUR);
            fread(updatingValue, MAX_FIELD_LENGTH, 1, fpDataFile);
            strcpy(updatingValue, RECORD_INACTIVE);
            printf("%s %s.\n", messagesArray[3],recordId);
        }
            fseek(fpDataFile, -MAX_FIELD_LENGTH, SEEK_CUR);
            fwrite(updatingValue, MAX_FIELD_LENGTH, 1, fpDataFile);
            fclose(fpDataFile);
    } 
    else 
    {
        printNotFoundMessage(recordId);
    }
}

void searchRecord()
{
    int counter;
    strcpy(recordId, getFieldValue(fieldNames[0]));
    int recordPosition = getRecordPosition(recordId);

    if (recordPosition != -1) 
    {
        fpDataFile = fopen(DATAFILE, "r");

        if (!fpDataFile) 
        {
            printf("Error opening %s file.\n", DATAFILE);
            fclose(fpDataFile);
            exit(0);
        }

        printf("-------------------------------\n");
        fseek(fpDataFile, recordPosition, SEEK_SET);
        for (counter = 0; counter < fieldsCount; counter++) 
        {
            fread(fieldValue, MAX_FIELD_LENGTH, 1, fpDataFile);
            printf("%s: %s\n", fieldNames[counter], fieldValue);
        }
        printf("-------------------------------\n");
    } 
    else 
    {
        printNotFoundMessage(recordId);
    }

    fclose(fpDataFile);
}

char* getFieldValue(char field[])
{
    printf("Enter %s: ", field);
    scanf("%s", fieldValue);
    return fieldValue;
}

void printNotFoundMessage(char recordId[])
{
    printf("%s %s.\n", messagesArray[4],recordId);
}
