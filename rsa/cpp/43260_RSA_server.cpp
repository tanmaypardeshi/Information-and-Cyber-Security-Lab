#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<cstring>
#include<math.h>
#include<string>

using namespace std;

#define PORT 8080

int main(int argc, char const *argv[])
{
    int valread, server_fd, new_socket;
    int opt = 1;
    int e, n;
    double msgval, c;
    
    string message;
    char *hello = "Hi client! I'm your server.";
    char *pub = "Server has received the public key.";
    char buffer[1024] = {0};
    
    struct sockaddr_in address;
    
    int addrlen = sizeof(address);
    
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        cout<<"ERROR::Socket failed\n";
    	return -1;
    }
    
    // Forcefully attaching socket to the port 8080
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        cout<<"ERROR::Could not attach socket to port\n";
    	return -1;
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Forcefully attaching socket to the port 8080
    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        cout<<"ERROR::Bind failed\n";
    	return -1;
    }
    if(listen(server_fd, 3) < 0)
    {
        cout<<"ERROR::Listener set up failed\n";
    	return -1;
    }
    if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
    {
        cout<<"ERROR::Could not accept connection\n";
    	return -1;
    }
    
    valread = read(new_socket, buffer, 1024);
    cout<<buffer<<"\n";
    send(new_socket, hello, strlen(hello), 0);

    valread = read(new_socket, &e, 1024);
    cout<<"Encryption key for client e = "<<e<<"\n";
    send(new_socket, pub, strlen(pub), 0);
    
    valread = read(new_socket, &n, 1024);
    cout<<"Encryption key for client n = "<<n<<"\n";
    send(new_socket, pub, strlen(pub), 0);
    
    valread = read(new_socket, buffer, 1024);
    cout<<"\n"<<buffer<<"\n";
    
    cout<<"\nEnter a message for client:\n";
	cin>>message;
	
    for(int i=0; i<message.size(); i++)
    {
		msgval = message[i] - 'a';
		
		c = pow(msgval, e);
		c = fmod(c, n);
		
		cout<<"\nCharacter "<<message[i]<<" encrypted as "<<c<<"\n";
		
		send(new_socket, &c, sizeof(double), 0);
		valread = read(new_socket , buffer, 1024);
	}
	cout<<"\n"<<buffer<<"\n";
    
    return 0;
}
