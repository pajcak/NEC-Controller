#include "headers/MsgIncoming.h"

CMsgGetCurrParamReply::CMsgGetCurrParamReply(const unsigned char * _buffer) {
    //unsigned char result [2]; /*Result code - 0x30,0x30(no error) / 0x30,0x31(Unsupported operation) */
    //unsigned char opCodePage [2]; /*Operation code page*/
    //unsigned char opCode [2]; /*Operation code*/
    //unsigned char type [2]; /*Operation type code - 0x30,0x30(set parameter) / 0x30,0x31(momentary)*/
    //unsigned char maxValue[4]; /*Maximum value which monitor can accept.*/
    //unsigned char currValue[4];
}

CMsgGetCurrParamReply::CMsgGetCurrParamReply(unsigned char _opCodePage[2], unsigned char _opCode[2]) {
    m_opCodePage[0] = _opCodePage[0];
    m_opCodePage[1] = _opCodePage[1];
    m_opCode[0] = _opCode[0];
    m_opCode[1] = _opCode[1];

}
CMsgGetCurrParamReply::CMsgGetCurrParamReply(const CMsgGetCurrParamReply& rhs)
: CAbstractMessage(rhs) {
    //TODO
}

CMsgGetCurrParamReply::~CMsgGetCurrParamReply() {
}

CAbstractMessage* CMsgGetCurrParamReply::clone() const {
    return new CMsgGetCurrParamReply(*this);
}

unsigned char CMsgGetCurrParamReply::getCheckCode() const {
}

int CMsgGetCurrParamReply::getLength(unsigned char & hi, unsigned char & lo) const {
    hi = '1';
    lo = '2';
    return 18;
}

std::basic_string<unsigned char> CMsgGetCurrParamReply::getLength() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back('1');
    ustr.push_back('2');
    return ustr;
}

std::basic_string<unsigned char> CMsgGetCurrParamReply::getBuffer() const { // MAYBE NOT NEEDED
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

/** \brief Fills fields from bytes in parameter.
 * Also checks if the bytes content corresponds to this exact message
 * 
 * @param data
 * @return Flag indicating if init was successful.
 */
bool CMsgGetCurrParamReply::initWithRawData(unsigned char * data) {
    //unsigned char result [2]; /*Result code - 0x30,0x30(no error) / 0x30,0x31(Unsupported operation) */
    //unsigned char opCodePage [2]; /*Operation code page*/
    //unsigned char opCode [2]; /*Operation code*/
    //unsigned char type [2]; /*Operation type code - 0x30,0x30(set parameter) / 0x30,0x31(momentary)*/
    //unsigned char maxValue[4]; /*Maximum value which monitor can accept.*/
    //unsigned char currValue[4];
    if (data[1] == 0x30) m_result[0] = data[1];
    else throw "InitWithRawData(): result exception\n";
    if (data[2] == 0x30 || data[2] == 0x31) m_result[1] = data[2];
    else throw "InitWithRawData(): result exception\n";
    
    if ( !(data[3] == m_opCodePage[0]) ) throw "InitWithRawData(): opCodePage exception\n";
    if ( !(data[4] == m_opCodePage[1]) ) throw "InitWithRawData(): opCodePage exception\n";
    
    if ( !(data[5] == m_opCode[0]) ) throw "InitWithRawData(): opCode exception\n";
    if ( !(data[6] == m_opCode[1]) ) throw "InitWithRawData(): opCode exception\n";

    if (data[7] == 0x30) m_type[0] = data[7];
    else throw "InitWithRawData(): type exception\n";
    if (data[8] == 0x30 || data[8] == 0x31) m_type[1] = data[8];
    else throw "InitWithRawData(): type exception\n";
    m_maxValue[0] = data[9];
    m_maxValue[1] = data[10];
    m_maxValue[2] = data[11];
    m_maxValue[3] = data[12];
    m_currValue[0] = data[13];
    m_currValue[1] = data[14];
    m_currValue[2] = data[15];
    m_currValue[3] = data[16];
    return true;
}
//******************************************************************************

CMsgSetParamReply::CMsgSetParamReply(const unsigned char * _buffer) {
    //unsigned char result [2]; /*Result code - 0x30,0x30(no error) / 0x30,0x31(Unsupported operation) */
    //unsigned char opCodePage [2]; /*Operation code page*/
    //unsigned char opCode [2]; /*Operation code*/
    //unsigned char type [2]; /*Operation type code - 0x30,0x30(set parameter) / 0x30,0x31(momentary)*/
    //unsigned char maxValue[4]; /*Maximum value that monitor can accept.*/
    //unsigned char reqSettingVal[4]; /*Echoes back the parameter for confirmation.*/
}

CMsgSetParamReply::CMsgSetParamReply(const CMsgSetParamReply & rhs)
: CAbstractMessage(rhs) {
    //TODO
}

CMsgSetParamReply::~CMsgSetParamReply() {
}

CAbstractMessage* CMsgSetParamReply::clone() const {
    return new CMsgSetParamReply(*this);
}

unsigned char CMsgSetParamReply::getCheckCode() const {
}

int CMsgSetParamReply::getLength(unsigned char & hi, unsigned char & lo) const {
    hi = '1';
    lo = '2';
    return 18;
}

std::basic_string<unsigned char> CMsgSetParamReply::getLength() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back('1');
    ustr.push_back('2');
    return ustr;
}

std::basic_string<unsigned char> CMsgSetParamReply::getBuffer() const {

}

bool CMsgSetParamReply::initWithRawData(unsigned char * data) {

    return false;
}
//******************************************************************************

CMsgCommSaveCurrSettingsReply::CMsgCommSaveCurrSettingsReply(const unsigned char * _buffer) {
    //unsigned char m_commandCode [4]; /* '0','0', '0', 'C' (0x30, 0x30, 0x30, 0x43)*/
    //    if ()
}

CAbstractMessage* CMsgCommSaveCurrSettingsReply::clone() const {

}

unsigned char CMsgCommSaveCurrSettingsReply::getCheckCode() const {
}

int CMsgCommSaveCurrSettingsReply::getLength(unsigned char & hi, unsigned char & lo) const {
    hi = '0';
    lo = '6';
    return 6;
}

std::basic_string<unsigned char> CMsgCommSaveCurrSettingsReply::getLength() const {
    std::basic_string<unsigned char> ustr;
    ustr.push_back('0');
    ustr.push_back('6');
    return ustr;
}

std::basic_string<unsigned char> CMsgCommSaveCurrSettingsReply::getBuffer() const {

}

bool CMsgCommSaveCurrSettingsReply::initWithRawData(unsigned char * data) {

    return false;
}
//******************************************************************************

CMsgCommNull::CMsgCommNull() {
    //unsigned char commandCode [2]; /* 'B','E' (0x42, 0x45)*/
    m_commandCode[0] = 0x42;
    m_commandCode[1] = 0x45;
}

CMsgCommNull::CMsgCommNull(const CMsgCommNull& rhs)
: CAbstractMessage(rhs) {
    //TODO
}

CMsgCommNull::~CMsgCommNull() {
}

CAbstractMessage* CMsgCommNull::clone() const {
    return new CMsgCommNull(*this);
}

unsigned char CMsgCommNull::getCheckCode() const {
}

int CMsgCommNull::getLength(unsigned char & hi, unsigned char & lo) const {
    hi = '0';
    lo = '4';
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

bool CMsgCommNull::initWithRawData(unsigned char * data) {

    return false;
}

//******************************************************************************