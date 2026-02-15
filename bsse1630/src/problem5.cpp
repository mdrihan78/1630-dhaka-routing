#include "../headers/problem5.h"
#include "../headers/nodesAndEdges.h"
#include "../headers/mode.h"
#include <cstdio>
#include <cmath>

// Problem 5: Simplified version - Fastest route with schedule
void executeProblem5() {
    double sourceLatitude, sourceLongitude, destinationLatitude, destinationLongitude;
    int departureHour, departureMinute;
    
    printf("\n--- Problem 5: Fastest Route with Schedule ---\n");
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
    
    // Dijkstra initialization - optimizing for TIME (distance/travelSpeed)
    for (int i = 0; i < vertexCount; i++) {
        distanceArray[i] = INFINITY_VALUE;
        predecessorArray[i] = -1;
        incomingConnectionArray[i] = -1;
        processedArray[i] = 0;
    }
    distanceArray[sourceVertex] = 0;
    
    // Speed constants (km/h)
    const double CAR_VELOCITY = 30.0;
    const double METRO_VELOCITY = 40.0;
    const double BUS_VELOCITY = 25.0;
    
    // Dijkstra's algorithm - optimizing for time
    for (int tokenCount = 0; tokenCount < vertexCount; tokenCount++) {
        int u = -1;
        double minimumTime = INFINITY_VALUE;
        
        for (int i = 0; i < vertexCount; i++) {
            if (!processedArray[i] && distanceArray[i] < minimumTime) {
                minimumTime = distanceArray[i];
                u = i;
            }
        }
        
        if (u == -1 || u == destinationVertex) break;
        
        processedArray[u] = 1;
        
        // Relax connectionArray - considering time = distance / travelSpeed
        for (int i = 0; i < connectionCount; i++) {
            if (connectionArray[i].originVertex == u) {
                if (connectionArray[i].transportMode != TRANSPORT_CAR && connectionArray[i].transportMode != TRANSPORT_METRO && 
                    connectionArray[i].transportMode != TRANSPORT_BIKOLPO && connectionArray[i].transportMode != TRANSPORT_UTTARA) {
                    continue;
                }
                
                int v = connectionArray[i].targetVertex;
                
                double travelSpeed = CAR_VELOCITY;
                if (connectionArray[i].transportMode == TRANSPORT_METRO) travelSpeed = METRO_VELOCITY;
                else if (connectionArray[i].transportMode == TRANSPORT_BIKOLPO || connectionArray[i].transportMode == TRANSPORT_UTTARA) 
                    travelSpeed = BUS_VELOCITY;
                
                double travelDuration = (connectionArray[i].distance / travelSpeed) * 60.0; // Convert targetVertex minutes
                double updatedTime = distanceArray[u] + travelDuration;
                
                if (updatedTime < distanceArray[v]) {
                    distanceArray[v] = updatedTime;
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
    
    int totalHours = (int)(distanceArray[destinationVertex] / 60);
    int totalMinutes = (int)(distanceArray[destinationVertex]) % 60;
    
    printf("\n========== PROBLEM 5: FASTEST ROUTE WITH SCHEDULE ==========\n");
    printf("Fastest routePath found with travel time: %d totalHours %d minutes\n", totalHours, totalMinutes);
    printf("Vehicle speeds: Metro 40 km/h, Car 30 km/h, Bus 25 km/h\n");
    printf("============================================================\n");
}
