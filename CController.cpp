#include "headers/CController.h"
#include "headers/CPacket.h"
#include "headers/MsgOutgoing.h"

#include <cstdio>
CController::CController() {
}

CController::~CController() {
}

int  CController::getBrightness() {
    unsigned char opCodePage[2] = {'0', '0'};/*operation code page for Brightness*/
    unsigned char opCode[2]     = {'1', '0'};/*operation code for Brightness*/
    /*Does not need to be deleted (deleted inside packet destructor)*/
//    CAbstractMessage * msg = new CMsgGetCurrParam(opCodePage, opCode); 
    // '*' menas destination address of all monitors connected by "daisy chain"
    // 'C' means Get parameter message type
    CMsgGetCurrParam msg(opCodePage, opCode);
    
    CPacket packet(CHeader('*', 'C', msg.getLength().c_str()), msg);
    std::basic_string<unsigned char> tmp;
    tmp = packet.getBuffer(); 
    for (int i = 0; i < tmp.length(); i++) {
        printf("(0x%02x | %c)\n", tmp[i], tmp[i]);
    }

    //    buffer = a.getbuffer;
//    write(buffer);
//    read(buffer)
//    pares(buffer);
//    return val
}
bool CController::setBrightness(const int & val) {
    
}
