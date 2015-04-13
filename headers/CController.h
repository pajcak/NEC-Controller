#ifndef __CCONTROLLER_H__
#define	__CCONTROLLER_H__

#include "CMonitor.h"
#include "CParameter.h"
#include <set>

class CController {
public:
    CController             ();
    CController             (const char * monitorAddr);
    virtual ~CController    ();
    
    void initController     ();
    void destroyController  ();
//------------------------------------------------------
    void addMonitor         (const char * monitorAddr, int port, int monitorID);
    bool deleteMonitor      (int monitorID);
    
    bool connectMonitor     (int monitorID);
    bool disconnectMonitor  (int monitorID);
    bool connectAll         ();
    void disconnectAll      ();
    bool isConnected        (int monitorID);
//-----------------------API----------------------------
    int getBrightness       (int monitorID);
    void setBrightness      (int monitorID, int val);
    /**
     * @return 1: ON\n
     * 2: Stand-by (power save)\n
     * 3: Suspend (power save)\n
     * 4: OFF (same as IR power off)
     */
    int powerStatusRead(int monitorID);
    /**
     * 
     * @param powerMode - 1(ON) or 4(OFF)
     */
    void powerControl(int monitorID, int powerMode);
    
private:
    pthread_mutex_t m_monitorsMutex;
    std::map<int, CMonitor*> m_monitors;
    
    void initParameters();
    std::map<std::string, CParameter> m_parameters;
};

#endif	/* __CCONTROLLER_H__ */

