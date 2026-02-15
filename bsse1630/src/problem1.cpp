#include "../headers/problem1.h"
#include "../headers/nodesAndEdges.h"
#include "../headers/mode.h"
#include "../headers/kmlExport.h"
#include <cstdio>
#include <cmath>

// Print detailed routePath for Problem 1
void displayProblem1Results(int routePath[], int routeLength, int sourceVertex, int destinationVertex,
                          double sourceLatitude, double sourceLongitude, double destinationLatitude, double destinationLongitude) {
    double accumulatedDistance = 0.0;
    double accumulatedCost = 0.0;
    
    printf("\n========== PROBLEM 1: SHORTEST CAR ROUTE ==========\n");
    printf("Source: (%.6f, %.6f)\n", sourceLongitude, sourceLatitude);
    printf("Destination: (%.6f, %.6f)\n", destinationLongitude, destinationLatitude);
    printf("===================================================\n\n");
    
    // If sourceVertex is not exactly on a node, show walking segment
    if (fabs(vertexArray[sourceVertex].lat - sourceLatitude) > 1e-6 || 
        fabs(vertexArray[sourceVertex].lon - sourceLongitude) > 1e-6) {
        double walkingDistance = calculateGeoDistance(sourceLatitude, sourceLongitude,
                                           vertexArray[sourceVertex].lat, vertexArray[sourceVertex].lon);
        printf("Walk originVertex Source (%.6f, %.6f) targetVertex (%.6f, %.6f)\n",
               sourceLongitude, sourceLatitude, vertexArray[sourceVertex].lon, vertexArray[sourceVertex].lat);
        printf("  Distance: %.3f km, Cost: Tk 0.00\n\n", walkingDistance);
        accumulatedDistance += walkingDistance;
    }
    
    // Print each car segment
    for (int i = routeLength - 1; i > 0; i--) {
        int originVertex = routePath[i];
        int targetVertex = routePath[i - 1];
        
        // Find the edge distance
        double connectionDistance = 0;
        for (int j = 0; j < connectionCount; j++) {
            if (connectionArray[j].originVertex == originVertex && connectionArray[j].targetVertex == targetVertex && connectionArray[j].transportMode == TRANSPORT_CAR) {
                connectionDistance = connectionArray[j].distance;
                break;
            }
        }
        
        double segmentCost = connectionDistance * CAR_COST_PER_KM;
        accumulatedDistance += connectionDistance;
        accumulatedCost += segmentCost;
        
        printf("Ride Car originVertex (%.6f, %.6f) targetVertex (%.6f, %.6f)\n",
               vertexArray[originVertex].lon, vertexArray[originVertex].lat,
               vertexArray[targetVertex].lon, vertexArray[targetVertex].lat);
        printf("  Distance: %.3f km, Cost: Tk %.2f\n\n", connectionDistance, segmentCost);
    }
    
    // If destination is not exactly on a node, show walking segment
    if (fabs(vertexArray[destinationVertex].lat - destinationLatitude) > 1e-6 || 
        fabs(vertexArray[destinationVertex].lon - destinationLongitude) > 1e-6) {
        double walkingDistance = calculateGeoDistance(vertexArray[destinationVertex].lat, vertexArray[destinationVertex].lon,
                                           destinationLatitude, destinationLongitude);
        printf("Walk originVertex (%.6f, %.6f) targetVertex Destination (%.6f, %.6f)\n",
               vertexArray[destinationVertex].lon, vertexArray[destinationVertex].lat, destinationLongitude, destinationLatitude);
        printf("  Distance: %.3f km, Cost: Tk 0.00\n\n", walkingDistance);
        accumulatedDistance += walkingDistance;
    }
    
    printf("===================================================\n");
    printf("Total Distance: %.3f km\n", accumulatedDistance);
    printf("Total Cost: Tk %.2f\n", accumulatedCost);
    printf("===================================================\n");
}

