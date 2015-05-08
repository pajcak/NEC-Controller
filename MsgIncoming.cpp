//    File: MsgIncoming.cpp
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

#include "headers/MsgIncoming.h"
#include "headers/Utils.h"
#include <cstdio>


//******************************************************************************
CMsgGetCurrParamReply::CMsgGetCurrParamReply() {

}
/** \brief Fills fields from bytes in parameter.
 * Also checks if the bytes content corresponds to this exact message
 * 
 * @param data
 */
CMsgGetCurrParamReply::CMsgGetCurrParamReply(const unsigned char * _buffer) {
    this->initWithRawData(_buffer);
}
void CMsgGetCurrParamReply::initWithRawData(const unsigned char * _buffer) {
    //s_STX = 0x02;
    //unsigned char result [2]; /*Result code - 0x30,0x30(no error) / 0x30,0x31(Unsupported operation) */
    //unsigned char opCodePage [2]; /*Operation code page*/
    //unsigned char opCode [2]; /*Operation code*/
    //unsigned char type [2]; /*Operation type code - 0x30,0x30(set parameter) / 0x30,0x31(momentary)*/
    //unsigned char maxValue[4]; /*Maximum value which monitor can accept.*/
    //unsigned char currValue[4];
    //s_ETX = 0x03;
    if (_buffer[0] != 0x02) throw "CMsgGetCurrParamReply(const unsigned char*): STX exception\n";
    if (_buffer[1] == 0x30) m_result[0] = _buffer[1];
    else throw "CMsgGetCurrParamReply(const unsigned char*): result exception\n";
    if (_buffer[2] == 0x30 || _buffer[2] == 0x31) m_result[1] = _buffer[2];
    else throw "CMsgGetCurrParamReply(const unsigned char*): result exception\n";
    if (m_result[1] == 0x31) 
        throw "CMsgGetCurrParamReply(const unsigned char*): m_result: unsupported operation\n";
    
    m_opCodePage[0] = _buffer[3];
    m_opCodePage[1] = _buffer[4];
    m_opCode[0]     = _buffer[5]; 
    m_opCode[1]     = _buffer[6];

    if (_buffer[7] == 0x30) m_type[0] = _buffer[7];
    else throw "CMsgGetCurrParamReply(const unsigned char*): operation type code exception\n";
    if (_buffer[8] == 0x30 || _buffer[8] == 0x31) m_type[1] = _buffer[8];
    else throw "CMsgGetCurrParamReply(const unsigned char*): operation type code exception\n";
    m_maxValue[0] = _buffer[9];
    m_maxValue[1] = _buffer[10];
    m_maxValue[2] = _buffer[11];
    m_maxValue[3] = _buffer[12];
    m_currValue[0] = _buffer[13];
    m_currValue[1] = _buffer[14];
    m_currValue[2] = _buffer[15];
    m_currValue[3] = _buffer[16];
    if (_buffer[17] != 0x03) 
        throw "CMsgGetCurrParamReply(const unsigned char*): ETX exception\n";
}
CMsgGetCurrParamReply::CMsgGetCurrParamReply(const CMsgGetCurrParamReply& rhs)
: CAbstractMessage(rhs) {
    //TODO
}

CMsgGetCurrParamReply::~CMsgGetCurrParamReply() {
}

unsigned char CMsgGetCurrParamReply::getCheckCode() const {
    //TODO
    return 'X';
}

int CMsgGetCurrParamReply::getLengthInt() const {
    return 18;
}

std::basic_string<unsigned char> CMsgGetCurrParamReply::getLength() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back('1');
    ustr.push_back('2');
    return ustr;
}

