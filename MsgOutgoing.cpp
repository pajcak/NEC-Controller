#include "headers/MsgOutgoing.h"
#include <cstring> //std::basic_string()
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

//******************************************************************************
CMsgGetCurrParam::CMsgGetCurrParam(unsigned char _opCodePage[2], unsigned char _opCode[2]) {
    m_opCodePage[0] = _opCodePage[0];
    m_opCodePage[1] = _opCodePage[1];
    m_opCode[0] = _opCode[0];
    m_opCode[1] = _opCode[1];
    //    unsigned char opCodePage [2]; /*Operation code page*/
    //    unsigned char opCode [2]; /*Operation code*/
}

CMsgGetCurrParam::CMsgGetCurrParam(const CMsgGetCurrParam& rhs)
: CAbstractMessage(rhs) {
    m_opCodePage[0] = rhs.m_opCodePage[0];
    m_opCodePage[1] = rhs.m_opCodePage[1];
    m_opCode[0] = rhs.m_opCode[0];
    m_opCode[1] = rhs.m_opCode[1];
}

CMsgGetCurrParam::~CMsgGetCurrParam() {
}

CAbstractMessage* CMsgGetCurrParam::clone() const {
    return new CMsgGetCurrParam(*this);    
}

unsigned char CMsgGetCurrParam::getCheckCode() const {
    return (s_STX ^ m_opCodePage[0] ^ m_opCodePage[1] ^ m_opCode[0] ^ m_opCode[1] ^ s_ETX);
}

int CMsgGetCurrParam::getLength(unsigned char & hi, unsigned char & lo) const {
    //    (int)((((hi>='A')?(hi-'A'+10):(hi-'0'))<<4)+((lo>='A')?(lo-'A'+10):(lo-'0')))
    hi = '0';
    lo = '6';
    return 6;
}

std::basic_string<unsigned char> CMsgGetCurrParam::getLength() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back('0');
    ustr.push_back('6');
    return ustr;
}

std::basic_string<unsigned char> CMsgGetCurrParam::getBuffer() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back(s_STX);
    ustr.push_back(m_opCodePage[0]);
    ustr.push_back(m_opCodePage[1]);
    ustr.push_back(m_opCode[0]);
    ustr.push_back(m_opCode[1]);
    ustr.push_back(s_ETX);
    return ustr;
}
//******************************************************************************

CMsgSetParam::CMsgSetParam(unsigned char _opCodePage[2], unsigned char _opCode[2],
        unsigned char _setValue[4]) {
    m_opCodePage[0] = _opCodePage[0];
    m_opCodePage[1] = _opCodePage[1];
    m_opCode[0] = _opCode[0];
    m_opCode[1] = _opCode[1];
    m_setValue[0] = _setValue[0];
    m_setValue[1] = _setValue[1];
    m_setValue[2] = _setValue[2];
    m_setValue[3] = _setValue[3];
}

CMsgSetParam::CMsgSetParam(const CMsgSetParam& rhs) {
    m_opCodePage[0] = rhs.m_opCodePage[0];
    m_opCodePage[1] = rhs.m_opCodePage[1];
    m_opCode[0] = rhs.m_opCode[0];
    m_opCode[1] = rhs.m_opCode[1];
    m_setValue[0] = rhs.m_setValue[0];
    m_setValue[1] = rhs.m_setValue[1];
    m_setValue[2] = rhs.m_setValue[2];
    m_setValue[3] = rhs.m_setValue[3];
}

CMsgSetParam::~CMsgSetParam() {
}

CAbstractMessage* CMsgSetParam::clone() const {
    return new CMsgSetParam(*this);    
}

unsigned char CMsgSetParam::getCheckCode() const {
    return (s_STX ^ m_opCodePage[0] ^ m_opCodePage[1] ^ m_opCode[0] ^ m_opCode[1]
            ^ m_setValue[0] ^ m_setValue[1] ^ m_setValue[2] ^ m_setValue[3] ^ s_ETX);
}

int CMsgSetParam::getLength(unsigned char & hi, unsigned char & lo) const {
    hi = '0';
    lo = 'A';
    return 10;
}

std::basic_string<unsigned char> CMsgSetParam::getLength() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back('0');
    ustr.push_back('A');
    return ustr;
}


/** \brief Concatenation of all parts of the message.
 *  \return All the message in bytes.
 */
