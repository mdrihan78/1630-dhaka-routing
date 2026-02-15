#include <cstdio>
#include <cstdlib>
#include "headers/mode.h"
#include "headers/nodesAndEdges.h"
#include "headers/csvParse.h"
#include "headers/kmlExport.h"
#include "headers/problem1.h"
#include "headers/problem2.h"
#include "headers/problem3.h"
#include "headers/problem4.h"
#include "headers/problem5.h"
#include "headers/problem6.h"

int main() {
    printf("========================================\n");
    printf("   Dhaka Transportation Route Finder\n");
    printf("========================================\n\n");
    
    // Load all CSV files
    printf("Loading transportation network...\n");
    loadRoadNetworkData("Dataset/Roadmap-Dhaka.csv");
    loadMetroNetworkData("Dataset/Routemap-DhakaMetroRail.csv");
    loadBusNetworkData("Dataset/Routemap-BikolpoBus.csv", TRANSPORT_BIKOLPO);
    loadBusNetworkData("Dataset/Routemap-UttaraBus.csv", TRANSPORT_UTTARA);
    
    if (vertexCount == 0) {
        printf("No vertexArray loaded. Please check your CSV files.\n");
        return 1;
    }
    
    printf("\nGraph loaded successfully!\n");
    printf("Total vertexArray: %d\n", vertexCount);
    printf("Total connectionArray: %d\n\n", connectionCount);
    
    // Main menu loop
    while (true) {
        printf("\n========================================\n");
        printf("          Mr Efficient Menu\n");
        printf("========================================\n");
        printf("[1] Shortest Car Route [Problem 1]\n");
        printf("[2] Cheapest Route (Car and Metro) [Problem 2]\n");
        printf("[3] Cheapest Route (All Modes) [Problem 3]\n");
        printf("[4] Cheapest Route with Schedule [Problem 4]\n");
        printf("[5] Fastest Route with Schedule [Problem 5]\n");
        printf("[6] Cheapest Route with Deadline [Problem 6]\n");
        printf("[7] Export All Networks to KML\n");
        printf("[8] Quit\n");
        printf("========================================\n");
        
        int menuSelection;
        printf("Enter Choice: ");
        scanf("%d", &menuSelection);
        
        if (menuSelection == 8) {
            printf("\nThank you for using Mr Efficient!\n");
            break;
        }
        
        switch (menuSelection) {
            case 1:
                executeProblem1();
                break;
            case 2:
                executeProblem2();
                break;
            case 3:
                executeProblem3();
                break;
            case 4:
                executeProblem4();
                break;
            case 5:
                executeProblem5();
                break;
            case 6:
                executeProblem6();
                break;
            case 7:
                exportAllNetworksToKML(".");
                break;
            default:
                printf("\nInvalid menuSelection. Please try again.\n");
                break;
        }
    }
    
    return 0;
}
