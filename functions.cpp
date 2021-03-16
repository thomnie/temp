#include "functions.h"
#include <cstring>
#include <string.h>
#include "mbed.h"
#include <cstdio>

void connect(){
    // Get pointer to default network interface
    NetworkInterface *network = NetworkInterface::get_default_instance();

    if (!network) {
      printf("Failed to get default network interface\n");
      while (1);
    }

    // Connect to network
    printf("\nConnecting to the network...\n");
    nsapi_size_or_error_t result = network->connect();

    // Check if the connection is successful
    if (result != 0) {
        printf("Failed to connect to network: %d\n", result);
        while(1);
    }

    printf("Connection to network successful!\n");

    // Create and allocate resources for socket
    TCPSocket socket;
    socket.open(network);

    // Create destination address
    SocketAddress address;

    // Get IP address of host by name
    result = network->gethostbyname("10.0.0.89", &address);

    // Check result
    if (result != 0) {
        printf("Failed to get IP address of host: %d\n", result);
        while(1);
    }

    printf("Got address of host\n");

    // Set server port number
    address.set_port(9090);

    // Connect to server at the given address
    result = socket.connect(address);

    // Check result
    if (result != 0) {
        printf("Failed to connect to server: %d\n", result);
        while(1);
    }

    printf("Connection to server successful!\n");

    // Send a simple http request
    char sbuffer[] =    "POST /api/v1/3977STDkNIUXJYXmqiuG/telemetry HTTP/1.1\r\n"
                        "Host: 10.0.0.89\r\n"
                        "Content-Type: application/json\r\n"
                        "Content-Length: 22\r\n"
                        "\r\n";
    int scount = socket.send(sbuffer, sizeof sbuffer);
    printf("sent %d [%.*s]\n", scount, strstr(sbuffer, "\r\n") - sbuffer, sbuffer);

    char http_post[] = "{\"Temperature\": 40.1}";

    int post_int = socket.send(http_post, sizeof http_post);
    printf("sent %d\n", post_int);

    socket.close();
    network->disconnect();

}