#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH_OF_CITY_NAME 20
#define MAX_URL_LENGTH 200
#define WEATHER_DATA_FILE "weather.txt"

void fetchTemprature(char cityName[]) 
{
    char url[MAX_URL_LENGTH];
    sprintf(url, "curl \"https://api.openweathermap.org/data/2.5/weather?q=%s&appid=abe3a0f4d0b6cebfbe7393b4b4e3aa28&units=metric\" > %s", cityName, WEATHER_DATA_FILE);
    printf("Fetching Temperature please wait..\n");
    system(url);
}

void printTemperature(char cityName[]) 
{
    FILE *fpWeatherData = fopen(WEATHER_DATA_FILE, "r");
    if (!fpWeatherData) 
    {
        printf("Error opening %s file for reading.\n", WEATHER_DATA_FILE);
        fclose(fpWeatherData);
        exit(0);
    }

    fseek(fpWeatherData, 0, SEEK_END);
    long length = ftell(fpWeatherData);
    fseek(fpWeatherData, 0, SEEK_SET);

    char weatherData[length + 1];
    fread(weatherData, length, 1, fpWeatherData);
    fclose(fpWeatherData);
    weatherData[length] = '\0';

    char *token = strtok(weatherData, "\"");

    while (token != NULL) 
    {
        if (strcmp(token, "temp") == 0) 
        {
            token = strtok(NULL, ":,");
            printf("Temperature of %s: %s\n", cityName, token);
            break;
        }
        token = strtok(NULL, "\"");
    }
}

int main() 
{
    char cityName[MAX_LENGTH_OF_CITY_NAME];

    printf("Enter city name to get its Temperature: ");
    scanf("%s", cityName);

    fetchTemprature(cityName);
    printTemperature(cityName);

    return 0;
}
