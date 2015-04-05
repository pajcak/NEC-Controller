#ifndef __MSGCONTROLLER_H__
#define __MSGCONTROLLER_H__
/*TIPS: Distribute message classes for receive to other header file*/
#include "CAbstractMessage.h"

//******************************************************************************
class CMsgGetCurrParam : public CAbstractMessage {
public:
                                     CMsgGetCurrParam(unsigned char _opCodePage[2], unsigned char _opCode[2]);
                                     CMsgGetCurrParam(const CMsgGetCurrParam&);
                                     ~CMsgGetCurrParam();
     CAbstractMessage*               clone() const;
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
    
                                     CMsgSetParam(unsigned char _opCodePage[2], unsigned char _opCode[2],
                                                 unsigned char _setValue[4]);
                                     CMsgSetParam(const CMsgSetParam&);
                                     ~CMsgSetParam();
     CAbstractMessage*               clone() const;
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
     CAbstractMessage*               clone() const;
    unsigned char                    getCheckCode() const;
    int                              getLengthInt() const;
    std::basic_string<unsigned char> getLength() const;
    std::basic_string<unsigned char> getBuffer() const;
    //DOES NOT NEED TO BE IMPLEMENTED
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
     CAbstractMessage*               clone() const;
    unsigned char                    getCheckCode() const;
    int                              getLengthInt() const;
    std::basic_string<unsigned char> getLength() const;
    std::basic_string<unsigned char> getBuffer() const;
    //DOES NOT NEED TO BE IMPLEMENTED
private:
    unsigned char m_commandCode [4]; /* 'O', '1', 'D', '6' */
};

#endif /* __MSGCONTROLLER_H__ */