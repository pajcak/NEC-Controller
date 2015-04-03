#include "headers/CController.h"
#include "headers/CPacket.h"
#include "headers/MsgOutgoing.h"
#include "headers/CMonitor.h"
#include "headers/MsgIncoming.h"

#include <cstdio>
CController::CController() {
    //temporary
    m_monitor = new CMonitor("localhost", 12345);
    m_monitor->establishConnection();
}

CController::~CController() {
    m_monitor->disconnect();
    delete m_monitor;
}

int  CController::getBrightness() {
    unsigned char opCodePage[2] = {'0', '0'};/*operation code page for Brightness*/
    unsigned char opCode[2]     = {'1', '0'};/*operation code for Brightness*/
    /*Does not need to be deleted (deleted inside packet destructor)*/
//    CAbstractMessage * msg = new CMsgGetCurrParam(opCodePage, opCode); 
    // '*' means destination address of all monitors connected by "daisy chain"
    // 'C' means Get parameter message type
    CMsgGetCurrParam msg(opCodePage, opCode);
    
    CPacket packet(CHeader('*', 'C', msg.getLength().c_str()), msg);
    m_monitor->sendPacket(packet);
    printf("***SENT****\n");
    
    CMsgGetCurrParamReply expectedMsg(opCodePage, opCode);
    CPacket expected(CHeader(), expectedMsg);
    m_monitor->receivePacket(expected);
}
bool CController::setBrightness(const int & val) {
    
}
