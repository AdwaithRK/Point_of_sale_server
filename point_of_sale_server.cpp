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
using namespace std;
#define PORT 8080
#define PRICE_LIST "product_price.txt"

using namespace std;

int product_code_g = -1, product_quantity_g = -1;

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
    char *pch;
    char *current_product;
    int count = 0;
    // cout << "kooooiii\n";
    pch = strtok(price_line, " ");
    // cout << "ho ho \n";
    if (stoi(pch) == product_code)
    {
        pch = strtok(NULL, " ");
        pch = strtok(NULL, " ");
        return stoi(pch);
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
    char *pch;
    char *current_product;
    int count = 0;
    pch = strtok((char *)line.c_str(), " ");
    if (stoi(pch) == product_code)
    {
        pch = strtok(NULL, " ");
        return pch;
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

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    long valread;
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
            float total_price = 0;

            while (1)
            {
                char buffer[30000] = {0};
                int product_code, product_quantity, product_price;
                string product_name;
                char *pch;
                valread = read(new_socket, buffer, 30000);
                printf("-----------------Request--------------------");
                printf("\n content in buffer is : %s\n", buffer);
                if (buffer[0] == '0')
                {
                    getProductCodeAndProductQuantity(buffer);
                    product_code = product_code_g;
                    product_quantity = product_quantity_g;
                    product_price = getProductPrice(product_code);
                    product_name = getProductName(product_code);
                    cout << "Product price is : " << product_price << "\n";
                    cout << "Product quantity is : " << product_quantity << "\n";
                    total_price += product_price * product_quantity;
                    cout << "Total price is : " << total_price << "\n";
                    content = "0 product price: " + to_string(product_price) + " product name: " + product_name;
                    write(new_socket, content.c_str(), strlen(content.c_str()));
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

        // get_header(buffer);
        // close(new_socket);
    }

    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}