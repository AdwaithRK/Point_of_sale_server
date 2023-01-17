// Server side C program to demonstrate HTTP Server programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;
#define PORT 8080
#define PRICE_LIST "product_price.txt"

using namespace std;

int getProductCode(char *buffer)
{
    istringstream ss(buffer);
    string product_id;

    for (int i = 0; i < 2; i++)
    {
        ss >> product_id;
    }

    return stoi(product_id);
}

int getProductQuantity(char *buffer)
{
    istringstream ss(buffer);
    string product_quantity;

    for (int i = 0; i < 3; i++)
    {
        ss >> product_quantity;
    }

    return stoi(product_quantity);
}

int checkPrice(char *price_line, int product_code)
{
    istringstream ss(price_line);
    string product_code_in_line;
    string price;

    for (int i = 0; i < 1; i++)
    {
        ss >> product_code_in_line;
    }

    if (stoi(product_code_in_line) == product_code)
    {

        for (int i = 0; i < 2; i++)
        {
            ss >> price;
        }
        return stoi(price);
    }

    return -1;
}

int getProductPrice(int product_code)
{
    ifstream file;
    file.open(PRICE_LIST);

    if (!file.is_open())
    {
        cout << "Unable to open the file." << endl;
        return 0;
    }

    string line;
    while (getline(file, line) && !line.empty())
    {
        cout << "line is " << line << "\n";
        if (line.length() > 0)
        {
            int price = checkPrice((char *)line.c_str(), product_code);
            if (price != -1)
                return price;
        }
    }

    file.close();

    return 0;
}

string checkName(string line, int product_code)
{
    istringstream ss(line);
    string product_code_in_line;
    string name;

    for (int i = 0; i < 1; i++)
    {
        ss >> product_code_in_line;
    }

    if (stoi(product_code_in_line) == product_code)
    {
        for (int i = 0; i < 1; i++)
        {
            ss >> name;
        }
        return name;
    }

    return "\0";
}

string getProductName(int product_code)
{
    ifstream file;
    file.open(PRICE_LIST);

    if (!file.is_open())
    {
        cout << "Unable to open the file." << endl;
        return 0;
    }

    string line;
    while (getline(file, line))
    {
        string name = checkName((char *)line.c_str(), product_code);
        if (strcmp(name.c_str(), "\0") != 0)
            return name;
    }

    file.close();

    return 0;
}

vector<int> cacheValidProductId()
{
    ifstream file;
    file.open(PRICE_LIST);
    vector<int> cachedPriceList;

    if (!file.is_open())
    {
        cout << "Unable to open the file." << endl;
        return cachedPriceList;
    }

    string line;
    while (getline(file, line))
    {
        istringstream ss(line);
        string product_code_in_line;

        ss >> product_code_in_line;

        cachedPriceList.push_back(stoi(product_code_in_line));
    }

    file.close();

    return cachedPriceList;
}

void serviceRequest(int new_socket)
{
    float total_price = 0;
    vector<int> cached_valid_ids = cacheValidProductId();
    long valread;

    while (1)
    {
        char buffer[30000] = {0};
        int product_code, product_quantity, product_price;
        string product_name, content;
        char *pch;
        valread = read(new_socket, buffer, 30000);
        printf("-----------------Request--------------------");
        printf("\n content in buffer is : %s\n", buffer);
        if (buffer[0] == '0')
        {
            product_code = getProductCode(buffer);
            product_quantity = getProductQuantity(buffer);
            if (find(cached_valid_ids.begin(), cached_valid_ids.end(), product_code) == cached_valid_ids.end())
            {
                content = "1 product_code: " + to_string(product_price) + " NOT_FOUND!!";
                write(new_socket, content.c_str(), strlen(content.c_str()));
            }
            else
            {
                product_price = getProductPrice(product_code);
                product_name = getProductName(product_code);
                cout << "Product price is : " << product_price << "\n";
                cout << "Product quantity is : " << product_quantity << "\n";
                total_price += product_price * product_quantity;
                cout << "Total price is : " << total_price << "\n";
                content = "0 product price: " + to_string(product_price) + " product name: " + product_name;
                write(new_socket, content.c_str(), strlen(content.c_str()));
            }
            // extraction from client message is done
        }

        if (buffer[0] == '1')
        {
            content = "0 total price: " + to_string(total_price);
            write(new_socket, content.c_str(), strlen(content.c_str()));
            close(new_socket);
            break;
        }

        printf("--------------------------------------------\n");
    }
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    string content = "";

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
            int childpid;
            if ((childpid = fork()) == 0)
            {
                printf("\nRequest Serviced with child process %d\n", getpid());
                serviceRequest(new_socket);
                close(new_socket); //
                exit(0);           // child exiting after servicing the request
            }
        }

        close(new_socket);
    }

    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}