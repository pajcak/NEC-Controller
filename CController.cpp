#include "headers/CController.h"
#include "headers/CPacket.h"
#include "headers/MsgOutgoing.h"
#include "headers/CMonitor.h"
#include "headers/MsgIncoming.h"

#include <cstdio>
CController::CController() {
    //temporary
    m_monitor = new CMonitor("localhost", 12345);
    m_monitor->establishConnection();
}

CController::~CController() {
    m_monitor->disconnect();
    delete m_monitor;
}

int  CController::getBrightness() {
    unsigned char opCodePage[2] = {'0', '0'};/*operation code page for Brightness*/
    unsigned char opCode[2]     = {'1', '0'};/*operation code for Brightness*/
    CAbstractMessage * msg = new CMsgGetCurrParam(opCodePage, opCode);
    CAbstractMessage * gprMsg;
    try {
        gprMsg = m_monitor->getParameter(msg);
    } catch (const char * s) {
        printf("CAUGHT: %s", s);
        //here i have to throw exception higher
        throw s;
    }
    if (!gprMsg) throw "CController::getBrightness(): NullObject received.";
    
    CMsgGetCurrParamReply * getParamReply = dynamic_cast<CMsgGetCurrParamReply*>(gprMsg);
    if (getParamReply == 0) throw "CController::getBrightness(): dynamic_cast.";
    
    int brightness = getParamReply->getCurrValue();
    printf ("MaxVal=  [%d]\n", getParamReply->getMaxValue());
    printf ("CurrVal= [%d]\n", brightness);
    if (gprMsg) delete gprMsg;
    delete msg;
    return brightness;
}
bool CController::setBrightness(const int & val) {
    //TODO
    return false;
}
