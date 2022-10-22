#include <gps.h>
#include "test.h"
#include "oled.h"
#include "defines.h"


int ground_speed;

void setup() {
    if (ProcessGPS()) {
        ground_speed++;
    }
}

void loop() {
}