std::basic_string<unsigned char> CMsgGetCurrParamReply::getBuffer() const { // only for debugging
    std::basic_string<unsigned char> ustr;
    ustr.push_back(s_STX);
    ustr.push_back(m_result[0]);
    ustr.push_back(m_result[1]);
    ustr.push_back(m_opCodePage[0]);
    ustr.push_back(m_opCodePage[1]);
    ustr.push_back(m_opCode[0]);
    ustr.push_back(m_opCode[1]);
    ustr.push_back(m_type[0]);
    ustr.push_back(m_type[1]);
    ustr.push_back(m_maxValue[0]);
    ustr.push_back(m_maxValue[1]);
    ustr.push_back(m_maxValue[2]);
    ustr.push_back(m_maxValue[3]);
    ustr.push_back(m_currValue[0]);
    ustr.push_back(m_currValue[1]);
    ustr.push_back(m_currValue[2]);
    ustr.push_back(m_currValue[3]);
    ustr.push_back(s_ETX);
    return ustr;
}
int CMsgGetCurrParamReply::getMaxValue() const {
    return fourBytesToInt(m_maxValue[0], m_maxValue[1], m_maxValue[2], m_maxValue[3]);
}
int CMsgGetCurrParamReply::getCurrValue() const {
    return fourBytesToInt(m_currValue[0], m_currValue[1], m_currValue[2], m_currValue[3]);
}

//******************************************************************************
CMsgSetParamReply::CMsgSetParamReply() {

}

CMsgSetParamReply::CMsgSetParamReply(const unsigned char * _buffer) {
    this->initWithRawData(_buffer);
}
void CMsgSetParamReply::initWithRawData(const unsigned char * _buffer) {
    //unsigned char result [2]; /*Result code - 0x30,0x30(no error) / 0x30,0x31(Unsupported operation) */
    //unsigned char opCodePage [2]; /*Operation code page*/
    //unsigned char opCode [2]; /*Operation code*/
    //unsigned char type [2]; /*Operation type code - 0x30,0x30(set parameter) / 0x30,0x31(momentary)*/
    //unsigned char maxValue[4]; /*Maximum value that monitor can accept.*/
    //unsigned char reqSettingVal[4]; /*Echoes back the parameter for confirmation.*/
    
    if (_buffer[0] != 0x02) throw "CMsgSetParamReply(const unsigned char*): STX exception\n";
    if (_buffer[1] == 0x30) m_result[0] = _buffer[1];
    else throw "CMsgSetParamReply(const unsigned char*): result exception\n";
    if (_buffer[2] == 0x30 || _buffer[2] == 0x31) m_result[1] = _buffer[2];
    else throw "CMsgSetParamReply(const unsigned char*): result exception\n";
    if (m_result[1] == 0x31) 
        throw "CMsgSetParamReply(const unsigned char*): m_result: unsupported operation\n";
    
    m_opCodePage[0] = _buffer[3];
    m_opCodePage[1] = _buffer[4];
    m_opCode[0]     = _buffer[5]; 
    m_opCode[1]     = _buffer[6];

    if (_buffer[7] == 0x30) m_type[0] = _buffer[7];
    else throw "CMsgSetParamReply(const unsigned char*): operation type code exception\n";
    if (_buffer[8] == 0x30 || _buffer[8] == 0x31) m_type[1] = _buffer[8];
    else throw "CMsgSetParamReply(const unsigned char*): operation type code exception\n";
    m_maxValue[0] = _buffer[9];
    m_maxValue[1] = _buffer[10];
    m_maxValue[2] = _buffer[11];
    m_maxValue[3] = _buffer[12];
    m_reqSettingVal[0] = _buffer[13];
    m_reqSettingVal[1] = _buffer[14];
    m_reqSettingVal[2] = _buffer[15];
    m_reqSettingVal[3] = _buffer[16];
    if (_buffer[17] != 0x03) 
        throw "CMsgSetParamReply(const unsigned char*): ETX exception\n";
}
CMsgSetParamReply::CMsgSetParamReply(const CMsgSetParamReply & rhs)
: CAbstractMessage(rhs) {
    //TODO
}

CMsgSetParamReply::~CMsgSetParamReply() {
}

