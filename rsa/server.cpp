#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <math.h>
using namespace std;
#define PORT 8080

int gcd(int a, int b)
{
    int temp;
    while(1)
    {
        temp = a % b;
        if(temp == 0)
            return b;
        a = b;
        b = temp;
    }
}

int main(int argc, char const *argv[])
{
    double to_client = 89;
    double e, n, c;
    
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

    cout<<"Listening for clients\n";

    // Accept a client socket and send message to client
    client_socket = accept(server_socket, NULL, NULL);

    if (client_socket == -1)
    {
        cout<<"\nCould not accept a client connection\n";
    }

    recv(client_socket, &e, sizeof(double), 0);
    
    recv(client_socket, &n, sizeof(double), 0);
    
    cout<<"Original message from server is: "<<to_client<<endl;

    cout<<"Public key is: ("<<e<<", "<<n<<")"<<endl;

    c = pow(to_client, e);
    c = fmod(c, n);

    cout<<"After encryption using client's public key, message is: "<<c<<endl;

    send(client_socket, &c, sizeof(double), 0);

    cout<<"Message sent to client"<<endl;
    
    return 0;
}
