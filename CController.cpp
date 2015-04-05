#include "headers/CController.h"
#include "headers/CPacket.h"
#include "headers/MsgOutgoing.h"
#include "headers/CMonitor.h"
#include "headers/MsgIncoming.h"
#include "headers/Utils.h"
#include <cstdio>
CController::CController() {
    //temporary
//    m_monitor = new CMonitor("10.0.0.100", 7142);
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

    gprMsg = m_monitor->getParameter(msg);

    CMsgGetCurrParamReply * getParamReply = dynamic_cast<CMsgGetCurrParamReply*>(gprMsg);
    if (getParamReply == 0) {
        delete msg;
        throw "CController::getBrightness(): dynamic_cast.";
    }
    
    int brightness = getParamReply->getCurrValue();
    printf ("MaxVal=  [%d]\n", getParamReply->getMaxValue());
    printf ("CurrVal= [%d]\n", brightness);
    if (gprMsg) delete gprMsg;
    delete msg;
    return brightness;
}
void CController::setBrightness(const int & val) {
    unsigned char opCodePage[2] = {'0', '0'};/*operation code page for Brightness*/
    unsigned char opCode[2]     = {'1', '0'};/*operation code for Brightness*/

    unsigned char value[4];
    IntToFourBytes(val, value);
    CMsgSetParam msg (opCodePage, opCode, value);
    CAbstractMessage * sprMsg;

    sprMsg = m_monitor->setParameter(&msg);
    
    CMsgSetParamReply * setParamReply = dynamic_cast<CMsgSetParamReply*>(sprMsg);
    if (setParamReply == 0) {
        throw "CController::setBrightness(): dynamic_cast.";
    }
    
    printf("MaxVal: %d\n", setParamReply->getMaxValue());
    printf("CurrVal: %d\n", setParamReply->getCurrValue());
    
    if (setParamReply->getCurrValue() != val) 
        throw "CController::setBrightness(): incorrect confirm value in setParameterReply";
    
    m_monitor->saveCurrentSettings();
    
    if (sprMsg) delete sprMsg;
}
int CController::powerStatusRead() {
    return m_monitor->powerStatusRead();
}
