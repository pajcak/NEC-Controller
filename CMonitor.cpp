//    File: CMonitor.cpp
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

#include "headers/CMonitor.h"
#include "headers/MsgIncoming.h"
#include "headers/MsgOutgoing.h"
#include "headers/IReceivable.h"

#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

//#define DEBUG

CMonitor::CMonitor(const char * _srvAddr, const int & _port)
: m_serverAddr(_srvAddr), m_port(_port)
{
    pthread_mutex_init(&m_mutex, NULL);
}

CMonitor::~CMonitor()
{
//    pthread_mutex_destroy(&m_mutex);
//    if (m_addrInfo != NULL) freeaddrinfo(m_addrInfo);
//    close(m_socketFD);
}

bool CMonitor::establishConnection() {
    char portStr[10];

    snprintf(portStr, sizeof (portStr), "%d", m_port);
    if (getaddrinfo(m_serverAddr, portStr, NULL, &m_addrInfo)) {
        throw "CMonitor::establishConnection(): Getaddrinfo() failed...\n";
        return false;
    }
    m_socketFD = socket(m_addrInfo -> ai_family, SOCK_STREAM, 0);
    if (m_socketFD == -1) {
        //        freeaddrinfo(ai); -->destructor called even though exception is thrown
        throw "CMonitor::establishConnection(): Socket() failed...\n";
        return false;
    }
    if (connect(m_socketFD, m_addrInfo -> ai_addr, m_addrInfo -> ai_addrlen) == -1) {
        close(m_socketFD);
        //        freeaddrinfo(m_addrInfo); -->destructor called even though exception is thrown
        //        close(m_socketFD); -->destructor called even though exception is thrown
        throw "CMonitor::establishConnection(): Connect() failed...\n";
        return false;
    }
    freeaddrinfo(m_addrInfo);
    /*Connected to the server*/
    return true;
}

void CMonitor::disconnect() {
    close(m_socketFD);
}

bool CMonitor::isConnected() const {
    int error_code;
    socklen_t error_code_size = sizeof(error_code);
    getsockopt(m_socketFD, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);
    
    return (error_code == 0);
}
void CMonitor::sendPacket(  const unsigned char & DESTINATION,
                            const unsigned char & MSG_TYPE,
                            const CAbstractMessage * msg)
{
    /*HEADER|MESSAGE|CHECK_CODE|DELIMITER*/
    const unsigned char DELIMITER = 0x0D;
    CHeader header(DESTINATION, MSG_TYPE, msg->getLength().c_str());
    const unsigned char CHECK_CODE = header.getCheckCode() ^ msg->getCheckCode();

    std::basic_string<unsigned char> outBuffer;
    outBuffer.append(header.getBuffer());
    outBuffer.append(msg->getBuffer());
    outBuffer.push_back(CHECK_CODE);
    outBuffer.push_back(DELIMITER);

#ifdef DEBUG/*-------------------------------------------------------------*/
    fprintf(stderr, "===============SENDING================\n");
    fprintf(stderr, "Destination : [0x%02x] | [%c]\n", DESTINATION, DESTINATION);
    fprintf(stderr, "Message type: [0x%02x] | [%c]\n", MSG_TYPE, MSG_TYPE);
    std::basic_string<unsigned char> hBuff = header.getBuffer();
    for (unsigned int i = 0; i < hBuff.length(); i++) {
        fprintf(stderr, "Head[%d]: [0x%02x] | [%c]\n", i, hBuff[i], hBuff[i]);
    }
    fprintf(stderr, "\n");
    std::basic_string<unsigned char> mBuff = msg->getBuffer();
    for (unsigned int i = 0; i < mBuff.length(); i++) {
        fprintf(stderr, "Message[%d]: [0x%02x] | [%c]\n", i, mBuff.at(i), mBuff.at(i));
    }
    fprintf(stderr, "\n");
    fprintf(stderr, "CheckCode: [0x%02x] | [%c]\n", CHECK_CODE, CHECK_CODE);
    fprintf(stderr, "Delimiter: [0x%02x] | [%c]\n", DELIMITER, DELIMITER);
#endif /*-----------------------------------------------------------------*/
    int writtenLen = write(m_socketFD, outBuffer.c_str(), outBuffer.length());
    if (writtenLen < (header.getLen() + msg->getLengthInt() + 2))
        throw "sendPacket(): written length too small.";

}
void CMonitor::receiveData(CHeader * header, CAbstractMessage * msg) {
//-----------------HEADER---------------
    unsigned char headBuffer [7];
    int readLength = read(m_socketFD, headBuffer, sizeof (headBuffer));
    if (readLength < 7) throw "receiveData(): header readLength too small.";
    
    
    header->initWithRawData(headBuffer);
    //received message is NULLMessage 
    if (header->getMsgLenInt() == CMsgCommNull().getLengthInt())
        throw "saveCurrentSettings(): NULL MESSAGE.";
//-----------------MESSAGE---------------
    const int MSG_LEN = header->getMsgLenInt();
    unsigned char msgBuffer [MSG_LEN];
    readLength = read(m_socketFD, msgBuffer, MSG_LEN);    

    if (readLength < MSG_LEN)
        throw "receiveData(): msg readLength too small.";
    
    msg->initWithRawData(msgBuffer);

    unsigned char checkCode;
    unsigned char delimiter;
    read(m_socketFD, &checkCode, 1);
//    if (checkCode != (header->getCheckCode() ^ msg->getCheckCode()))
//        throw "receiveData(): incorrect checkCode.";
    read(m_socketFD, &delimiter, 1);
//    if (delimiter != 0x0D)
//        throw "receiveData(): incorrect delimiter.";

#ifdef DEBUG/*-------------------------------------------------------------*/
    printf("==============RECEIVED===============\n");
    
    std::basic_string<unsigned char> hBuff = header->getBuffer();
    for (unsigned int i = 0; i < hBuff.length(); i++) {
        printf("Head[%d]: [0x%02x] | [%c]\n", i, hBuff[i], hBuff[i]);
    }
    printf("\n");
    std::basic_string<unsigned char> mBuff = msg->getBuffer();
    for (unsigned int i = 0; i < mBuff.length(); i++) {
        printf("Message[%d]: [0x%02x] | [%c]\n", i, mBuff.at(i), mBuff.at(i));
    }
    printf("\n");
    printf("CheckCode: [0x%02x] | [%c]\n", checkCode, checkCode);
    printf("Delimiter: [0x%02x] | [%c]\n", delimiter, delimiter);
#endif /*-----------------------------------------------------------------*/
    
}