std::basic_string<unsigned char> CMsgSetParam::getBuffer() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back(s_STX);
    ustr.push_back(m_opCodePage[0]);
    ustr.push_back(m_opCodePage[1]);
    ustr.push_back(m_opCode[0]);
    ustr.push_back(m_opCode[1]);
    ustr.push_back(m_setValue[0]);
    ustr.push_back(m_setValue[1]);
    ustr.push_back(m_setValue[2]);
    ustr.push_back(m_setValue[3]);
    ustr.push_back(s_ETX);
    return ustr;
}
//******************************************************************************

/*-------------------COMMAND-MESSAGES-----------------------------------------*/
CMsgCommSaveCurrSettings::CMsgCommSaveCurrSettings() { // no params, bcs commandCode is already defined
    m_commandCode[0] = 0x30;
    m_commandCode[1] = 0x43;
    //unsigned char commandCode [2]; /* 'O','C' (0x30, 0x43)*/
}
CMsgCommSaveCurrSettings::CMsgCommSaveCurrSettings(const CMsgCommSaveCurrSettings& rhs) { 
    m_commandCode[0] = 0x30;
    m_commandCode[1] = 0x43;
}

CMsgCommSaveCurrSettings::~CMsgCommSaveCurrSettings() {
}

CAbstractMessage* CMsgCommSaveCurrSettings::clone() const {
    return new CMsgCommSaveCurrSettings(*this);    
}

unsigned char CMsgCommSaveCurrSettings::getCheckCode() const {
    return (s_STX ^ m_commandCode[0] ^ m_commandCode[1] ^ s_ETX);
}

int CMsgCommSaveCurrSettings::getLength(unsigned char & hi, unsigned char & lo) const {
    hi = '0';
    lo = '4';
    return 4;
}

std::basic_string<unsigned char> CMsgCommSaveCurrSettings::getLength() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back('0');
    ustr.push_back('4');
    return ustr;
}
// saves all bytes to _outBuffer and returns the number of bytes saved

std::basic_string<unsigned char> CMsgCommSaveCurrSettings::getBuffer() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back(s_STX);
    ustr.push_back(m_commandCode[0]);
    ustr.push_back(m_commandCode[1]);
    ustr.push_back(s_ETX);
    return ustr;
}
//Complete "Save Current setting" command packet as follows;
// ASCII: 01h-30h-41h-30h-41h-30h-34h-02h-30h-43h-03h-CHK-0Dh
// SOH-'0'-'A'-'0'-'A'-'0'-'4'-STX-'0'-'C'-ETX-CHK- CR 
// The monitor replies the packet for confirmation as follows;
// SOH-'0'-'0'-'A'-'B'-'0'-'6'-STX-'0'-'0'-'0'-'C'-ETX-CHK- CR

//******************************************************************************

CMsgCommGetTiming::CMsgCommGetTiming() {// no params, bcs commandCode is already defined
    m_commandCode[0] = 0x30;
    m_commandCode[1] = 0x37;
    //unsigned char commandCode [2]; /* 'O','7' (0x30, 0x37)*/
}
CMsgCommGetTiming::CMsgCommGetTiming(const CMsgCommGetTiming& rhs) {
    m_commandCode[0] = 0x30;
    m_commandCode[1] = 0x37;
}

CMsgCommGetTiming::~CMsgCommGetTiming() {
}

CAbstractMessage* CMsgCommGetTiming::clone() const {
    return new CMsgCommGetTiming(*this);    
}

unsigned char CMsgCommGetTiming::getCheckCode() const {
    return (s_STX ^ m_commandCode[0] ^ m_commandCode[1] ^ s_ETX);
}

int CMsgCommGetTiming::getLength(unsigned char & hi, unsigned char & lo) const {
    hi = '0';
    lo = '4';
    return 4;
}

std::basic_string<unsigned char> CMsgCommGetTiming::getLength() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back('0');
    ustr.push_back('4');
    return ustr;
}

// saves all bytes to _outBuffer and returns the number of bytes saved

std::basic_string<unsigned char> CMsgCommGetTiming::getBuffer() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back(s_STX);
    ustr.push_back(m_commandCode[0]);
    ustr.push_back(m_commandCode[1]);
    ustr.push_back(s_ETX);
    return ustr;
}
//******************************************************************************