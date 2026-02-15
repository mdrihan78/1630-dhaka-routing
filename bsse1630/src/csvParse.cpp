#include "../headers/csvParse.h"
#include "../headers/nodesAndEdges.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>

// ------------------------------------------------------------
// Helper function to trim whitespace
// ------------------------------------------------------------
void removeWhitespace(char *stringPtr) {
    if (!stringPtr) return;

    // Trim leading whitespace
    char *currentPos = stringPtr;
    while (*currentPos && isspace((unsigned char)*currentPos)) currentPos++;
    if (currentPos != stringPtr) memmove(stringPtr, currentPos, strlen(currentPos) + 1);

    // Trim trailing whitespace
    size_t stringLength = strlen(stringPtr);
    while (stringLength > 0 && isspace((unsigned char)stringPtr[stringLength - 1])) {
        stringPtr[--stringLength] = '\0';
    }
}

// ------------------------------------------------------------
// Split CSV line by comma (Windows compatible)
// ------------------------------------------------------------
int tokenizeCSVLine(char *lineBuffer, char **tokenArray, int tokenLimit) {
    int tokenCount = 0;

    char *currentToken = strtok(lineBuffer, ",");
    while (currentToken && tokenCount < tokenLimit) {
        removeWhitespace(currentToken);
        tokenArray[tokenCount++] = currentToken;
        currentToken = strtok(NULL, ",");
    }

    return tokenCount;
}

// ------------------------------------------------------------
// Check if token is a valid number
// ------------------------------------------------------------
int checkNumericToken(const char *stringPtr) {
    if (!stringPtr) return 0;

    while (*stringPtr && isspace((unsigned char)*stringPtr)) stringPtr++;
    if (*stringPtr == '\0') return 0;

    char *endPtr = NULL;
    strtod(stringPtr, &endPtr);

    while (endPtr && *endPtr && isspace((unsigned char)*endPtr)) endPtr++;

    return endPtr && *endPtr == '\0';
}

// ------------------------------------------------------------
// Parse Roadmap-Dhaka.csv
// ------------------------------------------------------------
void loadRoadNetworkData(const char *filePath) {
    FILE *fileHandle = fopen(filePath, "r");
    if (!fileHandle) {
        printf("Error: Could not open %s\n", filePath);
        return;
    }

    char lineBuffer[BUFFER_SIZE];
    char *tokenArray[TOKEN_LIMIT];
    int roadSegmentCount = 0;

    printf("Parsing roadmap CSV...\n");

    while (fgets(lineBuffer, sizeof(lineBuffer), fileHandle)) {
        lineBuffer[strcspn(lineBuffer, "\r\n")] = 0;
        if (strlen(lineBuffer) == 0) continue;

        int tokenCount = tokenizeCSVLine(lineBuffer, tokenArray, TOKEN_LIMIT);
        if (tokenCount < 6) continue;

        const char *altitudeToken = tokenArray[tokenCount - 2];
        const char *lengthToken = tokenArray[tokenCount - 1];

        if (!checkNumericToken(altitudeToken) || !checkNumericToken(lengthToken))
            continue;

        int coordinateCount = tokenCount - 3;
        if (coordinateCount < 4 || coordinateCount % 2 != 0)
            continue;

        roadSegmentCount++;

        for (int index = 1; index + 3 <= tokenCount - 2; index += 2) {
            double longitude1 = atof(tokenArray[index]);
            double latitude1 = atof(tokenArray[index + 1]);
            double longitude2 = atof(tokenArray[index + 2]);
            double latitude2 = atof(tokenArray[index + 3]);

            int originVertex = locateOrCreateVertex(latitude1, longitude1);
            int targetVertex = locateOrCreateVertex(latitude2, longitude2);

            double segmentDistance = calculateGeoDistance(latitude1, longitude1, latitude2, longitude2);

            insertConnection(originVertex, targetVertex, TRANSPORT_CAR, segmentDistance);
            insertConnection(targetVertex, originVertex, TRANSPORT_CAR, segmentDistance);
        }
    }

    fclose(fileHandle);
    printf("Parsed %d road segments\n", roadSegmentCount);
}

