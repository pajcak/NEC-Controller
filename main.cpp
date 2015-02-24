#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string>

#include "headers/MsgOutgoing.h"
#include "headers/CPacket.h"

using namespace std;

int openCliSocket(const char * name, int port) {
    struct addrinfo * ai;
    char portStr[10];

    /* Adresa, kde server posloucha. Podle name se urci typ adresy
     * (IPv4/6) a jeji binarni podoba
     */
    snprintf(portStr, sizeof ( portStr), "%d", port);
    if (getaddrinfo(name, portStr, NULL, &ai)) {
        printf("Getaddrinfo() failed...\n");
        return -1;
    }
    /* Otevreni soketu, typ soketu (family) podle navratove hodnoty getaddrinfo,
     * stream = TCP
     */
    int fd = socket(ai -> ai_family, SOCK_STREAM, 0);
    if (fd == -1) {
        freeaddrinfo(ai);
        printf("Socket() failed...\n");
        return -1;
    }
    /* Zadost o spojeni se serverem (ted se teprve zacne komunikovat).
     * vysledkem je bud otevrene datove spojeni nebo chyba.
     */
    if (connect(fd, ai -> ai_addr, ai -> ai_addrlen) == -1) {
        close(fd);
        freeaddrinfo(ai);
        printf("Connect() failed...\n");
        return -1;
    }
    printf("Connected to %s\n", name);
    freeaddrinfo(ai);
    return fd;
}

int main(int argc, char * argv []) {
    int cnt, delay;
    
    if (argc != 4
            || sscanf(argv[1], "%d", &cnt) != 1
            || sscanf(argv[2], "%d", &delay) != 1) {
        printf("<cnt> <delay> <str>\n");
        return 1;
    }
    //   Adresa a port, kde server nasloucha
    int fd = openCliSocket("localhost", 12345);

    // posilame zadana data, zadany pocet krat
    for (int i = 0; i < cnt; i++) {
        char buffer[200];
//        CPacket p; 
//        snprintf(buffer, sizeof(buffer), "%s", p.getBuffStr().c_str());

        //        int length = snprintf(buffer, sizeof ( buffer), "%d: %s\n", i, argv[3]);
//        if (length >= sizeof (buffer) || length < 0) {
//            /* error handling (buffer under/over-flow) */
//        }
        write(fd, buffer, strlen(buffer));

//        int l = read(fd, buffer, sizeof ( buffer));
//        write(STDOUT_FILENO, buffer, l);
        // prodleva mezi zaslanymi daty
        
/*The controller(here) should wait for a packet interval before next command is sent.
The packet interval needs to be longer than 600msec for the LCD monitor.*/
        usleep(delay * 1000);
    }
    // uzavreni spojeni klientem. Server zjisti uzavreni spojeni a
    // uvolni prostredky na sve strane.
    close(fd);
    return 0;
}
