// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
#define PORT 8080

int main(int argc, char const *argv[])
{
    // int sock = 0, valread, client_fd;
    // struct sockaddr_in serv_addr;
    // char *hello = "Hello from client";
    // char buffer[1024] = {0};
    // if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    // {
    //     printf("\n Socket creation error \n");
    //     return -1;
    // }

    // serv_addr.sin_family = AF_INET;
    // serv_addr.sin_port = htons(PORT);

    // // Convert IPv4 and IPv6 addresses from text to binary
    // // form
    // if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    // {
    //     printf(
    //         "\nInvalid address/ Address not supported \n");
    //     return -1;
    // }

    // if ((client_fd = connect(sock, (struct sockaddr *)&serv_addr,
    //                          sizeof(serv_addr))) < 0)
    // {
    //     printf("\nConnection Failed \n");
    //     return -1;
    // }

    // printf("\nConnected with the server!!");

    // send(sock, hello, strlen(hello), 0);
    // printf("Hello message sent\n");
    // valread = read(sock, buffer, 1024);
    // printf("%s\n", buffer);

    // closing the connected socket
    //  close(client_fd);
    // return 0;

    int input_response;

    int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
    // char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }

    while (1)
    {

        cout << "\n Press 1 to open session with server or 0 to exit the program \n";

        cin >> input_response;

        if (input_response == 1)
        {

            if ((client_fd = connect(sock, (struct sockaddr *)&serv_addr,
                                     sizeof(serv_addr))) < 0)
            {
                printf("\nConnection Failed \n");
                return -1;
            }

            printf("\nConnected with the server!! b");

            while (1)
            {
                // cout << "\nblah blah... \n";
                int request_type = 1;
                int number_of_items;
                int upc_code;
                cout << "\nAdd an item(0) or close(1)\n";

                cin >> request_type;

                if (request_type == 0)
                {
                    cout << "\n Enter UPC code of the item to be added \n";
                    cin >> upc_code;

                    cout << "\n Enter number of items to be added \n";
                    cin >> number_of_items;

                    string request_string = to_string(request_type) + " " + to_string(upc_code) + " " + to_string(number_of_items);

                    send(sock, request_string.c_str(), strlen(request_string.c_str()), 0);

                    valread = read(sock, buffer, 1024);

                    cout << "\nResponse from server: " << buffer << "\n";
                }

                if (request_type == 1)
                {
                    cout << "\n Sending close request to the server!! \n";
                    send(sock, "1", 1, 0);

                    read(sock, buffer, 1024);

                    cout << "\nResponse from server: " << buffer << "\n";

                    break;
                }
            }
        }

        if (input_response == 0)
            exit(0);
    }

    return 0;
}