// ------------------------------------------------------------
// Parse Metro CSV
// ------------------------------------------------------------
void loadMetroNetworkData(const char *filePath) {
    FILE *fileHandle = fopen(filePath, "r");
    if (!fileHandle) {
        printf("Error: Could not open %s\n", filePath);
        return;
    }

    char lineBuffer[BUFFER_SIZE];
    char *tokenArray[TOKEN_LIMIT];
    int routeSegmentCount = 0;

    printf("Parsing metro CSV...\n");

    while (fgets(lineBuffer, sizeof(lineBuffer), fileHandle)) {
        lineBuffer[strcspn(lineBuffer, "\r\n")] = 0;
        if (strlen(lineBuffer) == 0) continue;

        int tokenCount = tokenizeCSVLine(lineBuffer, tokenArray, TOKEN_LIMIT);
        if (tokenCount < 5) continue;

        const char *originStationName = tokenArray[tokenCount - 2];
        const char *terminalStationName   = tokenArray[tokenCount - 1];

        if (checkNumericToken(originStationName) || checkNumericToken(terminalStationName))
            continue;

        int coordinateCount = tokenCount - 3;
        if (coordinateCount < 4 || coordinateCount % 2 != 0)
            continue;

        routeSegmentCount++;

        for (int index = 1; index + 3 <= tokenCount - 2; index += 2) {
            double longitude1 = atof(tokenArray[index]);
            double latitude1 = atof(tokenArray[index + 1]);
            double longitude2 = atof(tokenArray[index + 2]);
            double latitude2 = atof(tokenArray[index + 3]);

            int originVertex = locateOrCreateVertex(latitude1, longitude1);
            int targetVertex   = locateOrCreateVertex(latitude2, longitude2);

            double segmentDistance = calculateGeoDistance(latitude1, longitude1, latitude2, longitude2);

            insertConnection(originVertex, targetVertex, TRANSPORT_METRO, segmentDistance);
            insertConnection(targetVertex, originVertex, TRANSPORT_METRO, segmentDistance);

            if (index == 1) {
                strncpy(vertexArray[originVertex].name, originStationName,
                        sizeof(vertexArray[originVertex].name) - 1);
                vertexArray[originVertex].name[sizeof(vertexArray[originVertex].name) - 1] = '\0';
            }

            if (index + 4 > tokenCount - 2) {
                strncpy(vertexArray[targetVertex].name, terminalStationName,
                        sizeof(vertexArray[targetVertex].name) - 1);
                vertexArray[targetVertex].name[sizeof(vertexArray[targetVertex].name) - 1] = '\0';
            }
        }
    }

    fclose(fileHandle);
    printf("Parsed %d metro routes\n", routeSegmentCount);
}

// ------------------------------------------------------------
// Parse Bus CSV
// ------------------------------------------------------------
void loadBusNetworkData(const char *filePath, TransportMode transportType) {
    FILE *fileHandle = fopen(filePath, "r");
    if (!fileHandle) {
        printf("Error: Could not open %s\n", filePath);
        return;
    }

    char lineBuffer[BUFFER_SIZE];
    char *tokenArray[TOKEN_LIMIT];
    int routeSegmentCount = 0;

    printf("Parsing bus CSV (%s)...\n", retrieveTransportTypeName(transportType));

    while (fgets(lineBuffer, sizeof(lineBuffer), fileHandle)) {
        lineBuffer[strcspn(lineBuffer, "\r\n")] = 0;
        if (strlen(lineBuffer) == 0) continue;

        int tokenCount = tokenizeCSVLine(lineBuffer, tokenArray, TOKEN_LIMIT);
        if (tokenCount < 5) continue;

        const char *originStationName = tokenArray[tokenCount - 2];
        const char *terminalStationName   = tokenArray[tokenCount - 1];

        if (checkNumericToken(originStationName) || checkNumericToken(terminalStationName))
            continue;

        int coordinateCount = tokenCount - 3;
        if (coordinateCount < 4 || coordinateCount % 2 != 0)
            continue;

        routeSegmentCount++;

        for (int index = 1; index + 3 <= tokenCount - 2; index += 2) {
            double longitude1 = atof(tokenArray[index]);
            double latitude1 = atof(tokenArray[index + 1]);
            double longitude2 = atof(tokenArray[index + 2]);
            double latitude2 = atof(tokenArray[index + 3]);

            int originVertex = locateOrCreateVertex(latitude1, longitude1);
            int targetVertex   = locateOrCreateVertex(latitude2, longitude2);

            double segmentDistance = calculateGeoDistance(latitude1, longitude1, latitude2, longitude2);

            insertConnection(originVertex, targetVertex, transportType, segmentDistance);
            insertConnection(targetVertex, originVertex, transportType, segmentDistance);

            if (index == 1) {
                strncpy(vertexArray[originVertex].name, originStationName,
                        sizeof(vertexArray[originVertex].name) - 1);
                vertexArray[originVertex].name[sizeof(vertexArray[originVertex].name) - 1] = '\0';
            }

            if (index + 4 > tokenCount - 2) {
                strncpy(vertexArray[targetVertex].name, terminalStationName,
                        sizeof(vertexArray[targetVertex].name) - 1);
                vertexArray[targetVertex].name[sizeof(vertexArray[targetVertex].name) - 1] = '\0';
            }
        }
    }

    fclose(fileHandle);
    printf("Parsed %d bus routes\n", routeSegmentCount);
}