unsigned char CMsgSetParamReply::getCheckCode() const {
    //TODO
    return 'X';
}

int CMsgSetParamReply::getLengthInt() const {
    return 18;
}

std::basic_string<unsigned char> CMsgSetParamReply::getLength() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back('1');
    ustr.push_back('2');
    return ustr;
}

std::basic_string<unsigned char> CMsgSetParamReply::getBuffer() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back(s_STX);
    ustr.push_back(m_result[0]);
    ustr.push_back(m_result[1]);
    ustr.push_back(m_opCodePage[0]);
    ustr.push_back(m_opCodePage[1]);
    ustr.push_back(m_opCode[0]);
    ustr.push_back(m_opCode[1]);
    ustr.push_back(m_type[0]);
    ustr.push_back(m_type[1]);
    ustr.push_back(m_maxValue[0]);
    ustr.push_back(m_maxValue[1]);
    ustr.push_back(m_maxValue[2]);
    ustr.push_back(m_maxValue[3]);
    ustr.push_back(m_reqSettingVal[0]);
    ustr.push_back(m_reqSettingVal[1]);
    ustr.push_back(m_reqSettingVal[2]);
    ustr.push_back(m_reqSettingVal[3]);
    ustr.push_back(s_ETX);
    return ustr;
}
int                              CMsgSetParamReply::getMaxValue() const {
   return fourBytesToInt(m_maxValue[0], m_maxValue[1], m_maxValue[2], m_maxValue[3]); 
}
int                              CMsgSetParamReply::getCurrValue() const {
    return fourBytesToInt(m_reqSettingVal[0], m_reqSettingVal[1],
                          m_reqSettingVal[2], m_reqSettingVal[3]);
}
//******************************************************************************
CMsgCommSaveCurrSettingsReply::CMsgCommSaveCurrSettingsReply() {

}

CMsgCommSaveCurrSettingsReply::CMsgCommSaveCurrSettingsReply(const unsigned char * _buffer) {
    this->initWithRawData(_buffer);
}

void CMsgCommSaveCurrSettingsReply::initWithRawData(const unsigned char * _buffer) {
    if (_buffer[0] != 0x02) 
        throw "CMsgCommSaveCurrSettingsReply(const unsigned char*): STX exception\n";
    if (_buffer[1] == 0x30) m_commandCode[0] = _buffer[1];
    else throw "CMsgCommSaveCurrSettingsReply(const unsigned char*): command code exception\n";
    if (_buffer[2] == 0x30) m_commandCode[1] = _buffer[2];
    else throw "CMsgCommSaveCurrSettingsReply(const unsigned char*): command code exception\n";
    if (_buffer[3] == 0x30) m_commandCode[2] = _buffer[3];
    else throw "CMsgCommSaveCurrSettingsReply(const unsigned char*): command code exception\n";
    if (_buffer[4] == 0x43) m_commandCode[3] = _buffer[4];
    else throw "CMsgCommSaveCurrSettingsReply(const unsigned char*): command code exception\n";
        
    if (_buffer[5] != 0x03) 
        throw "CMsgCommSaveCurrSettingsReply(const unsigned char*): ETX exception\n";
}

unsigned char CMsgCommSaveCurrSettingsReply::getCheckCode() const {
    //TODO
    return 'X';
}

int CMsgCommSaveCurrSettingsReply::getLengthInt() const {
    return 6;
}

std::basic_string<unsigned char> CMsgCommSaveCurrSettingsReply::getLength() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back('0');
    ustr.push_back('6');
    return ustr;
}

