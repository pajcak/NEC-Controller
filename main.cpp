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
#include "headers/CController.h"

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
//    CController c;
//    c.getBrightness();
//    return 0;
//    snprintf(buffer, sizeof ( buffer), "%d: %s\n", i, "blablabla typu const char*");

        
/*The controller(here) should wait for a packet interval before next command is sent.
The packet interval needs to be longer than 600msec for the LCD monitor.*/
//        usleep(delay * 1000);
    return 0;
}
