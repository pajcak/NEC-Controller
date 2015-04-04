
#ifndef __CABSTRACTMESSAGE_H__
#define	__CABSTRACTMESSAGE_H__

#include <string>

class CAbstractMessage {
public:
                                              CAbstractMessage() {}
                                              CAbstractMessage(const unsigned char * _buffer);
                                              CAbstractMessage(const CAbstractMessage&) {}
    virtual                                   ~CAbstractMessage() {}
    
    
    virtual CAbstractMessage*                 clone() const = 0;
    virtual unsigned char                     getCheckCode() const = 0;
    // saves length of certain message into two bytes hi and lo and returns the length in integer
    virtual int                               getLengthInt() const = 0;
    virtual std::basic_string<unsigned char>  getLength() const = 0;
/** \brief Concatenation of all parts of the message.
 *  \return All the message in bytes.
 */
    virtual std::basic_string<unsigned char>  getBuffer() const = 0;
//========UTILS=========
    int fourBytesToInt( const unsigned char leftmost,
                        const unsigned char left,
                        const unsigned char right,
                        const unsigned char rightmost) const
    {
        return (int)(
                (((leftmost  >= 'A') ? (leftmost  - 'A' + 10) : (leftmost  - '0')) << 16) 
              + (((left      >= 'A') ? (left      - 'A' + 10) : (left      - '0')) << 8)
              + (((right     >= 'A') ? (right     - 'A' + 10) : (right     - '0')) << 4)
              +  ((rightmost >= 'A') ? (rightmost - 'A' + 10) : (rightmost - '0'))
              );
    }


protected:
    static const unsigned char s_STX = 0x02;
    static const unsigned char s_ETX = 0x03;    
};

#endif	/* __CABSTRACTMESSAGE_H__ */