std::basic_string<unsigned char> CMsgCommSaveCurrSettingsReply::getBuffer() const {
//    unsigned char m_commandCode [4]; /* '0','0', '0', 'C' (0x30, 0x30, 0x30, 0x43)*/
    std::basic_string<unsigned char> ustr;
    ustr.push_back(s_STX);
    ustr.push_back(m_commandCode[0]);
    ustr.push_back(m_commandCode[1]);
    ustr.push_back(m_commandCode[2]);
    ustr.push_back(m_commandCode[3]);
    ustr.push_back(s_ETX);
    return ustr;
        
}
//******************************************************************************
CMsgCommPowerStatusReadReply::CMsgCommPowerStatusReadReply() {

}
CMsgCommPowerStatusReadReply::CMsgCommPowerStatusReadReply(const unsigned char * _buffer) {
    this->initWithRawData(_buffer);
}
void CMsgCommPowerStatusReadReply::initWithRawData(const unsigned char * _buffer) {
//    unsigned char m_reserved [2]; /* 0x30,0x32 */
//    unsigned char m_result [2]; /*Result code - 0x30,0x30(no error) / 0x30,0x31(Unsupported operation) */
//    unsigned char m_commandCode [2]; /* 'D','6' (0x44, 0x36)*/
//    unsigned char m_type [2]; /*Operation type code - 0x30,0x30(set parameter) / 0x30,0x31(momentary)*/
//    unsigned char m_maxValue[4]; /*Maximum value of power modes. ('0', '0', '0', '4') */
//    unsigned char m_currValue[4]; /*Current power mode.('0', '0', '0', '1'->'4')*/
//for (int i = 0; i < 18; i++) printf("Received[%d]: 0x%02x | %c\n", i, _buffer[i], _buffer[i]);

    if (_buffer[0] != 0x02) throw "CMsgCommPowerStatusReadReply(const unsigned char*): STX exception\n";
    
    if (_buffer[1] != 0x30 || _buffer[2] != 0x32)
        throw "CMsgCommPowerStatusReadReply(const unsigned char*): reserved exception\n";
    else {
        m_reserved[0] = _buffer[1];
        m_reserved[1] = _buffer[2];
    }
    if (_buffer[3] == 0x30 && (_buffer[4] == 0x30 || _buffer[4] == 0x31)) {
        m_result[0] = _buffer[3];
        m_result[1] = _buffer[4];
    } else throw "CMsgCommPowerStatusReadReply(const unsigned char*): result exception\n";
    if (m_result[1] == 0x31) 
        throw "CMsgCommPowerStatusReadReply(const unsigned char*): m_result: unsupported operation\n";
    
    if (_buffer[5] == 0x44 && _buffer[6] == 0x36) {
        m_commandCode[0] = _buffer[5];
        m_commandCode[1] = _buffer[6];
    } else throw "CMsgCommPowerStatusReadReply(const unsigned char*): command code exception\n";

    if (_buffer[7] == 0x30) m_type[0] = _buffer[7];
    else throw "CMsgCommPowerStatusReadReply(const unsigned char*): operation type code exception\n";
    if (_buffer[8] == 0x30 || _buffer[8] == 0x31) m_type[1] = _buffer[8];
    else throw "CMsgCommPowerStatusReadReply(const unsigned char*): operation type code exception\n";
    
    if (_buffer[9] == 0x30) m_maxValue[0] = _buffer[9];
    else throw "CMsgCommPowerStatusReadReply(const unsigned char*): max value exception\n";
    if (_buffer[10] == 0x30) m_maxValue[1] = _buffer[10];
    else throw "CMsgCommPowerStatusReadReply(const unsigned char*): max value exception\n";
    if (_buffer[11] == 0x30) m_maxValue[2] = _buffer[11];
    else throw "CMsgCommPowerStatusReadReply(const unsigned char*): max value exception\n";
    if (_buffer[12] == 0x34) m_maxValue[3] = _buffer[12];
    else throw "CMsgCommPowerStatusReadReply(const unsigned char*): max value exception\n";
    
    if (_buffer[13] == 0x30) m_currValue[0] = _buffer[13];
    else throw "CMsgCommPowerStatusReadReply(const unsigned char*): curr value exception\n";
    if (_buffer[14] == 0x30) m_currValue[1] = _buffer[14];
    else throw "CMsgCommPowerStatusReadReply(const unsigned char*): curr value exception\n";
    if (_buffer[15] == 0x30) m_currValue[2] = _buffer[15];
    else throw "CMsgCommPowerStatusReadReply(const unsigned char*): curr value exception\n";
    m_currValue[3] = _buffer[16];
    
    if (_buffer[17] != 0x03) 
        throw "CMsgCommPowerStatusReadReply(const unsigned char*): ETX exception\n";
}

