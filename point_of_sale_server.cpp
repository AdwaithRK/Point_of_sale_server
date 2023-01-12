// Server side C program to demonstrate HTTP Server programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;
#define PORT 8080
#define PRICE_LIST "product_price.txt"

int getProductCode(char *buffer)
{
    char *pch;
    int count = 0;
    pch = strtok(buffer, " ");
    while (pch != NULL)
    {
        pch = strtok(NULL, " ");
        count++;
        if (count == 1)
            return stoi(pch);
    }

    return 0;
}

int getProductQuantity(char *buffer)
{
    char *pch;
    int count = 0;
    pch = strtok(buffer, " ");
    while (pch != NULL)
    {
        pch = strtok(NULL, " ");
        count++;
        if (count == 2)
            return stoi(pch);
    }
}

int getProductPrice(int product_code)
{
    ifstream file;
    file.open("product_price.txt");

    if (!file.is_open())
    {
        cout << "Unable to open the file." << endl;
        return 0;
    }

    string line;
    while (getline(file, line))
    {
        cout << line << endl;
    }

    file.close();
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    string content = "adwaith's server is up and running!!!";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        else
        {
            while (1)
            {
                char buffer[30000] = {0};
                char product_code[3];
                char *pch;
                valread = read(new_socket, buffer, 30000);
                printf("-----------------Request--------------------");
                printf("\n quantity in buffer is : %s\n", buffer);
                if (buffer[0] == '0')
                {
                    cout << "Product code is : " << getProductCode(buffer) << "\n";
                    cout << "Product quantity is : " << getProductQuantity(buffer) << "\n";
                }

                printf("--------------------------------------------\n");
                write(new_socket, content.c_str(), strlen(content.c_str()));
            }
        }

        // get_header(buffer);
        // close(new_socket);
    }

    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}