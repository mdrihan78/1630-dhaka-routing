#include "../headers/mode.h"

const char* retrieveTransportTypeName(TransportMode transportMode) {
    switch (transportMode) {
        case TRANSPORT_WALK:
            return "Walk";
        case TRANSPORT_CAR:
            return "Car";
        case TRANSPORT_METRO:
            return "Metro";
        case TRANSPORT_BIKOLPO:
            return "Bikolpo Bus";
        case TRANSPORT_UTTARA:
            return "Uttara Bus";
        default:
            return "Unknown";
    }
}
