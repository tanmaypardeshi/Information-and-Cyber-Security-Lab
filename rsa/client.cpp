#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <math.h>
using namespace std;

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

#define PORT 8080

int main(int argc, char const *argv[])
{
    int network_socket;
    double encrypted, decrypted;

     // Generation of public and private key
    double p = 3, q = 7;
    double n = p*q;

    double e = 2;
    double phi = (p-1)*(q-1);

    while(e < phi)
    {
        if(gcd(e, phi) == 1)
        {
            break;
        }
        else
        {
            e++;
        }
    }

    int k = 2;

    double d = (1 + (k*phi))/e;

    cout<<"(e,n) = ("<<e<<", "<<n<<")"<<endl;
    cout<<"(d,n) = ("<<d<<", "<<n<<")"<<endl;
    cout<<"phi = "<<phi<<endl;

    
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
  
    send(network_socket, &e, sizeof(double), 0);

    send(network_socket, &n, sizeof(double), 0);

    // Receive data from the server
    recv(network_socket, &encrypted, sizeof(double), 0);

    cout<<"Encrypted message from server is:- "<<encrypted<<endl;

    decrypted = pow(encrypted, d);
    decrypted = fmod(decrypted, n);

    cout<<"Original message from server is:- "<<decrypted<<endl;

    return 0;
}
