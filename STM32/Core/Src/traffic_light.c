/*
 * traffic_light.c
 *
 * Traffic light output driver implementation
 */

#include "traffic_light.h"

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} light_pin_t;

// lightPins[road][color]
static const light_pin_t lightPins[NUM_OF_ROADS][3] = {
    { {RED1_GPIO_Port, RED1_Pin}, {AMBER1_GPIO_Port, AMBER1_Pin}, {GREEN1_GPIO_Port, GREEN1_Pin} },
    { {RED2_GPIO_Port, RED2_Pin}, {AMBER2_GPIO_Port, AMBER2_Pin}, {GREEN2_GPIO_Port, GREEN2_Pin} },
};

void setRoadColor(road_t road, light_color_t color) {
    if (road >= NUM_OF_ROADS) return;

    // Turn the old color off before the new one comes on
    for (int c = COLOR_RED; c <= COLOR_GREEN; c++) {
        if (c != (int)color) {
            HAL_GPIO_WritePin(lightPins[road][c].port, lightPins[road][c].pin, LIGHT_OFF);
        }
    }
    if (color <= COLOR_GREEN) {
        HAL_GPIO_WritePin(lightPins[road][color].port, lightPins[road][color].pin, LIGHT_ON);
    }
}

void setBothRoads(light_color_t color) {
    setRoadColor(ROAD_1, color);
    setRoadColor(ROAD_2, color);
}

void clearAllTrafficLights(void) {
    setBothRoads(COLOR_OFF);
}
