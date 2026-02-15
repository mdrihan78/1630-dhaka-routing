#ifndef MODE_H
#define MODE_H

// Earth radius for Haversine formula
#define EARTH_RADIUS_KILOMETERS 6371.0
#define PI 3.14159265358979323846

// Transport modes
typedef enum {
    TRANSPORT_WALK,
    TRANSPORT_CAR,
    TRANSPORT_METRO,
    TRANSPORT_BIKOLPO,  // Bikolpo Bus
    TRANSPORT_UTTARA    // Uttara Bus
} TransportMode;

// Cost rates (Taka per km)
#define CAR_COST_PER_KM 20.0
#define METRO_COST_PER_KM 5.0
#define BIKOLPO_COST_PER_KM 7.0
#define UTTARA_COST_PER_KM 10.0

// Function targetVertex get transportMode name as string
const char* retrieveTransportTypeName(TransportMode transportMode);

#endif
