#ifndef NODES_AND_EDGES_H
#define NODES_AND_EDGES_H

#include "mode.h"

// Maximum limits
#define VERTEX_LIMIT 100000
#define INFINITY_VALUE 9999999999.0

// Vertex structure (intersection or station)
typedef struct {
    int id;
    char name[32];  // Station name if applicable
    double lat;
    double lon;
} Vertex;

// Connection structure (connection between vertexArray)
typedef struct {
    int originVertex;
    int targetVertex;
    TransportMode transportMode;
    double distance;  // Distance in km
} Connection;

// Global arrays (simple beginner approach)
extern Vertex vertexArray[VERTEX_LIMIT];
extern Connection connectionArray[VERTEX_LIMIT * 10];
extern int vertexCount;
extern int connectionCount;

// Dijkstra algorithm arrays
extern double distanceArray[VERTEX_LIMIT];
extern int predecessorArray[VERTEX_LIMIT];
extern int processedArray[VERTEX_LIMIT];
extern int incomingConnectionArray[VERTEX_LIMIT];

// Function declarations
int locateOrCreateVertex(double lat, double lon);
int locateClosestVertex(double lat, double lon);
void insertConnection(int originVertex, int targetVertex, TransportMode transportMode, double distance);
double calculateGeoDistance(double latitude1, double longitude1, double latitude2, double longitude2);

#endif
