#include "CHeader.h"
#include <cstring>

CHeader::CHeader(unsigned char _destination, unsigned char _source,
                unsigned char _msgType, unsigned char _msgLen [2])
: m_SOH(0x01), m_RESERVED(0x30), m_destination(_destination), m_source(_source), m_msgType(_msgType)
{
    m_msgLen[0] = _msgLen[0];
    m_msgLen[1] = _msgLen[1];
//    unsigned char SOH; /*0x01*/
//    unsigned char RESERVED; /*0x30*/
//    unsigned char destination; /*Controller(me) chooses the monitor / Replier(LCD) sets it with '0'(0x30)*/
//    unsigned char source; /*Controller(me) 0x30 / Replier(LCD) sets it with its ID*/
//    unsigned char msgType;
//    unsigned char msgLen [2]; /*from STX to ETX inclusive !Use CMessage.getLength()!*/
}
CHeader::CHeader(const char * _buffer) 
{
    if (strlen(_buffer) != 7) throw "CHeader::CHeader(): incorrect buffer size\n";
    m_SOH = _buffer[0];
    m_RESERVED = _buffer[1];
    m_destination = _buffer[2];
    m_source = _buffer[3];
    m_msgType = _buffer[4];
    m_msgLen[0] = _buffer[5];
    m_msgLen[1] = _buffer[6];
}
CHeader::CHeader(const CHeader & header)
: m_SOH(0x01), m_RESERVED(0x30), m_destination(header.m_destination),
        m_source(header.m_source), m_msgType(header.m_msgType)
{
    m_msgLen[0] = header.m_msgLen[0];
    m_msgLen[1] = header.m_msgLen[1];
}
unsigned char CHeader::getCheckCode() const
{
    return (m_RESERVED ^ m_destination ^ m_source ^ m_msgType ^ m_msgLen[0] ^ m_msgLen[1]);
}
std::basic_string<unsigned char> CHeader::getBuffer() {
    std::basic_string<unsigned char> ustr;
    ustr.push_back(m_SOH);
    ustr.push_back(m_RESERVED);
    ustr.push_back(m_destination);
    ustr.push_back(m_source);
    ustr.push_back(m_msgType);
    ustr.push_back(m_msgLen[0]);
    ustr.push_back(m_msgLen[1]);
    return ustr;
}


