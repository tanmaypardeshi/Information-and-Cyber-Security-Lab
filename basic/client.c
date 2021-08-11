#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    int network_socket;

    // Create a network socket
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8000);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int connection = connect(network_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    if (connection == -1)
    {
        printf("\nThere was an error while making connection to the server\n");
    }

    // Send a number to the server
    char to_server[256];
    printf("Enter a number to send to the server:- ");
    scanf("%s", to_server);
    send(network_socket, &to_server, sizeof(to_server), 0);

    // Receive data from the server
    char from_server[256];
    recv(network_socket, &from_server, sizeof(from_server), 0);

    // Print data received from the server
    printf("Sum of the digits of %s is = %s\n", to_server, from_server);

    return 0;
}