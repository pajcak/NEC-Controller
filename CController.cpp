#include "headers/CController.h"
#include "headers/MsgOutgoing.h"
#include "headers/CMonitor.h"
#include "headers/MsgIncoming.h"
#include "headers/Utils.h"
#include <cstdio>
#include <arpa/inet.h>
#define DEBUG
CController::CController() {
    //REPLACE THIS WITH METHOD LIKE INIT
    initParameters();
}

CController::~CController() {
    //REPLACE THIS WITH METHOD LIKE CLEANUP
    for (std::map<int, CMonitor*>::iterator it = m_monitors.begin();
            it != m_monitors.end(); ++it)
    {
        it->second->disconnect();
        delete it->second; // CMonitor* is dynamicly allocated in m_monitors std::map
    }
}
void CController::addMonitor (const char * monitorAddr, int port,  int monitorID) {
    struct sockaddr_in sa;
    if (inet_pton(AF_INET, monitorAddr, &(sa.sin_addr)) != 1)
        throw "CController::addMonitor: invalid monitorAddr.";
    if (port <= 1024 || port > 65535) throw "CController::addMonitor: invalid port.";
    m_monitors.insert(std::pair<int, CMonitor*>(monitorID, new CMonitor(monitorAddr, port)));
}
bool CController::deleteMonitor  (int monitorID) {
    std::map<int, CMonitor*>::iterator it = m_monitors.find(monitorID);
    if (it == m_monitors.end()) return false;
    if (it->second->isConnected()) it->second->disconnect();
    
    delete it->second;
    m_monitors.erase(it);
    return true;
}
bool CController::connectMonitor     (int monitorID) {
    std::map<int, CMonitor*>::iterator it = m_monitors.find(monitorID);
    if (it == m_monitors.end()) return false;
    if (!it->second->isConnected()) return it->second->establishConnection();
    return true;
}
bool CController::disconnectMonitor  (int monitorID) {
    std::map<int, CMonitor*>::iterator it = m_monitors.find(monitorID);
    if (it == m_monitors.end()) return false;
    if (it->second->isConnected()) it->second->disconnect();
    return true;
}    
bool CController::connectAll() {
    bool connOK = false;
    for (std::map<int, CMonitor*>::iterator it = m_monitors.begin();
            it != m_monitors.end(); ++it)
    {
        if (!isConnected(it->first))
            if (!it->second->establishConnection()) connOK = false;
    }
    return connOK;
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
int  CController::getBrightness(int monitorID) {
    if (!isConnected(monitorID)) throw "CController::getBrightness(): monitor not connected.";
    
    std::map<std::string, CParameter>::iterator it = m_parameters.find("brightness");

    CMsgGetCurrParam msg(it->second.m_opCodePage, it->second.m_opCode);
    
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) throw "CController::getBrightness(): no such monitor.";

    CAbstractMessage * gprMsg = monitIt->second->getParameter(&msg);
    
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
void CController::setBrightness(int monitorID, int val) {
    if (!isConnected(monitorID)) throw "CController::setBrightness(): monitor not connected.";
            
    std::map<std::string, CParameter>::iterator paramIt = m_parameters.find("brightness");

    if (paramIt == m_parameters.end()) throw "CController::setBrightness(): no such parameter.";

    if (!paramIt->second.m_range->contains(val))
        throw "CController::setBrightness(): requested value out of range.";
    
    unsigned char value[4];
    IntToFourBytes(val, value);
    
    CMsgSetParam msg (paramIt->second.m_opCodePage, paramIt->second.m_opCode, value);

    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) throw "CController::setBrightness(): no such monitor.";
    
    CAbstractMessage * sprMsg = monitIt->second->setParameter(&msg);
    
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
    
    monitIt->second->saveCurrentSettings();
    
    if (sprMsg) delete sprMsg;
}
int CController::powerStatusRead(int monitorID) {
    if (!isConnected(monitorID)) throw "CController::powerStatusRead(): monitor not connected.";
    
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) throw "CController::powerStatusRead(): no such monitor.";

    return monitIt->second->powerStatusRead();
}
void CController::powerControl(int monitorID, int powerMode) {
    if (!isConnected(monitorID)) throw "CController::powerControl(): monitor not connected.";
    
    unsigned char mode;
    if (powerMode == 1) mode = '1';
    else if (powerMode == 4) mode = '4';
    else throw "CController::powerControl(int): invalid power mode number (1 or 4 allowed).";
    
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) throw "CController::powerControl(): no such monitor.";

    int repliedValue = monitIt->second->powerControl(mode);
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
