#include "headers/CController.h"
#include "headers/MsgOutgoing.h"
#include "headers/CMonitor.h"
#include "headers/MsgIncoming.h"
#include "headers/Utils.h"
#include <cstdio>
#include <arpa/inet.h>

#define DEBUG
CController::CController() {}

CController::~CController() {}

void CController::initController() {
    initParameters();
    pthread_mutex_init(&m_monitorsMutex, NULL);
}
void CController::destroyController() {
    pthread_mutex_destroy(&m_monitorsMutex);
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
    
    pthread_mutex_lock(&m_monitorsMutex);
    m_monitors.insert(std::pair<int, CMonitor*>(monitorID, new CMonitor(monitorAddr, port)));
    pthread_mutex_unlock(&m_monitorsMutex);
}
bool CController::deleteMonitor  (int monitorID) {

    pthread_mutex_lock(&m_monitorsMutex);
    
    std::map<int, CMonitor*>::iterator it = m_monitors.find(monitorID);
    if (it == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        return false;
    }
    
    pthread_mutex_lock(&it->second->m_mutex);
    if (it->second->isConnected()) it->second->disconnect();
    
    CMonitor ** tmp = &it->second;
    m_monitors.erase(it);
    pthread_mutex_unlock(&(*tmp)->m_mutex);
    pthread_mutex_destroy(&(*tmp)->m_mutex);
    delete *tmp;
    
    pthread_mutex_unlock(&m_monitorsMutex);
    return true;
}
bool CController::connectMonitor     (int monitorID) {
    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator it = m_monitors.find(monitorID);
    if (it == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        return false;
    }
    if (!it->second->isConnected()) {
        bool res;
        try {
            res = it->second->establishConnection();
        } catch (...) {
            pthread_mutex_unlock(&m_monitorsMutex);
            throw;
        }
        pthread_mutex_unlock(&m_monitorsMutex);
        return res;
    }
    pthread_mutex_unlock(&m_monitorsMutex);
    return true;
}
bool CController::disconnectMonitor  (int monitorID) {
    pthread_mutex_lock(&m_monitorsMutex);
    
    std::map<int, CMonitor*>::iterator it = m_monitors.find(monitorID);
    if (it == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        return false;
    }
    
    if (it->second->isConnected())
        it->second->disconnect();
    pthread_mutex_unlock(&m_monitorsMutex);
    return true;
}    
bool CController::connectAll() {
    pthread_mutex_lock(&m_monitorsMutex);
    bool connOK = false;
    for (std::map<int, CMonitor*>::iterator it = m_monitors.begin();
            it != m_monitors.end(); ++it)
    {
        if (!it->second->isConnected())
            try {
                if (!it->second->establishConnection()) connOK = false;
            } catch (...) {
                pthread_mutex_unlock(&m_monitorsMutex);
                throw;
            }
    }
    pthread_mutex_unlock(&m_monitorsMutex);
    return connOK;
}
void CController::disconnectAll() {
    pthread_mutex_lock(&m_monitorsMutex);
    for (std::map<int, CMonitor*>::iterator it = m_monitors.begin();
            it != m_monitors.end(); ++it)
    {
        if (it->second->isConnected()) it->second->disconnect();
    }
    pthread_mutex_unlock(&m_monitorsMutex);
    
}
bool CController::isConnected(int monitorID) {
    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator it = m_monitors.find(monitorID);
    if (it == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::isConnected: invalid monitor ID.";
    }
    bool res = it->second->isConnected();
    pthread_mutex_unlock(&m_monitorsMutex);
    return res;
}

//-----------------------API----------------------------
int  CController::getBrightness(int monitorID) {
    if (!isConnected(monitorID)) throw "CController::getBrightness(): monitor not connected.";
    
    std::map<std::string, CParameter>::const_iterator paramIt = m_parameters.find("brightness");
    if (paramIt == m_parameters.end()) 
        throw "CController::getBrightness(): Internal error, no such parameter.";
    
    CMsgGetCurrParam msg(paramIt->second.m_opCodePage, paramIt->second.m_opCode);
    
    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::getBrightness(): no such monitor.";
    }
    
    pthread_mutex_lock(&(monitIt->second->m_mutex));
    CMonitor ** monitItPtr = &(monitIt->second);
   
    pthread_mutex_unlock(&m_monitorsMutex);    

    CAbstractMessage * gprMsg;
    try {
        gprMsg = (*monitItPtr)->getParameter(&msg);
    } catch (...) {
        pthread_mutex_unlock(&((*monitItPtr)->m_mutex));
        throw;
    } 
    pthread_mutex_unlock(&((*monitItPtr)->m_mutex));
    
    
    CMsgGetCurrParamReply * getParamReply = dynamic_cast<CMsgGetCurrParamReply*>(gprMsg);
    if (getParamReply == 0)
        throw "CController::getBrightness(): Internal error, dynamic_cast.";
    
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
    
    std::map<std::string, CParameter>::const_iterator paramIt = m_parameters.find("brightness");

    if (paramIt == m_parameters.end()) throw "CController::setBrightness(): no such parameter.";

    if (!paramIt->second.m_range->contains(val))
        throw "CController::setBrightness(): requested value is out of range.";
    
    unsigned char value[4];
    IntToFourBytes(val, value);
    
    CMsgSetParam msg (paramIt->second.m_opCodePage, paramIt->second.m_opCode, value);

    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::setBrightness(): no such monitor.";
    }
    
    pthread_mutex_lock(&(monitIt->second->m_mutex));
    CMonitor ** monitItPtr = &(monitIt->second);
    
    pthread_mutex_unlock(&m_monitorsMutex);    
    
    CAbstractMessage * sprMsg;
    try {
        sprMsg = monitIt->second->setParameter(&msg);
    } catch (...) {
        pthread_mutex_unlock(&((*monitItPtr)->m_mutex));
        throw;
    }
    CMsgSetParamReply * setParamReply = dynamic_cast<CMsgSetParamReply*>(sprMsg);
    if (setParamReply == 0) {
        pthread_mutex_unlock(&((*monitItPtr)->m_mutex));
        throw "CController::setBrightness(): Internal error, bad dynamic_cast.";
    }
    
#ifdef DEBUG/*-------------------------------------------------------------*/
    printf("MaxVal: %d\n", setParamReply->getMaxValue());
    printf("CurrVal: %d\n", setParamReply->getCurrValue());
#endif /*-----------------------------------------------------------------*/
    
    if (setParamReply->getCurrValue() != val) {
        pthread_mutex_unlock(&((*monitItPtr)->m_mutex));
        throw "CController::setBrightness(): incorrect confirm value in setParameterReply.";
    }
    try {
        monitIt->second->saveCurrentSettings();
    } catch (...) {
        pthread_mutex_unlock(&((*monitItPtr)->m_mutex));
        throw;
    }
    pthread_mutex_unlock(&((*monitItPtr)->m_mutex));
        
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