unsigned char CMsgCommPowerStatusReadReply::getCheckCode() const {
    //TODO
    return 'X';
}

int CMsgCommPowerStatusReadReply::getLengthInt() const {
    return 18;
}

std::basic_string<unsigned char> CMsgCommPowerStatusReadReply::getLength() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back('1');
    ustr.push_back('2');
    return ustr;
}

std::basic_string<unsigned char> CMsgCommPowerStatusReadReply::getBuffer() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back(s_STX);
    ustr.push_back(m_reserved[0]);
    ustr.push_back(m_reserved[1]);
    ustr.push_back(m_result[0]);
    ustr.push_back(m_result[1]);
    ustr.push_back(m_commandCode[0]);
    ustr.push_back(m_commandCode[1]);
    ustr.push_back(m_type[0]);
    ustr.push_back(m_type[1]);
    ustr.push_back(m_maxValue[0]);
    ustr.push_back(m_maxValue[1]);
    ustr.push_back(m_maxValue[2]);
    ustr.push_back(m_maxValue[3]);
    ustr.push_back(m_currValue[0]);
    ustr.push_back(m_currValue[1]);
    ustr.push_back(m_currValue[2]);
    ustr.push_back(m_currValue[3]);
    ustr.push_back(s_ETX);
    return ustr;
}
int                              CMsgCommPowerStatusReadReply::getMaxModeCount() const {
   return fourBytesToInt(m_maxValue[0], m_maxValue[1], m_maxValue[2], m_maxValue[3]); 
}
int                              CMsgCommPowerStatusReadReply::getCurrMode() const {
    return fourBytesToInt(m_currValue[0], m_currValue[1],
                          m_currValue[2], m_currValue[3]);
}
//******************************************************************************
CMsgCommPowerControlReply::CMsgCommPowerControlReply() {

}
CMsgCommPowerControlReply::CMsgCommPowerControlReply(const unsigned char * _buffer) {
    this->initWithRawData(_buffer);
}
void CMsgCommPowerControlReply::initWithRawData(const unsigned char * _buffer) {
//    unsigned char m_result [2]; /*Result code - 0x30,0x30(no error) / 0x30,0x31(Unsupported operation) */
//    unsigned char m_commandCode [6]; /* 'C', '2', '0', '3', 'D', '6' */
//    unsigned char m_powerMode [4]; /* '0', '0', '0', '1'->'4' */

    if (_buffer[0] != 0x02) throw "CMsgCommPowerControlReply(const unsigned char*): STX exception\n";
    
    if (_buffer[1] == 0x30 && (_buffer[2] == 0x30 || _buffer[2] == 0x31)) {
        m_result[0] = _buffer[1];
        m_result[1] = _buffer[2];
    } else throw "CMsgCommPowerControlReply(const unsigned char*): result exception\n";
    if (m_result[1] == 0x31) 
        throw "CMsgCommPowerControlReply(const unsigned char*): m_result: unsupported operation\n";
    
    if (_buffer[3] == 0x43 && _buffer[4] == 0x32 && _buffer[5] == 0x30
        && _buffer[6] == 0x33 && _buffer[7] == 0x44 && _buffer[8] == 0x36) {
        m_commandCode[0] = _buffer[3];
        m_commandCode[1] = _buffer[4];
        m_commandCode[2] = _buffer[5];
        m_commandCode[3] = _buffer[6];
        m_commandCode[4] = _buffer[7];
        m_commandCode[5] = _buffer[8];
    } else throw "CMsgCommPowerControlReply(const unsigned char*): command code exception.\n";

    if (_buffer[9] == 0x30) m_powerMode[0] = _buffer[9];
    else throw "CMsgCommPowerControlReply(const unsigned char*): power mode exception\n";
    if (_buffer[10] == 0x30) m_powerMode[1] = _buffer[10];
    else throw "CMsgCommPowerControlReply(const unsigned char*): power mode exception\n";
    if (_buffer[11] == 0x30) m_powerMode[2] = _buffer[11];
    else throw "CMsgCommPowerControlReply(const unsigned char*): pwoer mode exception\n";
    m_powerMode[3] = _buffer[12];
    
    if (_buffer[13] != 0x03) 
        throw "CMsgCommPowerControlReply(const unsigned char*): ETX exception\n";
}

