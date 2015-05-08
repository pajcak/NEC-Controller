//    File: MsgOutgoing.h
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

#ifndef __MSGCONTROLLER_H__
#define __MSGCONTROLLER_H__
/*TIPS: Distribute message classes for receive to other header file*/
#include "CAbstractMessage.h"

//******************************************************************************
class CMsgGetCurrParam : public CAbstractMessage {
public:
                                     CMsgGetCurrParam(const unsigned char _opCodePage[2],
                                             const unsigned char _opCode[2]);
                                     CMsgGetCurrParam(const CMsgGetCurrParam&);
                                     ~CMsgGetCurrParam();
    unsigned char                    getCheckCode() const;
    int                              getLengthInt() const;
    std::basic_string<unsigned char> getLength() const;
    std::basic_string<unsigned char> getBuffer() const;
private:
    unsigned char m_opCodePage [2]; /*Operation code page*/
    unsigned char m_opCode [2]; /*Operation code*/
};
//******************************************************************************
class CMsgSetParam : public CAbstractMessage {
public:
    
                                     CMsgSetParam(const unsigned char _opCodePage[2],
                                             const unsigned char _opCode[2],
                                                 const unsigned char _setValue[4]);
                                     CMsgSetParam(const CMsgSetParam&);
                                     ~CMsgSetParam();
    unsigned char                    getCheckCode() const;
    int                              getLengthInt() const;
    std::basic_string<unsigned char> getLength() const;
    std::basic_string<unsigned char> getBuffer() const;
private:
    unsigned char m_opCodePage [2]; /*Operation code page*/
    unsigned char m_opCode [2]; /*Operation code*/
    unsigned char m_setValue[4];
};
//******************************************************************************
/*-------------------COMMAND-MESSAGES-----------------------------------------*/
//*******************************************************************************
class CMsgCommSaveCurrSettings : public CAbstractMessage {
public:
                                     CMsgCommSaveCurrSettings(); // no params, bcs commandCode is already defined
    unsigned char                    getCheckCode() const;
    int                              getLengthInt() const;
    std::basic_string<unsigned char> getLength() const;
    std::basic_string<unsigned char> getBuffer() const;
private:
    unsigned char m_commandCode [2]; /* 'O','C' (0x30, 0x43)*/

//Complete "Save Current setting" command packet as follows;
// ASCII: 01h-30h-41h-30h-41h-30h-34h-02h-30h-43h-03h-CHK-0Dh
// SOH-'0'-'A'-'0'-'A'-'0'-'4'-STX-'0'-'C'-ETX-CHK- CR 
// The monitor replies the packet for confirmation as follows;
// SOH-'0'-'0'-'A'-'B'-'0'-'6'-STX-'0'-'0'-'0'-'C'-ETX-CHK- CR
};
//******************************************************************************
class CMsgCommPowerStatusRead : public CAbstractMessage {
public:
                                     CMsgCommPowerStatusRead(); // no params, bcs commandCode is already defined
    unsigned char                    getCheckCode() const;
    int                              getLengthInt() const;
    std::basic_string<unsigned char> getLength() const;
    std::basic_string<unsigned char> getBuffer() const;
private:
    unsigned char m_commandCode [4]; /* 'O', '1', 'D', '6' */
};
//******************************************************************************
class CMsgCommPowerControl : public CAbstractMessage {
public:
                                     CMsgCommPowerControl(unsigned char _powerMode); // no params, bcs commandCode is already defined
    unsigned char                    getCheckCode() const;
    int                              getLengthInt() const;
    std::basic_string<unsigned char> getLength() const;
    std::basic_string<unsigned char> getBuffer() const;
private:
    unsigned char m_commandCode [6]; /* 'C', '2', '0', '3', 'D', '6' */
    unsigned char m_powerMode [4]; /* '0', '0', '0', '1'->'4' */
};

#endif /* __MSGCONTROLLER_H__ */