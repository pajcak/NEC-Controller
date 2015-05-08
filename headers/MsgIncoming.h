//    File: MsgIncoming.h
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

#ifndef __MSGINCOMING_H__
#define	__MSGINCOMING_H__

#include "CAbstractMessage.h"
#include "IReceivable.h"

class CMsgGetCurrParamReply : public CAbstractMessage {
public:
                                     CMsgGetCurrParamReply();
                                     CMsgGetCurrParamReply(const unsigned char * _buffer);
                                     CMsgGetCurrParamReply(const CMsgGetCurrParamReply &);
                                     ~CMsgGetCurrParamReply();
                                     
    void                               initWithRawData(const unsigned char * _buffer);
    unsigned char                    getCheckCode() const;
    int                              getLengthInt() const;
    std::basic_string<unsigned char> getLength() const;
    std::basic_string<unsigned char> getBuffer() const; // MAYBE NOT NEEDED
    int                              getMaxValue() const;
    int                              getCurrValue() const;
private:
    unsigned char m_result [2]; /*Result code - 0x30,0x30(no error) / 0x30,0x31(Unsupported operation) */
    unsigned char m_opCodePage [2]; /*Operation code page*/
    unsigned char m_opCode [2]; /*Operation code*/
    unsigned char m_type [2]; /*Operation type code - 0x30,0x30(set parameter) / 0x30,0x31(momentary)*/
    unsigned char m_maxValue[4]; /*Maximum value which monitor can accept.*/
    unsigned char m_currValue[4];
};
//******************************************************************************
class CMsgSetParamReply : public CAbstractMessage {
public:
                                     CMsgSetParamReply();
                                     CMsgSetParamReply(const unsigned char * _buffer);
                                     CMsgSetParamReply(const CMsgSetParamReply&);
                                     ~CMsgSetParamReply();
    void               initWithRawData(const unsigned char * _buffer);
    unsigned char                    getCheckCode() const;
    int                              getLengthInt() const;
    std::basic_string<unsigned char> getLength() const;
    std::basic_string<unsigned char> getBuffer() const; // MAYBE NOT NEEDED
    int                              getMaxValue() const;
    int                              getCurrValue() const;
private:
    unsigned char m_result [2]; /*Result code - 0x30,0x30(no error) / 0x30,0x31(Unsupported operation) */
    unsigned char m_opCodePage [2]; /*Operation code page*/
    unsigned char m_opCode [2]; /*Operation code*/
    unsigned char m_type [2]; /*Operation type code - 0x30,0x30(set parameter) / 0x30,0x31(momentary)*/
    unsigned char m_maxValue[4]; /*Maximum value that monitor can accept.*/
    unsigned char m_reqSettingVal[4]; /*Echoes back the parameter value for confirmation.*/
};
//*******************************************************************************
/*-------------------COMMAND-MESSAGES-----------------------------------------*/
//*******************************************************************************
class CMsgCommSaveCurrSettingsReply : public CAbstractMessage {
public:
                                     CMsgCommSaveCurrSettingsReply();
                                     CMsgCommSaveCurrSettingsReply(const unsigned char * _buffer);
    void   initWithRawData(const unsigned char * _buffer);
    unsigned char                    getCheckCode() const;
    int                              getLengthInt() const;
    std::basic_string<unsigned char> getLength() const;
    std::basic_string<unsigned char> getBuffer() const; // MAYBE NOT NEEDED
private:
    unsigned char m_commandCode [4]; /* '0','0', '0', 'C' (0x30, 0x30, 0x30, 0x43)*/
};
//******************************************************************************
class CMsgCommPowerStatusReadReply : public CAbstractMessage {
public:
                                     CMsgCommPowerStatusReadReply();
                                     CMsgCommPowerStatusReadReply(const unsigned char * _buffer);
    void   initWithRawData(const unsigned char * _buffer);
    unsigned char                    getCheckCode() const;
    int                              getLengthInt() const;
    std::basic_string<unsigned char> getLength() const;
    std::basic_string<unsigned char> getBuffer() const; // MAYBE NOT NEEDED
    int                              getMaxModeCount() const;
    int                              getCurrMode() const;
private:
    unsigned char m_reserved [2]; /* 0x30,0x32 */
    unsigned char m_result [2]; /*Result code - 0x30,0x30(no error) / 0x30,0x31(Unsupported operation) */
    unsigned char m_commandCode [2]; /* 'D','6' (0x44, 0x36)*/
    unsigned char m_type [2]; /*Operation type code - 0x30,0x30(set parameter) / 0x30,0x31(momentary)*/
    unsigned char m_maxValue[4]; /*Maximum value of power modes. ('0', '0', '0', '4') */
    unsigned char m_currValue[4]; /*Current power mode.('0', '0', '0', '1'->'4')*/
};
//******************************************************************************
class CMsgCommPowerControlReply : public CAbstractMessage {
public:
                                     CMsgCommPowerControlReply();
                                     CMsgCommPowerControlReply(const unsigned char * _buffer);
    void   initWithRawData(const unsigned char * _buffer);
    unsigned char                    getCheckCode() const;
    int                              getLengthInt() const;
    std::basic_string<unsigned char> getLength() const;
    std::basic_string<unsigned char> getBuffer() const; // MAYBE NOT NEEDED
    int                              getCurrMode() const;
private:
    unsigned char m_result [2]; /*Result code - 0x30,0x30(no error) / 0x30,0x31(Unsupported operation) */
    unsigned char m_commandCode [6]; /* 'C', '2', '0', '3', 'D', '6' */
    unsigned char m_powerMode [4]; /* '0', '0', '0', '1'->'4' */
};

//******************************************************************************
class CMsgCommNull : public CAbstractMessage {
public:
                                     CMsgCommNull();
                                     CMsgCommNull(const unsigned char * _buffer);
                                     CMsgCommNull(const CMsgCommNull&);
                                     ~CMsgCommNull();
    void                    initWithRawData(const unsigned char * _buffer);
    unsigned char                    getCheckCode() const;
    int                              getLengthInt() const;
    std::basic_string<unsigned char> getLength() const;
    std::basic_string<unsigned char> getBuffer() const; // MAYBE NOT NEEDED
private:
    unsigned char m_commandCode [2]; /* 'B','E' (0x42, 0x45)*/
//The NULL message returned from the monitor is used in the following cases;
//-> A timeout error has occurred. (The default timeout is 10sec.)
//-> The monitor receives an unsupported message type.
//-> The monitor detects a packet BCC (Block Check Code) error.
//-> To tell the controller that the monitor does not have any answer to give to the host
//    (not ready or not expected)
//      * Following operations need a certain time for to execute, so the monitor will return
//        this message when another message is received during execution.
//
//            Power ON, Power OFF, Auto Setup, Input, PIP Input, Auto Setup and Factory reset.
//
//Complete "NULL Message" command packet as follows;
//    01h-30h-30h-41h-42h-30h-34h-02h-42h-45h-03h-CHK-0Dh
//    SOH-'0'-'0'-'A'-'B'-'0'-'4'-STX-'B'-'E'-ETX-CHK- CR
};


#endif	/* __MSGINCOMING_H__ */