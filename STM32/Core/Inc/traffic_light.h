/*
 * traffic_light.h
 *
 * Traffic light output driver.
 * LED cathodes are driven by PA1-PA6 with anodes tied to +3.3 V,
 * so a LOW output turns an LED on (active-low).
 */

#ifndef INC_TRAFFIC_LIGHT_H_
#define INC_TRAFFIC_LIGHT_H_

#include "main.h"

#define LIGHT_ON        GPIO_PIN_RESET
#define LIGHT_OFF       GPIO_PIN_SET

typedef enum {
    ROAD_1 = 0,
    ROAD_2,
    NUM_OF_ROADS
} road_t;

typedef enum {
    COLOR_RED = 0,
    COLOR_AMBER,
    COLOR_GREEN,
    COLOR_OFF
} light_color_t;

// Show exactly one color (or none) on a road
void setRoadColor(road_t road, light_color_t color);

// Show the same color (or none) on both roads
void setBothRoads(light_color_t color);

void clearAllTrafficLights(void);

#endif /* INC_TRAFFIC_LIGHT_H_ */
