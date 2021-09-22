#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <math.h>

using namespace std;

#define PORT 8080

int main(int argc, char const *argv[])
{
    int valread, server_fd, new_socket;
    int opt = 1;
    int P = 23, G = 9, a, b, x, y, k;

    char *hello = "Hi client!";
    char buffer[1024] = {0};

    struct sockaddr_in address;

    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        cout << "ERROR::Socket failed\n";
        return -1;
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        cout << "ERROR::Could not attach socket to port\n";
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        cout << "ERROR::Bind failed\n";
        return -1;
    }
    if (listen(server_fd, 3) < 0)
    {
        cout << "ERROR::Listener set up failed\n";
        return -1;
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        cout << "ERROR::Could not accept connection\n";
        return -1;
    }

    valread = read(new_socket, buffer, 1024);
    cout << buffer << "\n";
    send(new_socket, hello, strlen(hello), 0);

    b = 4;
    //b = 1 + rand() % 10;
    cout << "INFO::Private key b = " << b << "\n";

    y = pow(G, b);
    y = y % P;
    send(new_socket, &y, sizeof(int), 0);
    valread = read(new_socket, &x, sizeof(int));
    cout << "INFO::Recieved generated key from client\n";

    k = pow(x, b);
    k = k % P;
    cout << "Secret Key Generated = " << k << "\n";

    return 0;
}
