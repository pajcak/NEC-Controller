#ifndef __CCONTROLLER_H__
#define	__CCONTROLLER_H__

#include "CMonitor.h"

class CController {
public:
    CController();
    virtual ~CController();
//-----------------------API----------------------------
    int getBrightness();
    void setBrightness(const int & val);
    /**
     * <Status>
     * @return 1: ON\n
     * 2: Stand-by (power save)\n
     * 3: Suspend (power save)\n
     * 4: OFF (same as IR power off)
     */
    int powerStatusRead();
    
private:
    CMonitor * m_monitor;
};

#endif	/* __CCONTROLLER_H__ */

