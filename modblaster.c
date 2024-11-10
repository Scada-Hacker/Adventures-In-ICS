#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define MODBUS_PORT 502
#define MODBUS_TIMEOUT_SEC 2
#define MODBUS_REQUEST_LEN 12

// Simple Modbus TCP request (Read Coils function)
unsigned char modbus_request[] = {
    0x00, 0x01, // Transaction ID
    0x00, 0x00, // Protocol ID
    0x00, 0x06, // Length
    0x01,       // Unit ID
    0x01,       // Function Code (Read Coils)
    0x00, 0x00, // Starting Address
    0x00, 0x01  // Quantity of Coils
};

// Function to scan a single IP for Modbus service
int scan_modbus(const char *ip_address) {
    int sock;
    struct sockaddr_in server_addr;
    unsigned char response[MODBUS_REQUEST_LEN];
    fd_set fds;
    struct timeval timeout;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(MODBUS_PORT);
    if (inet_pton(AF_INET, ip_address, &server_addr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        close(sock);
        return -1;
    }

    // Set timeout
    timeout.tv_sec = MODBUS_TIMEOUT_SEC;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    // Attempt to connect
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("No Modbus service on %s\n", ip_address);
        close(sock);
        return 0;
    }

    // Send Modbus request
    if (send(sock, modbus_request, sizeof(modbus_request), 0) < 0) {
        perror("Failed to send Modbus request");
        close(sock);
        return -1;
    }

    // Check for response
    FD_ZERO(&fds);
    FD_SET(sock, &fds);

    int res = select(sock + 1, &fds, NULL, NULL, &timeout);
    if (res > 0) {
        int len = recv(sock, response, sizeof(response), 0);
        if (len > 0) {
            printf("Modbus service found on %s\n", ip_address);
        } else {
            printf("No valid response from %s\n", ip_address);
        }
    } else {
        printf("No Modbus service on %s\n", ip_address);
    }

    // Clean up
    close(sock);
    return 0;
}

// Main function to scan a range of IPs
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <start_ip> <end_ip>\n", argv[0]);
        return 1;
    }

    struct in_addr start, end;
    if (inet_pton(AF_INET, argv[1], &start) <= 0 || inet_pton(AF_INET, argv[2], &end) <= 0) {
        perror("Invalid IP address format");
        return 1;
    }

    // Loop over the IP range
    for (unsigned long ip = ntohl(start.s_addr); ip <= ntohl(end.s_addr); ip++) {
        struct in_addr current;
        current.s_addr = htonl(ip);
        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &current, ip_str, INET_ADDRSTRLEN);
        scan_modbus(ip_str);
    }

    return 0;
}
