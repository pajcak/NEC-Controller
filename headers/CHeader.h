#ifndef __CHEADER_H__
#define	__CHEADER_H__

#include <string>

class CHeader {
public:
    CHeader(unsigned char _destination,
            unsigned char _msgType,
            const unsigned char _msgLen [2]);
    CHeader(const char * _buffer);
    CHeader(const CHeader & header);
    unsigned char getCheckCode() const;
    std::basic_string<unsigned char> getBuffer() const;
private:
    unsigned char m_SOH; /*0x01*/
    unsigned char m_RESERVED; /*0x30*/
    unsigned char m_destination; /*Controller(me) chooses the monitor / Replier(LCD) sets it with '0'(0x30)*/
    unsigned char m_source; /*Controller(me) 0x30 / Replier(LCD) sets it with its ID*/
    unsigned char m_msgType;
    unsigned char m_msgLen [2]; /*from STX to ETX inclusive !Use CMessage.getLength()!*/
};
/*MESSAGE TYPE(msgType)*/
//ASCII 'A' (0x41): Command.
//ASCII 'B' (0x42): Command reply. (e.g. Save current settings message)
//ASCII 'C' (0x43): Get current parameter from a monitor.
//ASCII 'D' (0x44): "Get parameter" reply.
//ASCII 'E' (0x45): Set parameter.
//ASCII 'F' (0x46): "Set parameter" reply.


#endif	/* __CHEADER_H__ */

