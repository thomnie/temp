#include "ThisThread.h"
#include "mbed.h"
#include <HTS221Sensor.h>
#include <cstdio>

#include "functions.h"

int main()
{
    float temperature;
    DevI2C i2c(PB_11, PB_10);
    HTS221Sensor sensor(&i2c);

    sensor.init(nullptr);
    sensor.enable();

    sensor.get_temperature(&temperature);

    connect();

}

