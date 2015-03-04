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

int main(int argc, char * argv []) {
    CController c;
    c.getBrightness();
//    return 0;
//    snprintf(buffer, sizeof ( buffer), "%d: %s\n", i, "blablabla typu const char*");

        
/*The controller(here) should wait for a packet interval before next command is sent.
The packet interval needs to be longer than 600msec for the LCD monitor.*/
//        usleep(delay * 1000);
    return 0;
}
