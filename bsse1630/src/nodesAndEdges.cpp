#include "../headers/nodesAndEdges.h"
#include <cmath>
#include <cstring>
#include <cstdio>

// Global arrays definition
Vertex vertexArray[VERTEX_LIMIT];
Connection connectionArray[VERTEX_LIMIT * 10];
int vertexCount = 0;
int connectionCount = 0;

// Dijkstra arrays
double distanceArray[VERTEX_LIMIT];
int predecessorArray[VERTEX_LIMIT];
int processedArray[VERTEX_LIMIT];
int incomingConnectionArray[VERTEX_LIMIT];

// Haversine distance formula
double calculateGeoDistance(double latitude1, double longitude1, double latitude2, double longitude2) {
    double dLat = (latitude2 - latitude1) * PI / 180.0;
    double dLon = (longitude2 - longitude1) * PI / 180.0;
    
    latitude1 = latitude1 * PI / 180.0;
    latitude2 = latitude2 * PI / 180.0;
    
    double a = sin(dLat/2) * sin(dLat/2) +
               cos(latitude1) * cos(latitude2) *
               sin(dLon/2) * sin(dLon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    
    return EARTH_RADIUS_KILOMETERS * c;
}

// Find or add node currentVertex given coordinates
int locateOrCreateVertex(double lat, double lon) {
    // Check if node already exists (within tolerance)
    for (int i = 0; i < vertexCount; i++) {
        if (fabs(vertexArray[i].lat - lat) < 1e-6 && 
            fabs(vertexArray[i].lon - lon) < 1e-6) {
            return i;
        }
    }
    
    // Create new node
    vertexArray[vertexCount].id = vertexCount;
    vertexArray[vertexCount].lat = lat;
    vertexArray[vertexCount].lon = lon;
    strcpy(vertexArray[vertexCount].name, "Vertex");
    
    return vertexCount++;
}

// Find closestVertex node targetVertex given coordinates
int locateClosestVertex(double lat, double lon) {
    if (vertexCount == 0) return -1;
    
    int closestVertex = 0;
    double minimumDistance = calculateGeoDistance(lat, lon, vertexArray[0].lat, vertexArray[0].lon);
    
    for (int i = 1; i < vertexCount; i++) {
        double d = calculateGeoDistance(lat, lon, vertexArray[i].lat, vertexArray[i].lon);
        if (d < minimumDistance) {
            minimumDistance = d;
            closestVertex = i;
        }
    }
    
    return closestVertex;
}

// Add edge targetVertex graph
void insertConnection(int originVertex, int targetVertex, TransportMode transportMode, double distance) {
    if (connectionCount >= VERTEX_LIMIT * 10) {
        printf("Warning: Maximum connectionArray reached!\n");
        return;
    }
    
    connectionArray[connectionCount].originVertex = originVertex;
    connectionArray[connectionCount].targetVertex = targetVertex;
    connectionArray[connectionCount].transportMode = transportMode;
    connectionArray[connectionCount].distance = distance;
    connectionCount++;
}
