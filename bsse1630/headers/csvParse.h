#ifndef CSV_PARSE_H
#define CSV_PARSE_H

#include "mode.h"

// Maximum lineBuffer and token sizes
#define BUFFER_SIZE 200000
#define TOKEN_LIMIT 5000

// CSV parsing functions
void loadRoadNetworkData(const char* filePath);
void loadMetroNetworkData(const char* filePath);
void loadBusNetworkData(const char* filePath, TransportMode transportType);

#endif