// Run Problem 1: Shortest Car Route
void executeProblem1() {
    double sourceLatitude, sourceLongitude, destinationLatitude, destinationLongitude;
    
    printf("\n--- Problem 1: Shortest Car Route ---\n");
    printf("Enter sourceVertex latitude and longitude: ");
    scanf("%lf %lf", &sourceLatitude, &sourceLongitude);
    
    printf("Enter destination latitude and longitude: ");
    scanf("%lf %lf", &destinationLatitude, &destinationLongitude);
    
    // Find closestVertex vertexArray
    int sourceVertex = locateClosestVertex(sourceLatitude, sourceLongitude);
    int destinationVertex = locateClosestVertex(destinationLatitude, destinationLongitude);
    
    if (sourceVertex == -1 || destinationVertex == -1) {
        printf("Error: Could not find vertexArray\n");
        return;
    }
    
    printf("\nUsing closestVertex roadmap vertexArray:\n");
    printf("Source Vertex: %s (%.6f, %.6f)\n", vertexArray[sourceVertex].name, 
           vertexArray[sourceVertex].lat, vertexArray[sourceVertex].lon);
    printf("Target Vertex: %s (%.6f, %.6f)\n", vertexArray[destinationVertex].name,
           vertexArray[destinationVertex].lat, vertexArray[destinationVertex].lon);
    
    // Dijkstra initialization
    for (int i = 0; i < vertexCount; i++) {
        distanceArray[i] = INFINITY_VALUE;
        predecessorArray[i] = -1;
        processedArray[i] = 0;
        incomingConnectionArray[i] = -1;
    }
    distanceArray[sourceVertex] = 0;
    
    // Dijkstra's algorithm
    for (int tokenCount = 0; tokenCount < vertexCount; tokenCount++) {
        int u = -1;
        double minimumDistance = INFINITY_VALUE;
        
        // Find unvisited node with minimum distance
        for (int i = 0; i < vertexCount; i++) {
            if (!processedArray[i] && distanceArray[i] < minimumDistance) {
                minimumDistance = distanceArray[i];
                u = i;
            }
        }
        
        if (u == -1 || u == destinationVertex) break;
        
        processedArray[u] = 1;
        
        // Relax connectionArray (only consider CAR transportMode for Problem 1)
        for (int i = 0; i < connectionCount; i++) {
            if (connectionArray[i].originVertex == u && connectionArray[i].transportMode == TRANSPORT_CAR) {
                int v = connectionArray[i].targetVertex;
                double updatedDistance = distanceArray[u] + connectionArray[i].distance;
                if (updatedDistance < distanceArray[v]) {
                    distanceArray[v] = updatedDistance;
                    predecessorArray[v] = u;
                    incomingConnectionArray[v] = i;
                }
            }
        }
    }
    
    // Build routePath
    int routePath[VERTEX_LIMIT];
    int routeConnections[VERTEX_LIMIT];
    int routeLength = 0;
    for (int currentVertex = destinationVertex; currentVertex != -1; currentVertex = predecessorArray[currentVertex]) {
        routePath[routeLength] = currentVertex;
        routeConnections[routeLength] = incomingConnectionArray[currentVertex];
        routeLength++;
    }
    
    if (routeLength == 1 || distanceArray[destinationVertex] >= INFINITY_VALUE) {
        printf("No routePath found between the selected vertexArray.\n");
        return;
    }
    
    printf("\nShortest routePath found with distance: %.3f km\n", distanceArray[destinationVertex]);
    
    // Print detailed routePath
    displayProblem1Results(routePath, routeLength, sourceVertex, destinationVertex, sourceLatitude, sourceLongitude, destinationLatitude, destinationLongitude);
    
    // Export to KML
    char kmlFilename[256];
    snprintf(kmlFilename, sizeof(kmlFilename), "problem1_shortest_car_route.kml");
    exportRouteToKML(kmlFilename, routePath, routeLength, routeConnections,
                     "Problem 1: Shortest Car Route",
                     "Shortest distance route using car only");
}
