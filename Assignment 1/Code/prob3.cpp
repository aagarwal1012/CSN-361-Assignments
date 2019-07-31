/** @file prob3.cpp
 *  @brief Problem Statement 3 : Ping a server.
 *
 *  @author Ayush Agarwal, 17114017
 *  @date July 2019
    */
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

char *dns_lookup(char *addr_host, struct sockaddr_in *addr_con)
{
    printf("\nResolving DNS..\n");
    struct hostent *host_entity;
    char *ip = (char *)malloc(NI_MAXHOST * sizeof(char));
    int i;

    if ((host_entity = gethostbyname(addr_host)) == NULL)
    {
        // No ip found for hostname
        return NULL;
    }

    //filling up address structure
    strcpy(ip, inet_ntoa(*(struct in_addr *)
                              host_entity->h_addr));

    (*addr_con).sin_family = host_entity->h_addrtype;
    (*addr_con).sin_port = htons(0);
    (*addr_con).sin_addr.s_addr = *(long *)host_entity->h_addr;

    return ip;
}

int main(int argc, char *argv[])
{

    int sockfd;
    char *ip_addr, *reverse_hostname;
    struct sockaddr_in addr_con;

    if (argc != 2)
    {
        printf("\nIncorrect Format %s <address>\n", argv[0]);
        return 0;
    }

    ip_addr = dns_lookup(argv[1], &addr_con);
    // PING google.com (172.217.167.46) 56(84) bytes of data.

    printf("\nPING '%s' IP: %s\n", argv[1], ip_addr);

    // 1. Creating Socket
    int s = socket(PF_INET, SOCK_RAW, 1);

    //  -> Exit the app if the socket failed to be created
    if (s <= 0)
    {
        perror("Socket Error");
        exit(0);
    }

    // 2. Create the ICMP Struct Header
    typedef struct
    {
        uint8_t type;
        uint8_t code;
        uint16_t chksum;
        uint32_t data;
    } icmp_hdr_t;

    //  3. Use the newly created struct to make a variable.
    icmp_hdr_t pckt;

    //  4. Set the appropriate values to our struct, which is our ICMP header
    pckt.type = 8;        // The echo request is 8
    pckt.code = 0;        // No need
    pckt.chksum = 0xfff7; // Fixed checksum since the data is not changing
    pckt.data = 0;        // We don't send anything.

    //  5. Creating a IP Header from a struct that exists in another library
    printf("Packet size: %ld\n", sizeof(pckt));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = 0;
    addr.sin_addr.s_addr = inet_addr("ip_address");

    //  6. Send our PING
    int actionSendResult = sendto(s, &pckt, sizeof(pckt),
                                  0, (struct sockaddr *)&addr, sizeof(addr));

    //  -> Exit the app if the option failed to be set
    if (actionSendResult < 0)
    {
        perror("Ping Error");
        exit(0);
    }

    // 7. Prepare all the necessary variable to handle the response
    unsigned int resAddressSize;
    unsigned char res[30] = "";
    struct sockaddr resAddress;

    //  8. Read the response from the remote host
    int response = recvfrom(s, res, sizeof(res), 0, &resAddress,
                             &resAddressSize);

    //  -> Display the response in its raw form (hex)
    if (response > 0)
    {
        // 64 bytes from del03s16-in-f14.1e100.net (172.217.167.46): icmp_seq=3 ttl=50 time=46.1 ms
        printf("%d bytes from %s : %s\n", response, ip_addr, argv[1]);

        exit(0);
    }
    else
    {
        perror("Response Error");
        exit(0);
    }

    return 0;
}