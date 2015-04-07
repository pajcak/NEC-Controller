#include "headers/CController.h"
#include "headers/MsgOutgoing.h"
#include "headers/CMonitor.h"
#include "headers/MsgIncoming.h"
#include "headers/Utils.h"
#include <cstdio>
#include <arpa/inet.h>
#define DEBUG
CController::CController() {
    //temporary
    initParameters();
}

CController::~CController() {
    //REPLACE THIS WITH METHOD LIKE CLEANUP
    for (std::map<int, CMonitor*>::iterator it = m_monitors.begin();
            it != m_monitors.end(); ++it)
    {
        it->second->disconnect();
        delete it->second;
    }
}
void CController::addMonitor (const char * monitorAddr, int port,  int monitorID) {
    struct sockaddr_in sa;
    if (inet_pton(AF_INET, monitorAddr, &(sa.sin_addr)) != 1)
        throw "CController::addMonitor: invalid monitorAddr.";
    if (port <= 1024 || port > 65535) throw "CController::addMonitor: invalid port.";
    m_monitors.insert(std::pair<int, CMonitor*>(monitorID, new CMonitor(monitorAddr, port)));
}
void CController::connectAll() {
    //What if some of them return false in establishConnection ??
    for (std::map<int, CMonitor*>::iterator it = m_monitors.begin();
            it != m_monitors.end(); ++it)
    {
        it->second->establishConnection();
    }
}
void CController::disconnectAll() {
    for (std::map<int, CMonitor*>::iterator it = m_monitors.begin();
            it != m_monitors.end(); ++it)
    {
        it->second->disconnect();
    }
    
}
bool CController::isConnected(int monitorID) {
    std::map<int, CMonitor*>::iterator it = m_monitors.find(monitorID);
    if (it == m_monitors.end()) throw "CController::isConnected: invalid monitor ID.";
    return it->second->isConnected();
}

//-----------------------API----------------------------
int  CController::getBrightness() {
    std::map<std::string, CParameter>::iterator it = m_parameters.find("brightness");

    CMsgGetCurrParam msg(it->second.m_opCodePage, it->second.m_opCode);
    
    CAbstractMessage * gprMsg = m_monitors[1]->getParameter(&msg);
    
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
    std::map<std::string, CParameter>::iterator it = m_parameters.find("brightness");

    if (it == m_parameters.end()) throw "ASD";

    if (!it->second.m_range->contains(val))
        throw "CController::setBrightness(): requested value out of range.";
    
    unsigned char value[4];
    IntToFourBytes(val, value);
    
    CMsgSetParam msg (it->second.m_opCodePage, it->second.m_opCode, value);

    CAbstractMessage * sprMsg = m_monitors[1]->setParameter(&msg);
    
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
    
    m_monitors[1]->saveCurrentSettings();
    
    if (sprMsg) delete sprMsg;
}
int CController::powerStatusRead() {
    return m_monitors[1]->powerStatusRead();
}
void CController::powerControl(int powerMode) {
    unsigned char mode;
    if (powerMode == 1) mode = '1';
    else if (powerMode == 4) mode = '4';
    else throw "CController::powerControl(int): invalid power mode number (1 or 4 allowed).";
    
    int repliedValue = m_monitors[1]->powerControl(mode);
    if (powerMode != repliedValue)
        throw "CController::powerControl(int): incorrect confirm value from monitor.";
}
//------------------------------------------------------------------------------
void CController::initParameters() {
    m_parameters.insert(std::pair<std::string, CParameter>(
                            "brightness",CParameter(
                                        (const unsigned char)'0',
                                        (const unsigned char)'0',
                                        (const unsigned char)'1',
                                        (const unsigned char)'0',
                                        new CRangeInterval(0, "dark", 100, "bright")))
                        );
}