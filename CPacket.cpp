#include <string>

#include "headers/CPacket.h"
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

CPacket::CPacket(const CHeader & header, const CAbstractMessage & message)
{
    m_header = new CHeader(header);
    m_message = message.clone();
    
    m_checkCode = ( m_header->getCheckCode() ^ m_message->getCheckCode() );
}
CPacket::~CPacket() {
    delete m_header;
    delete m_message;
}
std::basic_string<unsigned char> CPacket::getBuffer() const {
    std::basic_string<unsigned char> uString;
    uString.append(m_header->getBuffer());
    uString.append(m_message->getBuffer());
    uString.push_back(m_checkCode);
    uString.push_back(m_delimiter);
    return uString;
}
unsigned char CPacket::getCheckCode() const {
    return ( m_header->getCheckCode() ^ m_message->getCheckCode() );
}
const CHeader * CPacket::getHeader() const {
    return m_header;
}
int CPacket::getLength() const {
    unsigned char dummy;
    return m_header->getLen() + m_message->getLength(dummy, dummy) + 2;
}
CAbstractMessage *       CPacket::getMessage() {
    return m_message;
}
const CAbstractMessage * CPacket::getMessage() const {
    return (const CAbstractMessage*)m_message;
}