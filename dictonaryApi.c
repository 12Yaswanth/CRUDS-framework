//To get definition and pronunciation of the given word

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 50
#define MAX_URL_LENGTH 100
#define MAX_DEFINITION_LENGTH 200
#define MAX_AUDIO_URL_LENGTH 200

FILE *fpDictionary;
char apiUrl[MAX_URL_LENGTH];
char inputWord[MAX_WORD_LENGTH];
char* definition;
char* pronunciationUrl;

char* getDefinition(char *response);
char* getpronunciationUrl(char *response);


int main() 
{
    while(1)
    { 
    printf("---------------------------------------------------\n");    
    printf("Enter 0 to exit.\n");    
    printf("Enter a word to get its meaning and pronunciation: ");
    scanf("%s", inputWord);
    if(strcmp(inputWord, "0") == 0)
    {
        printf("Exiting the program. You entered '0'.\n");
        exit(0);
    }

    sprintf(apiUrl, "curl -s \"https://api.dictionaryapi.dev/api/v2/entries/en/%s\" > dictionaryApiResponse.txt", inputWord);
    system(apiUrl);

    fpDictionary = fopen("dictionaryApiResponse.txt", "r");
    int fileSize;
    fseek(fpDictionary, 0, SEEK_END);
    fileSize = ftell(fpDictionary);
    fseek(fpDictionary, 0, SEEK_SET);
    char apiResponse[fileSize];
    fgets(apiResponse, fileSize, fpDictionary);

    definition = getDefinition(apiResponse);
    if (strcmp(definition, "Not found") == 0) 
    {
        printf("Sorry, the word \"%s\" is not found in the dictionary.\n", inputWord);
    } 
    else 
    {
        printf("\nDefinition of the word \"%s\": %s\n", inputWord, definition);
    }

    fseek(fpDictionary, 0, SEEK_SET);
    fgets(apiResponse, fileSize, fpDictionary);
    pronunciationUrl = getpronunciationUrl(apiResponse);
    if (strcmp(pronunciationUrl, "Not found") == 0) 
    {
        printf("Pronunciation not found for the word \"%s\" in the dictionary.\n", inputWord);
    }
     else 
    {
        printf("\nPlaying pronunciation of word \"%s\"..\n", inputWord);
        char playCommand[MAX_URL_LENGTH];
        sprintf(playCommand, "start %s", pronunciationUrl);
        system(playCommand);
    }

    fclose(fpDictionary);
    }
    return 0;
}

char* getDefinition(char *response) 
{
    definition = malloc(MAX_DEFINITION_LENGTH);
    strcpy(definition, "Not found");
    char *token = strtok(response, ":");
    while (token != NULL) 
    {
        if (strcmp(token, "[{\"definition\"") == 0) 
        {
            token = strtok(NULL, "\";.");
            strcpy(definition, token);
            break;
        }
        token = strtok(NULL, ":");
    }
    return definition;
}


char* getpronunciationUrl(char *response)
{
    pronunciationUrl = malloc(MAX_AUDIO_URL_LENGTH);
    strcpy(pronunciationUrl, "Not found");
    int isPronunciationFound = 0;

    char *token = strtok(response, "\"");
    while (token != NULL) 
    {
       
        if (strcmp(token, "audio") == 0) 
        {
            token = strtok(NULL, "\"");
            token = strtok(NULL, "\"");
            strcpy(pronunciationUrl, token);
            if((strlen(pronunciationUrl) < 10))
            {
                token = strtok(NULL, "\"");
                continue;
            }
            else
            {
                break;
            }
        }

        token = strtok(NULL, "\"");
    
    }
    return pronunciationUrl;
}
