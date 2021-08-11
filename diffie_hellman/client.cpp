#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <math.h>

using namespace std;

#define PORT 8080

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    int P = 23, G = 9, a, b, x, y, k;

    char *hello = "Hi, this is client!";
    char buffer[1024] = {0};

    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout << "ERROR:: Couldn't create socket\n";
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        cout << "ERROR:: Invalid address\n";
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cout << "ERROR::Connection Failed\n";
        return -1;
    }

    send(sock, hello, strlen(hello), 0);
    valread = read(sock, buffer, 1024);
    cout << buffer << "\n";

    a = 3;
    //a = 1 + rand() % 10;
    cout << "INFO::Private key a = " << a << "\n";

    x = pow(G, a);
    x = x % P;
    send(sock, &x, sizeof(int), 0);
    valread = read(sock, &y, sizeof(int));
    cout << "INFO::Recieved generated key from server\n";

    k = pow(y, a);
    k = k % P;
    cout << "Secret Key Generated = " << k << "\n";

    return 0;
}
