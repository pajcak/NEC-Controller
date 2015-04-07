#ifndef __CCONTROLLER_H__
#define	__CCONTROLLER_H__

#include "CMonitor.h"
#include "CParameter.h"
#include <set>

class CController {
public:
    CController();
    CController(const char * monitorAddr);
    //REPLACE THIS WITH METHOD LIKE CLEANUP
    virtual ~CController();
    void addMonitor (const char * monitorAddr, int port, int monitorID);
    void connectAll();
    //method for connecting/disconnecting enum of monitors
    void disconnectAll();
    bool isConnected(int monitorID);
//-----------------------API----------------------------
    int getBrightness();
    void setBrightness(int val);
    /**
     * @return 1: ON\n
     * 2: Stand-by (power save)\n
     * 3: Suspend (power save)\n
     * 4: OFF (same as IR power off)
     */
    int powerStatusRead();
    /**
     * 
     * @param powerMode - 1(ON) or 4(OFF)
     */
    void powerControl(int powerMode);
    
private:
    std::map<int, CMonitor*> m_monitors;
    
    void initParameters();
    std::map<std::string, CParameter> m_parameters;
};

#endif	/* __CCONTROLLER_H__ */

