#include "headers/CController.h"
#include "headers/MsgOutgoing.h"
#include "headers/CMonitor.h"
#include "headers/MsgIncoming.h"
#include "headers/Utils.h"
#include <cstdio>

#define DEBUG
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
    CMsgGetCurrParam msg(opCodePage, opCode);
    
    CAbstractMessage * gprMsg = m_monitor->getParameter(&msg);
    
    CMsgGetCurrParamReply * getParamReply = dynamic_cast<CMsgGetCurrParamReply*>(gprMsg);
    if (getParamReply == 0) {
        throw "CController::getBrightness(): dynamic_cast.";
    }
    
    int brightness = getParamReply->getCurrValue();

#ifdef DEBUG/*-------------------------------------------------------------*/
    printf ("MaxVal=  [%d]\n", getParamReply->getMaxValue());
    printf ("CurrVal= [%d]\n", brightness);
#endif /*-----------------------------------------------------------------*/
    
    if (gprMsg) delete gprMsg;
    return brightness;
}
void CController::setBrightness(int val) {
    unsigned char opCodePage[2] = {'0', '0'};/*operation code page for Brightness*/
    unsigned char opCode[2]     = {'1', '0'};/*operation code for Brightness*/

    unsigned char value[4];
    IntToFourBytes(val, value);
    CMsgSetParam msg (opCodePage, opCode, value);

    CAbstractMessage * sprMsg = m_monitor->setParameter(&msg);
    
    CMsgSetParamReply * setParamReply = dynamic_cast<CMsgSetParamReply*>(sprMsg);
    if (setParamReply == 0) {
        throw "CController::setBrightness(): bad dynamic_cast.";
    }
    
#ifdef DEBUG/*-------------------------------------------------------------*/
    printf("MaxVal: %d\n", setParamReply->getMaxValue());
    printf("CurrVal: %d\n", setParamReply->getCurrValue());
#endif /*-----------------------------------------------------------------*/
    
    if (setParamReply->getCurrValue() != val) 
        throw "CController::setBrightness(): incorrect confirm value in setParameterReply.";
    
    m_monitor->saveCurrentSettings();
    
    if (sprMsg) delete sprMsg;
}
int CController::powerStatusRead() {
    return m_monitor->powerStatusRead();
}
void CController::powerControl(int powerMode) {
    unsigned char mode;
    if (powerMode == 1) mode = '1';
    else if (powerMode == 4) mode = '4';
    else throw "CController::powerControl(int): invalid power mode number (1 or 4 allowed).";
    
    int repliedValue = m_monitor->powerControl(mode);
    if (powerMode != repliedValue)
        throw "CController::powerControl(int): incorrect confirm value from monitor.";
}