CAbstractMessage * CMonitor::getParameter(const CAbstractMessage * msg) {
    //'A' = monitor with ID = 1
    //'C' = "Get current parameter" type of message
    sendPacket('A', 'C', msg);
       //================RECEIVING==========================
    CHeader header;
    CMsgGetCurrParamReply * recvMessage = new CMsgGetCurrParamReply();
    try {
        receiveData(&header, recvMessage);
    } catch (...) {
        delete recvMessage;
        throw;
    }
    return recvMessage;
}

CAbstractMessage * CMonitor::setParameter(const CAbstractMessage* msg) {
    //monitor with ID = 1
    //"Set parameter" type of message
    sendPacket('A', 'E', msg);
       //================RECEIVING==========================
    CHeader header;
    CAbstractMessage* recvMessage = new CMsgSetParamReply ();
    try {
        receiveData(&header, recvMessage);
    } catch (...) {
        delete recvMessage;
        throw;
    }
    return recvMessage;
}

void CMonitor::saveCurrentSettings(void) {
    CAbstractMessage * msg = new CMsgCommSaveCurrSettings();
    sendPacket('A', 'A', msg);
    //================RECEIVING==========================
    CHeader header;
    CMsgCommSaveCurrSettingsReply recvMessage;
    try {
        receiveData(&header, &recvMessage);
    } catch (...) {
        delete msg;
        throw;
    }
    
    delete msg;
}
int      CMonitor::powerStatusRead(void) {
    CAbstractMessage * msg = new CMsgCommPowerStatusRead ();
    sendPacket('A', 'A', msg);
    //================RECEIVING==========================
    CHeader header;
    CMsgCommPowerStatusReadReply recvMessage;
    try {
        receiveData(&header, &recvMessage);
    } catch (...) {
        delete msg;
        throw;
    }

    if (recvMessage.getMaxModeCount() != 4) 
        throw "CMonitor::powerStatusRead(): invalid number of max modes.";
    
    delete msg;
    return recvMessage.getCurrMode();
}
int CMonitor::powerControl(const unsigned char & powerMode) {
    CAbstractMessage * msg = new CMsgCommPowerControl (powerMode);
    sendPacket('A', 'A', msg);
    //================RECEIVING==========================
    CHeader header;
    CMsgCommPowerControlReply recvMessage;
    try {
        receiveData(&header, &recvMessage);
    } catch (...) {
        delete msg;
        throw;
    }

    delete msg;
    return recvMessage.getCurrMode();
}

/*MESSAGE TYPE(msgType)*/
//ASCII 'A' (0x41): Command.
//ASCII 'B' (0x42): Command reply.
//ASCII 'C' (0x43): Get current parameter from a monitor.
//ASCII 'D' (0x44): "Get parameter" reply.
//ASCII 'E' (0x45): Set parameter.
//ASCII 'F' (0x46): "Set parameter" reply.
