#include "mbed.h"
#include <HTS221Sensor.h>

#include "functions.h"

Timer timer;

int main()
{
    float temperature, humidity;
    timer.start();

    DevI2C i2c(PB_11, PB_10);
    HTS221Sensor sensor(&i2c);

    sensor.init(nullptr);
    sensor.enable();
    
    connect_network();
    
    while (1) {
        if(timer.read_ms() >= 5000){
            sensor.get_temperature(&temperature);
            sensor.get_humidity(&humidity);
            
            connect_server(temperature, humidity);
            printf("Temperature: %.1f\nHumidity: %.1f\n\n", temperature, humidity);
            close_server();       
            timer.reset();
        }
    }
}


