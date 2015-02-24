#include "CController.h"
#include "CPacket.h"
#include "MsgOutgoing.h"

CController::CController() {
}

CController::~CController() {
}

int  CController::getBrightness() {
    unsigned char opCodePage[2] = {'0', '0'};/*operation code page for Brightness*/
    unsigned char opCode[2]     = {'1', '0'};/*operation code for Brightness*/
    /*Does not need to be deleted (deleted inside packet destructor)*/
    CAbstractMessage * msg = new CMsgGetCurrParam(opCodePage, opCode); 
    // '*' menas destination address of all monitors connected by "daisy chain"
    // 'C' means Get parameter message type
    CPacket packet(new CHeader('*', 'C', msg->getLength().c_str()), msg);
    
    //    buffer = a.getbuffer;
//    write(buffer);
//    read(buffer)
//    pares(buffer);
//    return val
}
bool CController::setBrightness(const int & val) {
    
}
