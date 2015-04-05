#include "CAbstractMessage.h"
#ifndef __IRECEIVABLE_H__
#define	__IRECEIVABLE_H__

class IReceivable {    
public:
    virtual void initWithRawData(const unsigned char * _buffer) = 0;
};

#endif	/* __IRECEIVABLE_H__ */

