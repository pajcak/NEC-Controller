#include "headers/CController.h"
#include "headers/MsgOutgoing.h"
#include "headers/CMonitor.h"
#include "headers/MsgIncoming.h"
#include "headers/Utils.h"
#include <cstdio>
#include <arpa/inet.h>

//#define DEBUG


void initController() {
    initParameters();
    pthread_mutex_init(&m_monitorsMutex, NULL);
}
void destroyController() {
    pthread_mutex_destroy(&m_monitorsMutex);
    for (std::map<int, CMonitor*>::iterator it = m_monitors.begin();
            it != m_monitors.end(); ++it)
    {
        it->second->disconnect();
        delete it->second; // CMonitor* is dynamicly allocated in m_monitors std::map
    }
}

void addMonitor (const char * monitorAddr, int port,  int monitorID) {
    struct sockaddr_in sa;
    if (inet_pton(AF_INET, monitorAddr, &(sa.sin_addr)) != 1)
        throw "CController::addMonitor: invalid monitorAddr.";
    if (port <= 1024 || port > 65535) throw "CController::addMonitor: invalid port.";
    
    pthread_mutex_lock(&m_monitorsMutex);
    m_monitors.insert(std::pair<int, CMonitor*>(monitorID, new CMonitor(monitorAddr, port)));
    pthread_mutex_unlock(&m_monitorsMutex);
}
void deleteMonitor  (int monitorID) {

    pthread_mutex_lock(&m_monitorsMutex);
    
    std::map<int, CMonitor*>::iterator it = m_monitors.find(monitorID);
    if (it == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::deleteMonitor(int): invalid monitor ID.";
    }
    
    pthread_mutex_lock(&it->second->m_mutex);
    if (it->second->isConnected()) it->second->disconnect();
    
    CMonitor ** tmp = &it->second;
    m_monitors.erase(it);
    pthread_mutex_unlock(&(*tmp)->m_mutex);
    pthread_mutex_destroy(&(*tmp)->m_mutex);
    delete *tmp;
    
    pthread_mutex_unlock(&m_monitorsMutex);
}
bool connectMonitor     (int monitorID) {
    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator it = m_monitors.find(monitorID);
    if (it == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::connectMonitor: invalid monitor ID.";
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
void disconnectMonitor  (int monitorID) {
    pthread_mutex_lock(&m_monitorsMutex);
    
    std::map<int, CMonitor*>::iterator it = m_monitors.find(monitorID);
    if (it == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::disconnectMonitor(int): invalid monitor ID.";
    }
    
    if (it->second->isConnected())
        it->second->disconnect();
    pthread_mutex_unlock(&m_monitorsMutex);
}    
bool connectAll() {
    pthread_mutex_lock(&m_monitorsMutex);
    bool connOK = true;
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
void disconnectAll() {
    pthread_mutex_lock(&m_monitorsMutex);
    for (std::map<int, CMonitor*>::iterator it = m_monitors.begin();
            it != m_monitors.end(); ++it)
    {
        if (it->second->isConnected()) it->second->disconnect();
    }
    pthread_mutex_unlock(&m_monitorsMutex);
    
}
bool isConnected(int monitorID) {
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
//-----------------------API----------------------------
//-----------------------API----------------------------
//-----------------------API----------------------------
int  getBacklight(int monitorID) {
    if (!isConnected(monitorID)) throw "CController::getBacklight(int): monitor not connected.";
    
    std::map<std::string, CParameter>::const_iterator paramIt = m_parameters.find("backlight");
    if (paramIt == m_parameters.end()) 
        throw "CController::getBacklight(int): Internal error, no such parameter.";
    
    CMsgGetCurrParam msg(paramIt->second.m_opCodePage, paramIt->second.m_opCode);
    
    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::getBacklight(int): no such monitor.";
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
        throw "CController::getBacklight(int): Internal error, dynamic_cast.";
    
    int backlight = getParamReply->getCurrValue();

#ifdef DEBUG/*-------------------------------------------------------------*/
    printf ("MaxVal=  [%d]\n", getParamReply->getMaxValue());
    printf ("CurrVal= [%d]\n", backlight);
#endif /*-----------------------------------------------------------------*/
    
    if (gprMsg) delete gprMsg;
    return backlight;
}
void setBacklight(int monitorID, int val) {
    if (!isConnected(monitorID)) throw "CController::setBacklight(int,int): monitor not connected.";
    
    std::map<std::string, CParameter>::const_iterator paramIt = m_parameters.find("backlight");

    if (paramIt == m_parameters.end()) throw "CController::setBacklight(int,int): no such parameter.";

    if (!paramIt->second.m_range->contains(val))
        throw "CController::setBacklight(int,int): requested value is out of range.";
    
    unsigned char value[4];
    IntToFourBytes(val, value);
    
    CMsgSetParam msg (paramIt->second.m_opCodePage, paramIt->second.m_opCode, value);

    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::setBacklight(int,int): no such monitor.";
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
        throw "CController::setBacklight(int,int): Internal error, bad dynamic_cast.";
    }
    
#ifdef DEBUG/*-------------------------------------------------------------*/
    printf("MaxVal: %d\n", setParamReply->getMaxValue());
    printf("CurrVal: %d\n", setParamReply->getCurrValue());
#endif /*-----------------------------------------------------------------*/
    
    if (setParamReply->getCurrValue() != val) {
        pthread_mutex_unlock(&((*monitItPtr)->m_mutex));
        throw "CController::setBacklight(int,int): incorrect confirm value in setParameterReply.";
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
//------------------------------------------------------------------------------
int  getContrast            (int monitorID) {
    if (!isConnected(monitorID)) throw "CController::getContrast(int): monitor not connected.";
    
    std::map<std::string, CParameter>::const_iterator paramIt = m_parameters.find("contrast");
    if (paramIt == m_parameters.end()) 
        throw "CController::getContrast(int): Internal error, no such parameter.";
    
    CMsgGetCurrParam msg(paramIt->second.m_opCodePage, paramIt->second.m_opCode);
    
    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::getContrast(int): no such monitor.";
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
        throw "CController::getContrast(int): Internal error, dynamic_cast.";
    
    int contrast = getParamReply->getCurrValue();

#ifdef DEBUG/*-------------------------------------------------------------*/
    printf ("MaxVal=  [%d]\n", getParamReply->getMaxValue());
    printf ("CurrVal= [%d]\n", contrast);
#endif /*-----------------------------------------------------------------*/
    
    if (gprMsg) delete gprMsg;
    return contrast;
}
void setContrast            (int monitorID, int val) {
    if (!isConnected(monitorID)) throw "CController::setContrast(int,int): monitor not connected.";

    std::map<std::string, CParameter>::const_iterator paramIt = m_parameters.find("contrast");

    if (paramIt == m_parameters.end()) throw "CController::setContrast(int,int): no such parameter.";

    if (!paramIt->second.m_range->contains(val))
        throw "CController::setContrast(int,int): requested value is out of range.";
    
    unsigned char value[4];
    IntToFourBytes(val, value);
    
    CMsgSetParam msg (paramIt->second.m_opCodePage, paramIt->second.m_opCode, value);

    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::setContrast(int,int): no such monitor.";
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
        throw "CController::setContrast(int,int): Internal error, bad dynamic_cast.";
    }
    
#ifdef DEBUG/*-------------------------------------------------------------*/
    printf("MaxVal: %d\n", setParamReply->getMaxValue());
    printf("CurrVal: %d\n", setParamReply->getCurrValue());
#endif /*-----------------------------------------------------------------*/
    
    if (setParamReply->getCurrValue() != val) {
        pthread_mutex_unlock(&((*monitItPtr)->m_mutex));
        throw "CController::setContrast(int,int): incorrect confirm value in setParameterReply.";
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
//------------------------------------------------------------------------------
int  getSharpness           (int monitorID) {
    if (!isConnected(monitorID)) throw "CController::getSharpness(int): monitor not connected.";
    
    std::map<std::string, CParameter>::const_iterator paramIt = m_parameters.find("sharpness");
    if (paramIt == m_parameters.end()) 
        throw "CController::getSharpness(int): Internal error, no such parameter.";
    
    CMsgGetCurrParam msg(paramIt->second.m_opCodePage, paramIt->second.m_opCode);
    
    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::getSharpness(int): no such monitor.";
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
        throw "CController::getSharpness(int): Internal error, dynamic_cast.";
    
    int sharpness = getParamReply->getCurrValue();

#ifdef DEBUG/*-------------------------------------------------------------*/
    printf ("MaxVal=  [%d]\n", getParamReply->getMaxValue());
    printf ("CurrVal= [%d]\n", sharpness);
#endif /*-----------------------------------------------------------------*/
    
    if (gprMsg) delete gprMsg;
    return sharpness;
}
void setSharpness           (int monitorID, int val) {
    if (!isConnected(monitorID)) throw "CController::setSharpness(int,int): monitor not connected.";
    
    std::map<std::string, CParameter>::const_iterator paramIt = m_parameters.find("sharpness");

    if (paramIt == m_parameters.end()) throw "CController::setSharpness(int,int): no such parameter.";

    if (!paramIt->second.m_range->contains(val))
        throw "CController::setSharpness(int,int): requested value is out of range.";
    
    unsigned char value[4];
    IntToFourBytes(val, value);
    
    CMsgSetParam msg (paramIt->second.m_opCodePage, paramIt->second.m_opCode, value);

    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::setSharpness(int,int): no such monitor.";
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
        throw "CController::setSharpness(int,int): Internal error, bad dynamic_cast.";
    }
    
#ifdef DEBUG/*-------------------------------------------------------------*/
    printf("MaxVal: %d\n", setParamReply->getMaxValue());
    printf("CurrVal: %d\n", setParamReply->getCurrValue());
#endif /*-----------------------------------------------------------------*/
    
    if (setParamReply->getCurrValue() != val) {
        pthread_mutex_unlock(&((*monitItPtr)->m_mutex));
        throw "CController::setSharpness(int,int): incorrect confirm value in setParameterReply.";
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
//------------------------------------------------------------------------------
int  getBrightness          (int monitorID) {
    if (!isConnected(monitorID)) throw "CController::getBrightness(int): monitor not connected.";
    
    std::map<std::string, CParameter>::const_iterator paramIt = m_parameters.find("brightness");
    if (paramIt == m_parameters.end()) 
        throw "CController::getBrightness(int): Internal error, no such parameter.";
    
    CMsgGetCurrParam msg(paramIt->second.m_opCodePage, paramIt->second.m_opCode);
    
    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::getBrightness(int): no such monitor.";
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
        throw "CController::getBrightness(int): Internal error, dynamic_cast.";
    
    int brightness = getParamReply->getCurrValue();

#ifdef DEBUG/*-------------------------------------------------------------*/
    printf ("MaxVal=  [%d]\n", getParamReply->getMaxValue());
    printf ("CurrVal= [%d]\n", brightness);
#endif /*-----------------------------------------------------------------*/
    
    if (gprMsg) delete gprMsg;
    return brightness;
}
void setBrightness          (int monitorID, int val) {
    if (!isConnected(monitorID)) throw "CController::setBrightness(int,int): monitor not connected.";
    
    std::map<std::string, CParameter>::const_iterator paramIt = m_parameters.find("brightness");

    if (paramIt == m_parameters.end()) throw "CController::setBrightness(int,int): no such parameter.";

    if (!paramIt->second.m_range->contains(val))
        throw "CController::setBrightness(int,int): requested value is out of range.";
    
    unsigned char value[4];
    IntToFourBytes(val, value);
    
    CMsgSetParam msg (paramIt->second.m_opCodePage, paramIt->second.m_opCode, value);

    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::setBrightness(int,int): no such monitor.";
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
        throw "CController::setBrightness(int,int): Internal error, bad dynamic_cast.";
    }
    
#ifdef DEBUG/*-------------------------------------------------------------*/
    printf("MaxVal: %d\n", setParamReply->getMaxValue());
    printf("CurrVal: %d\n", setParamReply->getCurrValue());
#endif /*-----------------------------------------------------------------*/
    
    if (setParamReply->getCurrValue() != val) {
        pthread_mutex_unlock(&((*monitItPtr)->m_mutex));
        throw "CController::setBrightness(int,int): incorrect confirm value in setParameterReply.";
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
//------------------------------------------------------------------------------
int  getHue                 (int monitorID) {
    if (!isConnected(monitorID)) throw "CController::getHue(int): monitor not connected.";
    
    std::map<std::string, CParameter>::const_iterator paramIt = m_parameters.find("hue");
    if (paramIt == m_parameters.end()) 
        throw "CController::getHue(int): Internal error, no such parameter.";
    
    CMsgGetCurrParam msg(paramIt->second.m_opCodePage, paramIt->second.m_opCode);
    
    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::getHue(int): no such monitor.";
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
        throw "CController::getHue(int): Internal error, dynamic_cast.";
    
    int hue = getParamReply->getCurrValue();

#ifdef DEBUG/*-------------------------------------------------------------*/
    printf ("MaxVal=  [%d]\n", getParamReply->getMaxValue());
    printf ("CurrVal= [%d]\n", hue);
#endif /*-----------------------------------------------------------------*/
    
    if (gprMsg) delete gprMsg;
    return hue;
}
void setHue                 (int monitorID, int val) {
    if (!isConnected(monitorID)) throw "CController::setHue(int,int): monitor not connected.";
    
    std::map<std::string, CParameter>::const_iterator paramIt = m_parameters.find("hue");

    if (paramIt == m_parameters.end()) throw "CController::setHue(int,int): no such parameter.";

    if (!paramIt->second.m_range->contains(val))
        throw "CController::setHue(int,int): requested value is out of range.";
    
    unsigned char value[4];
    IntToFourBytes(val, value);
    
    CMsgSetParam msg (paramIt->second.m_opCodePage, paramIt->second.m_opCode, value);

    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::setHue(int,int): no such monitor.";
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
        throw "CController::setHue(int,int): Internal error, bad dynamic_cast.";
    }
    
#ifdef DEBUG/*-------------------------------------------------------------*/
    printf("MaxVal: %d\n", setParamReply->getMaxValue());
    printf("CurrVal: %d\n", setParamReply->getCurrValue());
#endif /*-----------------------------------------------------------------*/
    
    if (setParamReply->getCurrValue() != val) {
        pthread_mutex_unlock(&((*monitItPtr)->m_mutex));
        throw "CController::setHue(int,int): incorrect confirm value in setParameterReply.";
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
//------------------------------------------------------------------------------
int  getPaleness            (int monitorID) {
    if (!isConnected(monitorID)) throw "CController::getPaleness(int): monitor not connected.";
    
    std::map<std::string, CParameter>::const_iterator paramIt = m_parameters.find("paleness");
    if (paramIt == m_parameters.end()) 
        throw "CController::getPaleness(int): Internal error, no such parameter.";
    
    CMsgGetCurrParam msg(paramIt->second.m_opCodePage, paramIt->second.m_opCode);
    
    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::getPaleness(int): no such monitor.";
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
        throw "CController::getPaleness(int): Internal error, dynamic_cast.";
    
    int paleness = getParamReply->getCurrValue();

#ifdef DEBUG/*-------------------------------------------------------------*/
    printf ("MaxVal=  [%d]\n", getParamReply->getMaxValue());
    printf ("CurrVal= [%d]\n", paleness);
#endif /*-----------------------------------------------------------------*/
    
    if (gprMsg) delete gprMsg;
    return paleness;
}
void setPaleness            (int monitorID, int val) {
    if (!isConnected(monitorID)) throw "CController::setPaleness(int,int): monitor not connected.";
    
    std::map<std::string, CParameter>::const_iterator paramIt = m_parameters.find("paleness");

    if (paramIt == m_parameters.end()) throw "CController::setPaleness(int,int): no such parameter.";

    if (!paramIt->second.m_range->contains(val))
        throw "CController::setPaleness(int,int): requested value is out of range.";
    
    unsigned char value[4];
    IntToFourBytes(val, value);
    
    CMsgSetParam msg (paramIt->second.m_opCodePage, paramIt->second.m_opCode, value);

    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::setPaleness(int,int): no such monitor.";
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
        throw "CController::setPaleness(int,int): Internal error, bad dynamic_cast.";
    }
    
#ifdef DEBUG/*-------------------------------------------------------------*/
    printf("MaxVal: %d\n", setParamReply->getMaxValue());
    printf("CurrVal: %d\n", setParamReply->getCurrValue());
#endif /*-----------------------------------------------------------------*/
    
    if (setParamReply->getCurrValue() != val) {
        pthread_mutex_unlock(&((*monitItPtr)->m_mutex));
        throw "CController::setPaleness(int,int): incorrect confirm value in setParameterReply.";
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
//------------------------------------------------------------------------------
int  getColorTemperature    (int monitorID) {
    if (!isConnected(monitorID)) throw "CController::getColorTemperature(int): monitor not connected.";
    
    std::map<std::string, CParameter>::const_iterator paramIt = m_parameters.find("colorTemperature");
    if (paramIt == m_parameters.end()) 
        throw "CController::getColorTemperature(int): Internal error, no such parameter.";
    
    CMsgGetCurrParam msg(paramIt->second.m_opCodePage, paramIt->second.m_opCode);
    
    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::getColorTemperature(int): no such monitor.";
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
        throw "CController::getColorTemperature(int): Internal error, dynamic_cast.";
    
    int colorTemperature = getParamReply->getCurrValue();

#ifdef DEBUG/*-------------------------------------------------------------*/
    printf ("MaxVal=  [%d]\n", getParamReply->getMaxValue());
    printf ("CurrVal= [%d]\n", colorTemperature);
#endif /*-----------------------------------------------------------------*/
    
    if (gprMsg) delete gprMsg;
    return colorTemperature;
}
void setColorTemperature    (int monitorID, int val) {
    if (!isConnected(monitorID)) throw "CController::setColorTemperature(int,int): monitor not connected.";
    
    std::map<std::string, CParameter>::const_iterator paramIt = m_parameters.find("colorTemperature");

    if (paramIt == m_parameters.end()) throw "CController::setColorTemperature(int,int): no such parameter.";

    if (!paramIt->second.m_range->contains(val))
        throw "CController::setColorTemperature(int,int): requested value is out of range.";
    
    unsigned char value[4];
    IntToFourBytes(val, value);
    
    CMsgSetParam msg (paramIt->second.m_opCodePage, paramIt->second.m_opCode, value);

    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::setColorTemperature(int,int): no such monitor.";
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
        throw "CController::setColorTemperature(int,int): Internal error, bad dynamic_cast.";
    }
    
#ifdef DEBUG/*-------------------------------------------------------------*/
    printf("MaxVal: %d\n", setParamReply->getMaxValue());
    printf("CurrVal: %d\n", setParamReply->getCurrValue());
#endif /*-----------------------------------------------------------------*/
    
    if (setParamReply->getCurrValue() != val) {
        pthread_mutex_unlock(&((*monitItPtr)->m_mutex));
        throw "CController::setColorTemperature(int,int): incorrect confirm value in setParameterReply.";
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
//------------------------------------------------------------------------------
int  getGammaCorrection     (int monitorID) {
    if (!isConnected(monitorID)) throw "CController::getGammaCorrection(int): monitor not connected.";
    
    std::map<std::string, CParameter>::const_iterator paramIt = m_parameters.find("gammaCorrection");
    if (paramIt == m_parameters.end()) 
        throw "CController::getGammaCorrection(int): Internal error, no such parameter.";
    
    CMsgGetCurrParam msg(paramIt->second.m_opCodePage, paramIt->second.m_opCode);
    
    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::getGammaCorrection(int): no such monitor.";
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
        throw "CController::getGammaCorrection(int): Internal error, dynamic_cast.";
    
    int gammaCorrection = getParamReply->getCurrValue();

#ifdef DEBUG/*-------------------------------------------------------------*/
    printf ("MaxVal=  [%d]\n", getParamReply->getMaxValue());
    printf ("CurrVal= [%d]\n", gammaCorrection);
#endif /*-----------------------------------------------------------------*/
    
    if (gprMsg) delete gprMsg;
    return gammaCorrection;
}
void setGammaCorrection     (int monitorID, int val) {
    if (!isConnected(monitorID)) throw "CController::setGammaCorrection(int,int): monitor not connected.";
    
    std::map<std::string, CParameter>::const_iterator paramIt = m_parameters.find("gammaCorrection");

    if (paramIt == m_parameters.end()) throw "CController::setGammaCorrection(int,int): no such parameter.";

    if (!paramIt->second.m_range->contains(val))
        throw "CController::setGammaCorrection(int,int): requested value is out of range.";
    
    unsigned char value[4];
    IntToFourBytes(val, value);
    
    CMsgSetParam msg (paramIt->second.m_opCodePage, paramIt->second.m_opCode, value);

    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::setGammaCorrection(int,int): no such monitor.";
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
        throw "CController::setGammaCorrection(int,int): Internal error, bad dynamic_cast.";
    }
    
#ifdef DEBUG/*-------------------------------------------------------------*/
    printf("MaxVal: %d\n", setParamReply->getMaxValue());
    printf("CurrVal: %d\n", setParamReply->getCurrValue());
#endif /*-----------------------------------------------------------------*/
    
    if (setParamReply->getCurrValue() != val) {
        pthread_mutex_unlock(&((*monitItPtr)->m_mutex));
        throw "CController::setGammaCorrection(int,int): incorrect confirm value in setParameterReply.";
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
//------------------------------------------------------------------------------
int  getVolume              (int monitorID) {
    if (!isConnected(monitorID)) throw "CController::getVolume(int): monitor not connected.";
    
    std::map<std::string, CParameter>::const_iterator paramIt = m_parameters.find("volume");
    if (paramIt == m_parameters.end()) 
        throw "CController::getVolume(int): Internal error, no such parameter.";
    
    CMsgGetCurrParam msg(paramIt->second.m_opCodePage, paramIt->second.m_opCode);
    
    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::getVolume(int): no such monitor.";
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
        throw "CController::getVolume(int): Internal error, dynamic_cast.";
    
    int volume = getParamReply->getCurrValue();

#ifdef DEBUG/*-------------------------------------------------------------*/
    printf ("MaxVal=  [%d]\n", getParamReply->getMaxValue());
    printf ("CurrVal= [%d]\n", volume);
#endif /*-----------------------------------------------------------------*/
    
    if (gprMsg) delete gprMsg;
    return volume;
}
void setVolume              (int monitorID, int val) {
    if (!isConnected(monitorID)) throw "CController::setVolume(int,int): monitor not connected.";
    
    std::map<std::string, CParameter>::const_iterator paramIt = m_parameters.find("volume");

    if (paramIt == m_parameters.end()) throw "CController::setVolume(int,int): no such parameter.";

    if (!paramIt->second.m_range->contains(val))
        throw "CController::setVolume(int,int): requested value is out of range.";
    
    unsigned char value[4];
    IntToFourBytes(val, value);
    
    CMsgSetParam msg (paramIt->second.m_opCodePage, paramIt->second.m_opCode, value);

    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::setVolume(int,int): no such monitor.";
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
        throw "CController::setVolume(int,int): Internal error, bad dynamic_cast.";
    }
    
#ifdef DEBUG/*-------------------------------------------------------------*/
    printf("MaxVal: %d\n", setParamReply->getMaxValue());
    printf("CurrVal: %d\n", setParamReply->getCurrValue());
#endif /*-----------------------------------------------------------------*/
    
    if (setParamReply->getCurrValue() != val) {
        pthread_mutex_unlock(&((*monitItPtr)->m_mutex));
        throw "CController::setVolume(int,int): incorrect confirm value in setParameterReply.";
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

//===================COMMANDS===========================
int powerStatusRead(int monitorID) {
    if (!isConnected(monitorID)) throw "CController::powerStatusRead(): monitor not connected.";
    
    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::powerStatusRead(): no such monitor.";
    }
    
    pthread_mutex_lock(&(monitIt->second->m_mutex));
    CMonitor ** monitItPtr = &(monitIt->second);
    
    pthread_mutex_unlock(&m_monitorsMutex);
    int powerStatus;
    try {
        powerStatus = monitIt->second->powerStatusRead();
    } catch (...) {
        pthread_mutex_unlock(&((*monitItPtr)->m_mutex));
        throw;
    }
    
    pthread_mutex_unlock(&((*monitItPtr)->m_mutex));
    return powerStatus;
}
//------------------------------------------------------------------------------
void powerControl(int monitorID, int powerMode) {
    if (!isConnected(monitorID)) throw "CController::powerControl(): monitor not connected.";
    
    unsigned char mode;
    if (powerMode == 1) mode = '1';
    else if (powerMode == 4) mode = '4';
    else throw "CController::powerControl(int): invalid power mode number (1 or 4 allowed).";
    
    
    pthread_mutex_lock(&m_monitorsMutex);
    std::map<int, CMonitor*>::iterator monitIt = m_monitors.find(monitorID);
    if (monitIt == m_monitors.end()) {
        pthread_mutex_unlock(&m_monitorsMutex);
        throw "CController::powerControl(int): no such monitor.";
    }
    pthread_mutex_lock(&(monitIt->second->m_mutex));
    CMonitor ** monitItPtr = &(monitIt->second);
    
    pthread_mutex_unlock(&m_monitorsMutex);

    int repliedValue;
    try {
        repliedValue = monitIt->second->powerControl(mode);
    } catch (...) {
        pthread_mutex_unlock(&((*monitItPtr)->m_mutex));
        throw;
    }
    pthread_mutex_unlock(&((*monitItPtr)->m_mutex));
    
    if (powerMode != repliedValue)
        throw "CController::powerControl(int): incorrect confirm value from monitor.";
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void initParameters() {
//---------------------------------FP1--------------------------------------------
m_parameters.insert(std::pair<std::string, CParameter>(
                            "backlight",CParameter(
                                        (const unsigned char)'0',
                                        (const unsigned char)'0',
                                        (const unsigned char)'1',
                                        (const unsigned char)'0',
                                        new CRangeInterval(0, "dark", 100, "bright")))
                        );
//---------------------------------FP2--------------------------------------------
m_parameters.insert(std::pair<std::string, CParameter>(
                            "contrast",CParameter(
                                        (const unsigned char)'0',
                                        (const unsigned char)'0',
                                        (const unsigned char)'1',
                                        (const unsigned char)'2',
                                        new CRangeInterval(0, "low", 100, "high")))
                        );
//---------------------------------FP3--------------------------------------------
m_parameters.insert(std::pair<std::string, CParameter>(
                            "sharpness",CParameter(
                                        (const unsigned char)'0',
                                        (const unsigned char)'0',
                                        (const unsigned char)'8',
                                        (const unsigned char)'C',
                                        new CRangeInterval(0, "dull", 24, "sharp")))
                        );
//---------------------------------FP4--------------------------------------------
m_parameters.insert(std::pair<std::string, CParameter>(
                            "brightness",CParameter(
                                        (const unsigned char)'0',
                                        (const unsigned char)'0',
                                        (const unsigned char)'9',
                                        (const unsigned char)'2',
                                        new CRangeInterval(0, "dark", 100, "bright")))
                        );
//---------------------------------FP5--------------------------------------------
m_parameters.insert(std::pair<std::string, CParameter>(
                            "hue",CParameter(
                                        (const unsigned char)'0',
                                        (const unsigned char)'0',
                                        (const unsigned char)'9',
                                        (const unsigned char)'0',
                                        new CRangeInterval(0, "purplish", 100, "greenish")))
                        );
//---------------------------------FP6--------------------------------------------
m_parameters.insert(std::pair<std::string, CParameter>(
                            "paleness",CParameter(
                                        (const unsigned char)'0',
                                        (const unsigned char)'2',
                                        (const unsigned char)'1',
                                        (const unsigned char)'F',
                                        new CRangeInterval(0, "pale", 100, "deep")))
                        );
//---------------------------------FP7--------------------------------------------
m_parameters.insert(std::pair<std::string, CParameter>(
                            "colorTemperature",CParameter(
                                        (const unsigned char)'0',
                                        (const unsigned char)'0',
                                        (const unsigned char)'5',
                                        (const unsigned char)'4',
                                        new CRangeInterval(0, "2600K", 74, "10000K")))
                        );
//---------------------------------FP8--------------------------------------------
CRangeEnum * rng = new CRangeEnum();
rng->add(1, "Native Gamma");
rng->add(4, "Gamma=2.2");
rng->add(5, "Gamma=2.4");
rng->add(6, "S Gamma");
rng->add(7, "DICOM SIM.");
rng->add(8, "Programmable");
m_parameters.insert(std::pair<std::string, CParameter>(
                            "gammaCorrection",CParameter(
                                        (const unsigned char)'0',
                                        (const unsigned char)'2',
                                        (const unsigned char)'6',
                                        (const unsigned char)'8',
                                        rng))
                        );
//---------------------------------FP9--------------------------------------------
m_parameters.insert(std::pair<std::string, CParameter>(
                            "volume",CParameter(
                                        (const unsigned char)'0',
                                        (const unsigned char)'0',
                                        (const unsigned char)'6',
                                        (const unsigned char)'2',
                                        new CRangeInterval(0, "whisper", 100, "loud")))
                        );
}

