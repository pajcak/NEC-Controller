#ifndef __CCONTROLLER_H__
#define	__CCONTROLLER_H__

#include "CMonitor.h"
#include "CParameter.h"
#include <set>

class CController {
public:
    CController();
    CController(const char * monitorAddr);
    virtual ~CController();
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
    CMonitor * m_monitor;
    
    void initParameters();
    std::map<std::string, CParameter> m_parameters;
};

#endif	/* __CCONTROLLER_H__ */

