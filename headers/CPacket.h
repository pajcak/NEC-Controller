#ifndef __CPACKET_H__
#define	__CPACKET_H__
#include "CHeader.h"
#include "CAbstractMessage.h"
#include <string>

/*
*               PACKET STRUCTURE
*    ___________________________________________
*   | HEADER | MESSAGE | CHECK CODE | DELIMITER |
*   |________|_________|____________|___________|
*   (CHeader)|(CMessage)| calculate |  CR(0x0D) |
*                           1B            1B
* 
*Check code is the Block Check Code(=XOR) between the Header and the End of Message except SOH.
**/

/*MESSAGE TYPE(msgType)*/
//ASCII 'A' (0x41): Command.
//ASCII 'B' (0x42): Command reply. (e.g. Save current settings message)
//ASCII 'C' (0x43): Get current parameter from a monitor.
//ASCII 'D' (0x44): "Get parameter" reply.
//ASCII 'E' (0x45): Set parameter.
//ASCII 'F' (0x46): "Set parameter" reply.

class CPacket {
public:
    CPacket(const CHeader & header, const CAbstractMessage & message);
    ~CPacket();
    std::basic_string<unsigned char> getBuffer();
private:
    const CHeader & m_header;
    const CAbstractMessage & m_message;
    unsigned char m_checkCode;
    static const unsigned char m_delimiter = 0x0D;
};


#endif	/* __CPACKET_H__ */

