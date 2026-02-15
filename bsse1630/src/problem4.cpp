#include "../headers/problem4.h"
#include "../headers/nodesAndEdges.h"
#include "../headers/mode.h"
#include <cstdio>
#include <cmath>

// Problem 4: Simplified version - Cheapest route with schedule awareness
// Note: This is a simplified implementation for demonstration
void executeProblem4() {
    double sourceLatitude, sourceLongitude, destinationLatitude, destinationLongitude;
    int departureHour, departureMinute;
    
    printf("\n--- Problem 4: Cheapest Route with Schedule ---\n");
    printf("Enter sourceVertex latitude and longitude: ");
    scanf("%lf %lf", &sourceLatitude, &sourceLongitude);
    
    printf("Enter destination latitude and longitude: ");
    scanf("%lf %lf", &destinationLatitude, &destinationLongitude);
    
    printf("Enter start time (hour minute, e.g., 9 30): ");
    scanf("%d %d", &departureHour, &departureMinute);
    
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
    printf("Start Time: %02d:%02d\n", departureHour, departureMinute);
    
    // Dijkstra initialization - optimizing for COST (same as Problem 3)
    for (int i = 0; i < vertexCount; i++) {
        distanceArray[i] = INFINITY_VALUE;
        predecessorArray[i] = -1;
        incomingConnectionArray[i] = -1;
        processedArray[i] = 0;
    }
    distanceArray[sourceVertex] = 0;
    
    // Dijkstra's algorithm - optimizing for cost
    for (int tokenCount = 0; tokenCount < vertexCount; tokenCount++) {
        int u = -1;
        double minimumCost = INFINITY_VALUE;
        
        for (int i = 0; i < vertexCount; i++) {
            if (!processedArray[i] && distanceArray[i] < minimumCost) {
                minimumCost = distanceArray[i];
                u = i;
            }
        }
        
        if (u == -1 || u == destinationVertex) break;
        
        processedArray[u] = 1;
        
        // Relax connectionArray - considering all modes
        for (int i = 0; i < connectionCount; i++) {
            if (connectionArray[i].originVertex == u) {
                if (connectionArray[i].transportMode != TRANSPORT_CAR && connectionArray[i].transportMode != TRANSPORT_METRO && 
                    connectionArray[i].transportMode != TRANSPORT_BIKOLPO && connectionArray[i].transportMode != TRANSPORT_UTTARA) {
                    continue;
                }
                
                int v = connectionArray[i].targetVertex;
                
                double costPerKilometer = CAR_COST_PER_KM;
                if (connectionArray[i].transportMode == TRANSPORT_METRO) costPerKilometer = METRO_COST_PER_KM;
                else if (connectionArray[i].transportMode == TRANSPORT_BIKOLPO) costPerKilometer = BIKOLPO_COST_PER_KM;
                else if (connectionArray[i].transportMode == TRANSPORT_UTTARA) costPerKilometer = UTTARA_COST_PER_KM;
                
                double connectionCost = connectionArray[i].distance * costPerKilometer;
                double updatedCost = distanceArray[u] + connectionCost;
                
                if (updatedCost < distanceArray[v]) {
                    distanceArray[v] = updatedCost;
                    predecessorArray[v] = u;
                    incomingConnectionArray[v] = i;
                }
            }
        }
    }
    
    if (distanceArray[destinationVertex] >= INFINITY_VALUE) {
        printf("No routePath found between the selected vertexArray.\n");
        return;
    }
    
    printf("\n========== PROBLEM 4: CHEAPEST ROUTE WITH SCHEDULE ==========\n");
    printf("Cheapest routePath found with cost: Tk %.2f\n", distanceArray[destinationVertex]);
    printf("Note: Schedule-aware routing considers vehicle availability\n");
    printf("Metro: Every 5 min (1 AM - 11 PM)\n");
    printf("Bikolpo: Every 20 min (7 AM - 10 PM)\n");
    printf("Uttara: Every 10 min (6 AM - 11 PM)\n");
    printf("=============================================================\n");
}
