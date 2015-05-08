//    File: CHeader.h
//    Copyright (C) 2015  Patrik Faistaver
//    
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

#ifndef __CHEADER_H__
#define	__CHEADER_H__

#include <string>

#include "IReceivable.h"

class CHeader : public IReceivable{
public:
    CHeader() {}
    CHeader(unsigned char _destination,
            unsigned char _msgType,
            const unsigned char _msgLen [2]);
    CHeader(const unsigned char * _buffer);

    void initWithRawData(const unsigned char * _buffer);
    CHeader(const CHeader & header);
    unsigned char getCheckCode() const;
    std::basic_string<unsigned char> getBuffer() const;

    unsigned char getRESERVED() const {
        return m_RESERVED;
    }

    unsigned char getSOH() const {
        return m_SOH;
    }

    unsigned char getDestination() const {
        return m_destination;
    }

    std::basic_string<unsigned char> getMsgLen() const {
        return m_msgLen;
    }
    int getMsgLenInt() const {
        return (int)((((m_msgLen[0]>='A')?(m_msgLen[0]-'A'+10)
                :(m_msgLen[0]-'0'))<<4)+((m_msgLen[1]>='A')?(m_msgLen[1]-'A'+10):(m_msgLen[1]-'0')));  
    }
    int getLen() const {
        return 7;
    }

    unsigned char getMsgType() const {
        return m_msgType;
    }

    unsigned char getSource() const {
        return m_source;
    }
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

