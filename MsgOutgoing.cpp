#include "headers/MsgOutgoing.h"
#include "headers/Utils.h"
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
CMsgGetCurrParam::CMsgGetCurrParam(const unsigned char _opCodePage[2], const unsigned char _opCode[2]) {
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

unsigned char CMsgGetCurrParam::getCheckCode() const {
    return (s_STX ^ m_opCodePage[0] ^ m_opCodePage[1] ^ m_opCode[0] ^ m_opCode[1] ^ s_ETX);
}

int CMsgGetCurrParam::getLengthInt() const {
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

CMsgSetParam::CMsgSetParam(const unsigned char _opCodePage[2], const unsigned char _opCode[2],
        const unsigned char _setValue[4]) {
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

unsigned char CMsgSetParam::getCheckCode() const {
    return (s_STX ^ m_opCodePage[0] ^ m_opCodePage[1] ^ m_opCode[0] ^ m_opCode[1]
            ^ m_setValue[0] ^ m_setValue[1] ^ m_setValue[2] ^ m_setValue[3] ^ s_ETX);
}

int CMsgSetParam::getLengthInt() const {
    return 10;
}

std::basic_string<unsigned char> CMsgSetParam::getLength() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back('0');
    ustr.push_back('A');
    return ustr;
}

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

unsigned char CMsgCommSaveCurrSettings::getCheckCode() const {
    return (s_STX ^ m_commandCode[0] ^ m_commandCode[1] ^ s_ETX);
}

int CMsgCommSaveCurrSettings::getLengthInt() const {
    return 4;
}

std::basic_string<unsigned char> CMsgCommSaveCurrSettings::getLength() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back('0');
    ustr.push_back('4');
    return ustr;
}
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
CMsgCommPowerStatusRead::CMsgCommPowerStatusRead() { // no params, bcs commandCode is already defined
    m_commandCode[0] = 0x30;
    m_commandCode[1] = 0x31;
    m_commandCode[2] = 0x44;
    m_commandCode[3] = 0x36;
//    unsigned char m_commandCode [4]; /* 'O', '1', 'D', '6' */
}

unsigned char CMsgCommPowerStatusRead::getCheckCode() const {
    return (s_STX ^ m_commandCode[0] ^ m_commandCode[1] ^ m_commandCode[2] ^ m_commandCode[3] ^ s_ETX);
}

int CMsgCommPowerStatusRead::getLengthInt() const {
    return 6;
}

std::basic_string<unsigned char> CMsgCommPowerStatusRead::getLength() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back('0');
    ustr.push_back('6');
    return ustr;
}
std::basic_string<unsigned char> CMsgCommPowerStatusRead::getBuffer() const {
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
CMsgCommPowerControl::CMsgCommPowerControl(unsigned char _powerMode) { // no params, bcs commandCode is already defined
//    unsigned char m_commandCode [6]; /* 'C', '2', '0', '3', 'D', '6' */
//    unsigned char m_powerMode [4]; /* '0', '0', '0', '1'->'4' */
    m_commandCode[0] = 0x43;
    m_commandCode[1] = 0x32;
    m_commandCode[2] = 0x30;
    m_commandCode[3] = 0x33;
    m_commandCode[4] = 0x44;
    m_commandCode[5] = 0x36;
    m_powerMode[0] = 0x30;
    m_powerMode[1] = 0x30;
    m_powerMode[2] = 0x30;
    if (_powerMode == 0x31 || _powerMode == 0x32 || _powerMode == 0x33 || _powerMode == 0x34)
        m_powerMode[3] = _powerMode;
    else throw "CMsgCommPowerControl(unsigned char): invalid power mode value.";
}

unsigned char CMsgCommPowerControl::getCheckCode() const {
    return (s_STX ^
            m_commandCode[0] ^
            m_commandCode[1] ^
            m_commandCode[2] ^
            m_commandCode[3] ^
            m_commandCode[4] ^
            m_commandCode[5] ^
            m_powerMode[0] ^
            m_powerMode[1] ^
            m_powerMode[2] ^
            m_powerMode[3] ^
            s_ETX);
}

int CMsgCommPowerControl::getLengthInt() const {
    return 12;
}

std::basic_string<unsigned char> CMsgCommPowerControl::getLength() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back('0');
    ustr.push_back('C');
    return ustr;
}
std::basic_string<unsigned char> CMsgCommPowerControl::getBuffer() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back(s_STX);
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
