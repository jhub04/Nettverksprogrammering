#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#define PORT 8080
using namespace std;

string HTTPResponse(const string &request) {
    string response;
    if (request.find("GET / HTTP/1.1") != string::npos) {
        response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
                   "<html><body><h1>Welcome to the Homepage</h1></body></html>";
    } else if (request.find("GET /page1 HTTP/1.1") != string::npos) {
        response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
                   "<html><body><h1>This is Page 1</h1></body></html>";
    } else if (request.find("GET /page2 HTTP/1.1") != string::npos) {
        response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
                   "<html><body><h1>This is Page 2</h1></body></html>";
    } else {
        response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n"
                   "<html><body><h1>404 Page Not Found</h1></body></html>";
    }
    return response;
}

int main()
{
    int serverSocket, clientSocket;
    char buffer[1023] = {};
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


    while (true) {
        // Accepting connection request
        if ((clientSocket = accept(serverSocket, nullptr, nullptr)) < 0) {
            perror("Bind failure");
            exit(EXIT_FAILURE);
        }

        // Read request
        read(clientSocket, buffer, sizeof(buffer));
        cout << "Recieved request:\n" << buffer << endl;

        string response = HTTPResponse(buffer);

        send(clientSocket, response.c_str(), response.size(), 0);

        // Closing the socket.
        close(clientSocket);
    }

    // Closing the socket.
    close(serverSocket);


    return 0;
}
