#include "MsgIncoming.h"

CMsgGetCurrParamReply::CMsgGetCurrParamReply(const char * _buffer) {
    //unsigned char result [2]; /*Result code - 0x30,0x30(no error) / 0x30,0x31(Unsupported operation) */
    //unsigned char opCodePage [2]; /*Operation code page*/
    //unsigned char opCode [2]; /*Operation code*/
    //unsigned char type [2]; /*Operation type code - 0x30,0x30(set parameter) / 0x30,0x31(momentary)*/
    //unsigned char maxValue[4]; /*Maximum value which monitor can accept.*/
    //unsigned char currValue[4];
}

CMsgGetCurrParamReply::~CMsgGetCurrParamReply() {
}

unsigned char CMsgGetCurrParamReply::getCheckCode() const {
}

int CMsgGetCurrParamReply::getLength(unsigned char & hi, unsigned char & lo) {
    hi = '1';
    lo = '2';
    return 18;
}
std::basic_string<unsigned char> CMsgGetCurrParamReply::getLength() {
    std::basic_string<unsigned char> ustr;
    ustr.push_back('1');
    ustr.push_back('2');
    return ustr;
}

std::basic_string<unsigned char> CMsgGetCurrParamReply::getBuffer() { // MAYBE NOT NEEDED
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
//******************************************************************************

CMsgSetParamReply::CMsgSetParamReply(const char * _buffer) {
    //unsigned char result [2]; /*Result code - 0x30,0x30(no error) / 0x30,0x31(Unsupported operation) */
    //unsigned char opCodePage [2]; /*Operation code page*/
    //unsigned char opCode [2]; /*Operation code*/
    //unsigned char type [2]; /*Operation type code - 0x30,0x30(set parameter) / 0x30,0x31(momentary)*/
    //unsigned char maxValue[4]; /*Maximum value that monitor can accept.*/
    //unsigned char reqSettingVal[4]; /*Echoes back the parameter for confirmation.*/
}

CMsgSetParamReply::~CMsgSetParamReply() {
}

unsigned char CMsgSetParamReply::getCheckCode() const {
}

int CMsgSetParamReply::getLength(unsigned char & hi, unsigned char & lo) {
    hi = '1';
    lo = '2';
    return 18;
}
std::basic_string<unsigned char> CMsgSetParamReply::getLength() {
    std::basic_string<unsigned char> ustr;
    ustr.push_back('1');
    ustr.push_back('2');
    return ustr;
}

std::basic_string<unsigned char> CMsgSetParamReply::getBuffer() {// MAYBE NOT NEEDED
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
//******************************************************************************

CMsgCommGetTimingReply::CMsgCommGetTimingReply(const char * _buffer) {
    //**unsigned char commandCode [2]; /* '4','E' (0x34, 0x45)*/
    //**unsigned char SS [2]; /*Timing status byte*/
    //    Bit 7 = 1: Sync Frequency is out of range.
    //    Bit 6 = 1: Unstable count
    //    Bit 5-2 Reserved (Don't care)
    //    Bit 1
    //            1:Positive Horizontal sync polarity.
    //            0:Negative Horizontal sync polarity.
    //    Bit 0
    //            1:Positive Vertical sync polarity.
    //            0:Negative Vertical sync polarity.
    //**unsigned char Hfreq[4]; /*Horizontal Frequency in unit 0.01kHz*/
    //**unsigned char Vfreq[4]; /*Vertical Frequency in unit 0.01Hz*/
    //    When H Freq is '1''2''A''9' (31h, 32h, 41h, 39h), it means 47.77kHz.
}

CMsgCommGetTimingReply::~CMsgCommGetTimingReply() {
}

unsigned char CMsgCommGetTimingReply::getCheckCode() const {
}

int CMsgCommGetTimingReply::getLength(unsigned char & hi, unsigned char & lo) {
    hi = '0';
    lo = 'E';
    return 14;
}
std::basic_string<unsigned char> CMsgCommGetTimingReply::getLength() {
    std::basic_string<unsigned char> ustr;
    ustr.push_back('0');
    ustr.push_back('E');
    return ustr;
}

std::basic_string<unsigned char> CMsgCommGetTimingReply::getBuffer() { // MAYBE NOT NEEDED
    std::basic_string<unsigned char> ustr;
    ustr.push_back(s_STX);
    ustr.push_back(m_commandCode[0]);
    ustr.push_back(m_commandCode[1]);
    ustr.push_back(m_SS[0]);
    ustr.push_back(m_SS[1]);
    ustr.push_back(m_Hfreq[0]);
    ustr.push_back(m_Hfreq[1]);
    ustr.push_back(m_Hfreq[2]);
    ustr.push_back(m_Hfreq[3]);
    ustr.push_back(m_Vfreq[0]);
    ustr.push_back(m_Vfreq[1]);
    ustr.push_back(m_Vfreq[2]);
    ustr.push_back(m_Vfreq[3]);
    ustr.push_back(s_ETX);
    return ustr;    
}
//******************************************************************************

CMsgCommNull::CMsgCommNull() {
    //unsigned char commandCode [2]; /* 'B','E' (0x42, 0x45)*/
}

CMsgCommNull::~CMsgCommNull() {
}

unsigned char CMsgCommNull::getCheckCode() const {
}

int CMsgCommNull::getLength(unsigned char & hi, unsigned char & lo) {
    hi = '0';
    lo = '4';
    return 4;
}
std::basic_string<unsigned char> CMsgCommNull::getLength() {
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
//******************************************************************************

std::basic_string<unsigned char> CMsgCommNull::getBuffer() { // MAYBE NOT NEEDED
    std::basic_string<unsigned char> ustr;
    ustr.push_back(s_STX);
    ustr.push_back(m_commandCode[0]);
    ustr.push_back(m_commandCode[1]);
    ustr.push_back(s_ETX);
    return ustr;
}
