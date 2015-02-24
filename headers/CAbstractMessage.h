
#ifndef __CABSTRACTMESSAGE_H__
#define	__CABSTRACTMESSAGE_H__

#include <string>

class CAbstractMessage {
public:
                                              CAbstractMessage() {}
                                              CAbstractMessage(const CAbstractMessage&) {}
    virtual                                   ~CAbstractMessage() {}
    virtual unsigned char                     getCheckCode() const = 0;
    // saves length of certain message into two bytes hi and lo
    virtual int                               getLength(unsigned char & hi, unsigned char & lo) const = 0;
    virtual std::basic_string<unsigned char>  getLength() const = 0;
    // saves all bytes to _outBuffer and returns the number of bytes saved
    virtual std::basic_string<unsigned char>  getBuffer() const = 0;
protected:
    static const unsigned char s_STX = 0x02;
    static const unsigned char s_ETX = 0x03;
    
};



#endif	/* __CABSTRACTMESSAGE_H__ */