unsigned char CMsgCommPowerControlReply::getCheckCode() const {
    //TODO
    return 'X';
}

int CMsgCommPowerControlReply::getLengthInt() const {
    return 14;
}

std::basic_string<unsigned char> CMsgCommPowerControlReply::getLength() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back('0');
    ustr.push_back('E');
    return ustr;
}

std::basic_string<unsigned char> CMsgCommPowerControlReply::getBuffer() const {
//    unsigned char m_result [2]; /*Result code - 0x30,0x30(no error) / 0x30,0x31(Unsupported operation) */
//    unsigned char m_commandCode [6]; /* 'C', '2', '0', '3', 'D', '6' */
//    unsigned char m_powerMode [4]; /* '0', '0', '0', '1'->'4' */
    std::basic_string<unsigned char> ustr;
    ustr.push_back(s_STX);
    ustr.push_back(m_result[0]);
    ustr.push_back(m_result[1]);
    ustr.push_back(m_commandCode[0]);
    ustr.push_back(m_commandCode[1]);
    ustr.push_back(m_commandCode[2]);
    ustr.push_back(m_commandCode[3]);
    ustr.push_back(m_commandCode[4]);
    ustr.push_back(m_commandCode[5]);
    ustr.push_back(m_powerMode[0]);
    ustr.push_back(m_powerMode[1]);
    ustr.push_back(m_powerMode[2]);
    ustr.push_back(m_powerMode[3]);
    ustr.push_back(s_ETX);
    return ustr;
}
int                              CMsgCommPowerControlReply::getCurrMode() const {
    return fourBytesToInt(m_powerMode[0], m_powerMode[1],
                          m_powerMode[2], m_powerMode[3]);
}
//******************************************************************************

CMsgCommNull::CMsgCommNull() {
    //unsigned char commandCode [2]; /* 'B','E' (0x42, 0x45)*/
    m_commandCode[0] = 0x42;
    m_commandCode[1] = 0x45;
}
CMsgCommNull::CMsgCommNull(const unsigned char * _buffer) {

}
void CMsgCommNull::initWithRawData(const unsigned char * _buffer) {
}

CMsgCommNull::CMsgCommNull(const CMsgCommNull& rhs)
: CAbstractMessage(rhs) {
    //TODO
}

CMsgCommNull::~CMsgCommNull() {
}


unsigned char CMsgCommNull::getCheckCode() const {
    //TODO
    return 'X';
}

int CMsgCommNull::getLengthInt() const {
    return 4;
}

std::basic_string<unsigned char> CMsgCommNull::getLength() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back('0');
    ustr.push_back('4');
    return ustr;
}
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

std::basic_string<unsigned char> CMsgCommNull::getBuffer() const { // MAYBE NOT NEEDED
    std::basic_string<unsigned char> ustr;
    ustr.push_back(s_STX);
    ustr.push_back(m_commandCode[0]);
    ustr.push_back(m_commandCode[1]);
    ustr.push_back(s_ETX);
    return ustr;
}
//******************************************************************************