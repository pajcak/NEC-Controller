#include <string>

#include "headers/CController.h"
#include <iostream>
#include <vector>

using namespace std;

vector <int> g_monitorIDs;

void printMenu() {
//    system("clear");
//    cout << "=======MENU======" << endl;
//    cout << "add <addr> <port> <monitorID>" << endl
//            << "--adds new monitor" << endl << endl;
//    cout << connectAll
}

int main(int argc, char * argv []) {
    CController c;

    printMenu();

    try {
        c.addMonitor("127.0.0.1", 12345, 1);
        c.connectAll();
        //==========================================
        //        printf("is connected= %s", (c.isConnected(1) ? "yes" : "no"));
        //==========================================
        //        printf("Actual brightness = %d\n", c.getBrightness());
        //==========================================
        //        if (argc != 3) printf("Usage: %s <brightness> <monitorAddr>\n", argv[0]);
        //        int brightness = atoi(argv[1]);
        //        char * monitorAddr = argv[2];
        //        CController c(monitorAddr);
        //        if (brightness < 0 ||  brightness > 100)
        //            printf("brightness range must be: 0(dark) to 100(bright) inclusive\n");
        /*else */c.setBrightness(1, 80);
        //==========================================
        //        switch (c.powerStatusRead(1)) {
        //            case 1: { printf("Power status = ON\n"); break;}
        //            case 2: { printf("Power status = Stand-by\n"); break;}
        //            case 3: { printf("Power status = Suspend\n"); break;}
        //            case 4: { printf("Power status = OFF\n"); break;}
        //        }
        //==========================================
        //        if (argc != 2) printf("Usage: %s <powerModeNumber>\n", argv[0]);
        //        int powerMode = atoi(argv[1]);
        //        if (powerMode != 1 && powerMode != 4) printf("powerModeNumber must be: 1(ON) or 4(OFF)\n");
        //        else c.powerControl(powerMode);
        //==========================================
        c.disconnectAll();
    } catch (const char * x) {
        printf("MAIN: caught:%s\n", x);
    }
    return 0;
}
