#ifndef __CCONTROLLER_H__
#define	__CCONTROLLER_H__

#include "CMonitor.h"

class CController {
public:
    CController();
    virtual ~CController();
//-----------------------API----------------------------
    int getBrightness();
    bool setBrightness(const int & val);
    
private:
    CMonitor * m_monitor;
};

#endif	/* __CCONTROLLER_H__ */

