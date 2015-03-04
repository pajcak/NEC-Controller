#ifndef __CCONTROLLER_H__
#define	__CCONTROLLER_H__

#include "CConnection.h"

class CController {
public:
    CController();
    virtual ~CController();
//-----------------------API----------------------------
    int getBrightness();
    bool setBrightness(const int & val);
    
private:
    CConnection * m_connection;
};

#endif	/* __CCONTROLLER_H__ */

