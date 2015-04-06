#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string>

#include "headers/MsgOutgoing.h"
#include "headers/CController.h"

using namespace std;

int main(int argc, char * argv []) {
    CController c;
    try {
//        c.getBrightness();
//==========================================
//        c.setBrightness(80);
//==========================================
//        switch (c.powerStatusRead()) {
//            case 1: { printf("Power status = ON\n"); break;}
//            case 2: { printf("Power status = Stand-by\n"); break;}
//            case 3: { printf("Power status = Suspend\n"); break;}
//            case 4: { printf("Power status = OFF\n"); break;}
//        }
//==========================================
//        printf("PowerStatus: %d\n", c.powerStatusRead());
//==========================================
        c.powerControl(1);
//==========================================
    } catch (const char * x) {
        printf("MAIN: caught:%s\n", x);
    }
//    return 0;
//    snprintf(buffer, sizeof ( buffer), "%d: %s\n", i, "blablabla typu const char*");

        
/*The controller(here) should wait for a packet interval before next command is sent.
The packet interval needs to be longer than 600msec for the LCD monitor.*/
//        usleep(delay * 1000);
    return 0;
}
