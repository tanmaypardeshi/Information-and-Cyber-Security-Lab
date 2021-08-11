#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int get_sum(int number)
{
    int ans = 0;
    while (number > 0)
    {
        ans += number % 10;
        number /= 10;
    }
    return ans;
}

int main()
{
    char to_client[256], from_client[256];
    int number, answer;
    // Create the server socket
    int server_socket, client_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Define server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8000);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Call the bind function
    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    // Call the listen function
    listen(server_socket, 5);

    printf("Listening for clients\n");

    // Accept a client socket and send message to client
    client_socket = accept(server_socket, NULL, NULL);

    if (client_socket == -1)
    {
        printf("\nCould not accept a client connection\n");
    }

    recv(client_socket, &from_client, sizeof(from_client), 0);

    number = atoi(from_client);

    printf("\nReceived number %d from client\n", number);

    answer = get_sum(number);

    sprintf(to_client, "%d", answer);

    send(client_socket, to_client, sizeof(to_client), 0);

    printf("\nAnswer sent to client\n");

    return 0;
}