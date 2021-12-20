#include<iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<cstring>
#include<math.h>
#include<string>

using namespace std;

#define PORT 8080

int gcd(int x, int y)
{
    int temp;
    
    while(1)
    {
        temp = x % y;
        if(temp == 0)
          return y;
          
        x = y;
        y = temp;
    }
}

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    int p = 3, q = 7, n, phi, e, d, k;
    double msgval, c;
    
    string message = "";
    char *hello = "Hi, I'm a client!";
    char *ready = "Client is ready for server.";
    char *received = "Client has recieved the message.";
    char buffer[1024] = {0};
    
    struct sockaddr_in serv_addr;
    
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
    	cout<<"ERROR:: Couldn't create socket\n";
    	return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) 
    {
        cout<<"ERROR:: Invalid address\n";
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cout<<"ERROR::Connection Failed\n";
        return -1;
    }
    
    send(sock, hello, strlen(hello), 0);
    valread = read(sock , buffer, 1024);
    cout<<"\n"<<buffer<<"\n";
    
    n = p * q;
    phi = (p-1) * (q-1);
    
    e = 2;
    
    while(e < phi)
    {
        if(gcd(e, phi) == 1)
            break;
        else
            e++;
    }
    
    cout<<"Public key generated: "<<e<<"\n";
    cout<<"n = "<<n<<" phi = "<<phi<<"\n";
    
    send(sock, &e, sizeof(int), 0);
    valread = read(sock , buffer, 1024);
    
    send(sock, &n, sizeof(int), 0);
    valread = read(sock , buffer, 1024);
    cout<<"\n"<<buffer<<"\n";
    
    k = 2;
    d = (1 + (k*phi))/e;
    cout<<"d = "<<d<<"\n";
    
    cout<<"Private key generated.\n";
    send(sock, ready, strlen(ready), 0);
    
    while((valread = read(sock , &c, 1024)))
    {
		cout<<"Encrypted message received: "<<c<<"\n";
		
		msgval = pow(c, d);
		msgval = fmod(msgval, n);
		
		message += msgval + 'a';
		
		cout<<"\nDecrypted message: "<<message<<"\n";
		
		send(sock, received, strlen(received), 0);
    }
    
    
    
    return 0;
}
