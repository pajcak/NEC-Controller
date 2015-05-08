//    File: CAbstractMessage.h
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

#ifndef __CABSTRACTMESSAGE_H__
#define	__CABSTRACTMESSAGE_H__

#include <string>

#include "IReceivable.h"

class CAbstractMessage : public IReceivable {
public:
                                              CAbstractMessage() {}
                                              CAbstractMessage(const unsigned char * _buffer);
                                              CAbstractMessage(const CAbstractMessage&) {}
    void                                      initWithRawData(const unsigned char * _buffer) {}
    virtual                                   ~CAbstractMessage() {}
    virtual unsigned char                     getCheckCode() const = 0;
    // saves length of certain message into two bytes hi and lo and returns the length in integer
    virtual int                               getLengthInt() const = 0;
    virtual std::basic_string<unsigned char>  getLength() const = 0;
/** \brief Concatenation of all parts of the message.
 *  \return All the message in bytes.
 */
    virtual std::basic_string<unsigned char>  getBuffer() const = 0;
protected:
    
    static const unsigned char s_STX = 0x02;
    static const unsigned char s_ETX = 0x03;    
};

#endif	/* __CABSTRACTMESSAGE_H__ */

