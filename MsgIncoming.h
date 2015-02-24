#ifndef __MSGINCOMING_H__
#define	__MSGINCOMING_H__

#include "CAbstractMessage.h"

class CMsgGetCurrParamReply : public CAbstractMessage {
public:
                    CMsgGetCurrParamReply(const char * _buffer);
                    ~CMsgGetCurrParamReply();
    unsigned char   getCheckCode() const;
    int            getLength(unsigned char & hi, unsigned char & lo);
    // saves all bytes to _outBuffer and returns the number of bytes saved
    std::basic_string<unsigned char> getBuffer(); // MAYBE NOT NEEDED
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
                    CMsgSetParamReply(const char * _buffer);
                    ~CMsgSetParamReply();
    unsigned char   getCheckCode() const;
    int            getLength(unsigned char & hi, unsigned char & lo);
    // saves all bytes to _outBuffer and returns the number of bytes saved
    std::basic_string<unsigned char> getBuffer(); // MAYBE NOT NEEDED
private:
    unsigned char m_result [2]; /*Result code - 0x30,0x30(no error) / 0x30,0x31(Unsupported operation) */
    unsigned char m_opCodePage [2]; /*Operation code page*/
    unsigned char m_opCode [2]; /*Operation code*/
    unsigned char m_type [2]; /*Operation type code - 0x30,0x30(set parameter) / 0x30,0x31(momentary)*/
    unsigned char m_maxValue[4]; /*Maximum value that monitor can accept.*/
    unsigned char m_reqSettingVal[4]; /*Echoes back the parameter for confirmation.*/
};
//*******************************************************************************
/*-------------------COMMAND-MESSAGES-----------------------------------------*/
//*******************************************************************************
class CMsgCommGetTimingReply : public CAbstractMessage {
public:
                    CMsgCommGetTimingReply(const char * _buffer);
                    ~CMsgCommGetTimingReply();
    unsigned char   getCheckCode() const;
    int            getLength(unsigned char & hi, unsigned char & lo);
    // saves all bytes to _outBuffer and returns the number of bytes saved
    std::basic_string<unsigned char> getBuffer(); // MAYBE NOT NEEDED
private:
    unsigned char m_commandCode [2]; /* '4','E' (0x34, 0x45)*/
    unsigned char m_SS [2]; /*Timing status byte*/
//    Bit 7 = 1: Sync Frequency is out of range.
//    Bit 6 = 1: Unstable count
//    Bit 5-2 Reserved (Don't care)
//    Bit 1
//            1:Positive Horizontal sync polarity.
//            0:Negative Horizontal sync polarity.
//    Bit 0
//            1:Positive Vertical sync polarity.
//            0:Negative Vertical sync polarity.
    unsigned char m_Hfreq[4]; /*Horizontal Frequency in unit 0.01kHz*/
    unsigned char m_Vfreq[4]; /*Vertical Frequency in unit 0.01Hz*/
//    When H Freq is '1''2''A''9' (31h, 32h, 41h, 39h), it means 47.77kHz.
};
//******************************************************************************
class CMsgCommNull : public CAbstractMessage {
public:
                    CMsgCommNull();
                    ~CMsgCommNull();
    unsigned char   getCheckCode() const;
    int            getLength(unsigned char & hi, unsigned char & lo);
    // saves all bytes to _outBuffer and returns the number of bytes saved
    std::basic_string<unsigned char> getBuffer(); // MAYBE NOT NEEDED
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

