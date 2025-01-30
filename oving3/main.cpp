#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#define PORT 8080
using namespace std;

int main()
{
    int serverSocket, clientSocket;
    // Creating socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        return 0;
    }


    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listens to connections
    if (listen(serverSocket, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }


    cout << "Server is running on port" << PORT << endl;

    // Accepting connection request
    if ((clientSocket = accept(serverSocket, nullptr, nullptr)) < 0) {
        perror("Bind failure");
        exit(EXIT_FAILURE);
    }


    // Closing the socket.
    close(serverSocket);



    return 0;
}
