#include "../headers/problem2.h"
#include "../headers/nodesAndEdges.h"
#include "../headers/mode.h"
#include "../headers/kmlExport.h"
#include <cstdio>
#include <cmath>

// Print detailed routePath for Problem 2
void displayProblem2Results(int routePath[], int routeConnections[], int routeLength, int sourceVertex, int destinationVertex,
                          double sourceLatitude, double sourceLongitude, double destinationLatitude, double destinationLongitude) {
    double accumulatedDistance = 0.0;
    double accumulatedCost = 0.0;
    
    printf("\n========== PROBLEM 2: CHEAPEST ROUTE (CAR + METRO) ==========\n");
    printf("Source: (%.6f, %.6f)\n", sourceLongitude, sourceLatitude);
    printf("Destination: (%.6f, %.6f)\n", destinationLongitude, destinationLatitude);
    printf("=============================================================\n\n");
    
    // If sourceVertex is not exactly on a node, show walking segment
    if (fabs(vertexArray[sourceVertex].lat - sourceLatitude) > 1e-6 || 
        fabs(vertexArray[sourceVertex].lon - sourceLongitude) > 1e-6) {
        double walkingDistance = calculateGeoDistance(sourceLatitude, sourceLongitude,
                                           vertexArray[sourceVertex].lat, vertexArray[sourceVertex].lon);
        printf("Walk originVertex Source (%.6f, %.6f) targetVertex %s (%.6f, %.6f)\n",
               sourceLongitude, sourceLatitude, vertexArray[sourceVertex].name, vertexArray[sourceVertex].lon, vertexArray[sourceVertex].lat);
        printf("  Distance: %.3f km, Cost: Tk 0.00\n\n", walkingDistance);
        accumulatedDistance += walkingDistance;
    }
    
    // Print each segment using the stored edge information
    for (int i = routeLength - 1; i > 0; i--) {
        int originVertex = routePath[i];
        int targetVertex = routePath[i - 1];
        int connectionIndex = routeConnections[i - 1];
        
        double connectionDistance = 0;
        TransportMode connectionTransportMode = TRANSPORT_CAR;
        
        if (connectionIndex >= 0 && connectionIndex < connectionCount) {
            connectionDistance = connectionArray[connectionIndex].distance;
            connectionTransportMode = connectionArray[connectionIndex].transportMode;
        }
        
        double costPerKilometer = (connectionTransportMode == TRANSPORT_METRO) ? METRO_COST_PER_KM : CAR_COST_PER_KM;
        double segmentCost = connectionDistance * costPerKilometer;
        accumulatedDistance += connectionDistance;
        accumulatedCost += segmentCost;
        
        printf("Ride %s originVertex %s (%.6f, %.6f) targetVertex %s (%.6f, %.6f)\n",
               retrieveTransportTypeName(connectionTransportMode),
               vertexArray[originVertex].name, vertexArray[originVertex].lon, vertexArray[originVertex].lat,
               vertexArray[targetVertex].name, vertexArray[targetVertex].lon, vertexArray[targetVertex].lat);
        printf("  Distance: %.3f km, Cost: Tk %.2f\n\n", connectionDistance, segmentCost);
    }
    
    // If destination is not exactly on a node, show walking segment
    if (fabs(vertexArray[destinationVertex].lat - destinationLatitude) > 1e-6 || 
        fabs(vertexArray[destinationVertex].lon - destinationLongitude) > 1e-6) {
        double walkingDistance = calculateGeoDistance(vertexArray[destinationVertex].lat, vertexArray[destinationVertex].lon,
                                           destinationLatitude, destinationLongitude);
        printf("Walk originVertex %s (%.6f, %.6f) targetVertex Destination (%.6f, %.6f)\n",
               vertexArray[destinationVertex].name, vertexArray[destinationVertex].lon, vertexArray[destinationVertex].lat, destinationLongitude, destinationLatitude);
        printf("  Distance: %.3f km, Cost: Tk 0.00\n\n", walkingDistance);
        accumulatedDistance += walkingDistance;
    }
    
    printf("=============================================================\n");
    printf("Total Distance: %.3f km\n", accumulatedDistance);
    printf("Total Cost: Tk %.2f\n", accumulatedCost);
    printf("=============================================================\n");
}

// Run Problem 2: Cheapest Route (Car and Metro)
void executeProblem2() {
    double sourceLatitude, sourceLongitude, destinationLatitude, destinationLongitude;
    
    printf("\n--- Problem 2: Cheapest Route (Car and Metro) ---\n");
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
    
    printf("\nUsing closestVertex vertexArray:\n");
    printf("Source Vertex: %s (%.6f, %.6f)\n", vertexArray[sourceVertex].name, 
           vertexArray[sourceVertex].lat, vertexArray[sourceVertex].lon);
    printf("Target Vertex: %s (%.6f, %.6f)\n", vertexArray[destinationVertex].name,
           vertexArray[destinationVertex].lat, vertexArray[destinationVertex].lon);
    
    // Dijkstra initialization - optimizing for COST
    for (int i = 0; i < vertexCount; i++) {
        distanceArray[i] = INFINITY_VALUE;
        predecessorArray[i] = -1;
        incomingConnectionArray[i] = -1;  // Track which edge was used
        processedArray[i] = 0;
    }
    distanceArray[sourceVertex] = 0;
    
    // Dijkstra's algorithm - optimizing for cost
    for (int tokenCount = 0; tokenCount < vertexCount; tokenCount++) {
        int u = -1;
        double minimumCost = INFINITY_VALUE;
        
        // Find unvisited node with minimum cost
        for (int i = 0; i < vertexCount; i++) {
            if (!processedArray[i] && distanceArray[i] < minimumCost) {
                minimumCost = distanceArray[i];
                u = i;
            }
        }
        
        if (u == -1 || u == destinationVertex) break;
        
        processedArray[u] = 1;
        
        // Relax connectionArray - considering CAR and METRO modes for Problem 2
        for (int i = 0; i < connectionCount; i++) {
            if (connectionArray[i].originVertex == u) {
                // Only consider CAR and METRO modes
                if (connectionArray[i].transportMode != TRANSPORT_CAR && connectionArray[i].transportMode != TRANSPORT_METRO) {
                    continue;
                }
                
                int v = connectionArray[i].targetVertex;
                double costPerKilometer = (connectionArray[i].transportMode == TRANSPORT_METRO) ? METRO_COST_PER_KM : CAR_COST_PER_KM;
                
                double connectionCost = connectionArray[i].distance * costPerKilometer;
                double updatedCost = distanceArray[u] + connectionCost;
                
                if (updatedCost < distanceArray[v]) {
                    distanceArray[v] = updatedCost;
                    predecessorArray[v] = u;
                    incomingConnectionArray[v] = i;  // Remember which edge we used
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
    
    printf("\nCheapest routePath found with cost: Tk %.2f\n", distanceArray[destinationVertex]);
    
    // Print detailed routePath
    displayProblem2Results(routePath, routeConnections, routeLength, sourceVertex, destinationVertex, sourceLatitude, sourceLongitude, destinationLatitude, destinationLongitude);
    
    // Export to KML
    char kmlFilename[256];
    snprintf(kmlFilename, sizeof(kmlFilename), "problem2_cheapest_car_metro_route.kml");
    exportRouteToKML(kmlFilename, routePath, routeLength, routeConnections,
                     "Problem 2: Cheapest Route (Car + Metro)",
                     "Most cost-effective route using car and metro");
}
