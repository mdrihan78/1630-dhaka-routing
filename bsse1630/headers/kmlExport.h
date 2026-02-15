#ifndef KML_EXPORT_H
#define KML_EXPORT_H

#include "nodesAndEdges.h"

// KML export functions
void exportRouteToKML(const char* outputFilePath, 
                      int routePath[], 
                      int routeLength,
                      int routeConnections[],
                      const char* routeName,
                      const char* routeDescription);

void exportNetworkToKML(const char* outputFilePath,
                        TransportMode filterMode,
                        const char* networkName);

void exportAllNetworksToKML(const char* outputDirectory);

// Helper function to get color for transport mode
const char* getTransportModeColor(TransportMode transportMode);

// Helper function to get style for transport mode
const char* getTransportModeStyle(TransportMode transportMode);

#endif
