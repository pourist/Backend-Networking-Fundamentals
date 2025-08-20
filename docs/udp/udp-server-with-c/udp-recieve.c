#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void) {
    const int port = 5501;
    int sockfd = -1;

    // 1) create socket (IPv4, UDP)
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    // allow quick rebinding after restarts
    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("setsockopt(SO_REUSEADDR)");
        // not fatal; continuing
    }

    // comment out for infinite blocking
    /*
    struct timeval tv = { .tv_sec = 10, .tv_usec = 0 };
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1) {
        perror("setsockopt(SO_RCVTIMEO)");
    }
    */

    // 2) prepare local address (127.0.0.1:5501)
    struct sockaddr_in myaddr;
    memset(&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(port);

    // use modern converter (inet_pton), check result
    if (inet_pton(AF_INET, "127.0.0.1", &myaddr.sin_addr) != 1) {
        fprintf(stderr, "inet_pton failed for 127.0.0.1\n");
        close(sockfd);
        return 1;
    }

    // 3) bind (start listening on that IP:port)
    if (bind(sockfd, (struct sockaddr*)&myaddr, sizeof(myaddr)) == -1) {
        perror("bind");
        close(sockfd);
        return 1;
    }

    // 4) receive one datagram
    char buffer[1024];
    struct sockaddr_in remoteAddr;
    socklen_t addr_size = sizeof(remoteAddr);
    ssize_t n = recvfrom(sockfd,
                         buffer,
                         sizeof(buffer) - 1, // leave room for '\0'
                         0,
                         (struct sockaddr*)&remoteAddr,
                         &addr_size);
    if (n == -1) {
        perror("recvfrom");
        close(sockfd);
        return 1;
    }

    // 5) make payload safe to print as a C-string
    buffer[n] = '\0';

    // 6) render sender IP:port
    char sender_ip[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &remoteAddr.sin_addr, sender_ip, sizeof(sender_ip)) == NULL) {
        // fallback if conversion fails
        strncpy(sender_ip, "<?>", sizeof(sender_ip));
        sender_ip[sizeof(sender_ip)-1] = '\0';
    }
    unsigned sender_port = (unsigned)ntohs(remoteAddr.sin_port);

    // 7) print what we got
    printf("Got %zd bytes from %s:%u: \n%s\n", n, sender_ip, sender_port, buffer);

    // 8) clean up
    close(sockfd);
    return 0;
}
