#include <string.h>
#include "mbed.h"

#include "functions.h"

NetworkInterface *network = NetworkInterface::get_default_instance();
TCPSocket socket;
nsapi_size_or_error_t result = network->connect();


void connect_network(){

    if (!network) {
      printf("Failed to get default network interface\n");
      while (1);
    }

    printf("\nConnecting to the network...\n");

    if (result != 0) {
        printf("Failed to connect to network: %d\n", result);
        while(1);
    }

    printf("Connection to network successful!\n");

}

void connect_server(float temperature, float humidity){

    socket.open(network);
    SocketAddress address;

    result = network->gethostbyname("10.0.0.89", &address);

    if (result != 0) {
        printf("Failed to get IP address of host: %d\n", result);
        while(1);
    }

    printf("Got address of host\n");

    address.set_port(9090);

    result = socket.connect(address);

    if (result != 0) {
        printf("Failed to connect to server: %d\n", result);
        while(1);
    }

    printf("Connection to server successful!\n");

    char sbuffer[150];
    char temp_post[50];
    
    snprintf(temp_post, sizeof(temp_post), "{\"Temperature\": %.1f, \"Humidity\": %.1f}", temperature, humidity); 

    snprintf(sbuffer, sizeof(sbuffer),  "POST /api/v1/3977STDkNIUXJYXmqiuG/telemetry HTTP/1.1\r\n"
                                        "Host: 10.0.0.89\r\n"
                                        "Content-Type: application/json\r\n"
                                        "Content-Length: %d\r\n"
                                        "\r\n", strlen(temp_post));

    nsapi_size_t bytes_to_send = strlen(sbuffer);
    nsapi_size_or_error_t scount = 0;
    while(bytes_to_send){
        scount = socket.send(sbuffer + scount, bytes_to_send);
        if(scount < 0) 
            while(1);
        else
            printf("sent %d [%.*s]\n", scount, strstr(sbuffer, "\r\n") - sbuffer, sbuffer);
        
        bytes_to_send -= scount;
    }

    int temp_int_sent = socket.send(temp_post, strlen(temp_post));

    printf("sent %d\n\n", temp_int_sent);
    
    socket.close();


}

void close_server(){
    socket.close();
    // network->disconnect();
}

