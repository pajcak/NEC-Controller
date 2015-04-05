#include "headers/CMonitor.h"
#include "headers/MsgIncoming.h"
#include "headers/MsgOutgoing.h"
#include "headers/IReceivable.h"
//#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

CMonitor::CMonitor(const char * _srvAddr, const int & _port)
: m_serverAddr(_srvAddr), m_port(_port) {

}

CMonitor::~CMonitor() {
    freeaddrinfo(m_addrInfo);
    close(m_socketFD);
}

bool CMonitor::establishConnection() {
    struct addrinfo * ai;
    char portStr[10];

    snprintf(portStr, sizeof ( portStr), "%d", m_port);
    if (getaddrinfo(m_serverAddr, portStr, NULL, &ai)) {
        throw "Getaddrinfo() failed...\n";
        return false;
    }
    m_socketFD = socket(ai -> ai_family, SOCK_STREAM, 0);
    if (m_socketFD == -1) {
        //        freeaddrinfo(ai); -->destructor called even though exception is thrown
        throw "Socket() failed...\n";
        return false;
    }
    if (connect(m_socketFD, ai -> ai_addr, ai -> ai_addrlen) == -1) {
        close(m_socketFD);
        //        freeaddrinfo(m_addrInfo); -->destructor called even though exception is thrown
        //        close(m_socketFD); -->destructor called even though exception is thrown
        throw "Connect() failed...\n";
        return false;
    }
    /*Connected to the server*/
    return true;
}

void CMonitor::disconnect() {
    close(m_socketFD);
}

bool CMonitor::isConnected() {
    if (getpeername(m_socketFD, m_addrInfo->ai_addr,
            &m_addrInfo->ai_addrlen) == -1) return false;
    return true;
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

    int writtenLen = write(m_socketFD, outBuffer.c_str(), strlen((const char *) outBuffer.c_str()));
    if (writtenLen < (header.getLen() + msg->getLengthInt() + 2))
        throw "sendPacket(): written length too small.";

}
void CMonitor::receiveData(CHeader * header, CAbstractMessage * msg) {
//-----------------HEADER---------------
    printf("==============RECEIVING==============\n");
    unsigned char headBuffer [7];
    int readLength = read(m_socketFD, headBuffer, sizeof (headBuffer));
    if (readLength < 7) throw "receiveData(): header readLength too small.";
    
    for (int i = 0; i < readLength; i++) {
        printf("Head[%d]: [0x%02x] | [%c]\n", i, headBuffer[i], headBuffer[i]);
    }
    printf("\n");
    
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

    std::basic_string<unsigned char> x = msg->getBuffer();
    for (int i = 0; i < msg->getLengthInt(); i++) {
        printf("Message[%d]: [0x%02x] | [%c]\n", i, x.at(i), x.at(i));
    }
    printf("\n");
    
    unsigned char checkCode;
    unsigned char delimiter;
    read(m_socketFD, &checkCode, 1);
//    if (checkCode != (header->getCheckCode() ^ msg->getCheckCode()))
//        throw "receiveData(): incorrect checkCode.";
    read(m_socketFD, &delimiter, 1);
//    if (delimiter != 0x0D)
//        throw "receiveData(): incorrect delimiter.";
    printf("CheckCode: [0x%02x] | [%c]\n", checkCode, checkCode);
    printf("Delimiter: [0x%02x] | [%c]\n", delimiter, delimiter);
    printf("==============RECEIVED===============\n");
}

CAbstractMessage * CMonitor::getParameter(const CAbstractMessage * msg) {
    //'A' = monitor with ID = 1
    //'C' = "Get current parameter" type of message
    sendPacket('A', 'C', msg);
       //================RECEIVING==========================
    CHeader header;
    CMsgGetCurrParamReply * recvMessage = new CMsgGetCurrParamReply();
    receiveData(&header, recvMessage);
    return recvMessage;
}

CAbstractMessage * CMonitor::setParameter(const CAbstractMessage* msg) {
    //monitor with ID = 1
    //"Set parameter" type of message
    sendPacket('A', 'E', msg);
       //================RECEIVING==========================
    CHeader header;
    CMsgSetParamReply * recvMessage = new CMsgSetParamReply();
    receiveData(&header, recvMessage);
    return recvMessage;
}

void CMonitor::saveCurrentSettings(void) {
    CAbstractMessage * msg = new CMsgCommSaveCurrSettings();
    sendPacket('A', 'A', msg);
    //================RECEIVING==========================
    CHeader header;
    CMsgCommSaveCurrSettingsReply recvMessage;
    receiveData(&header, &recvMessage);    
    
    delete msg;
}
int      CMonitor::powerStatusRead(void) {
    CAbstractMessage * msg = new CMsgCommPowerStatusRead ();
    sendPacket('A', 'A', msg);
    //================RECEIVING==========================
    CHeader header;
    CMsgCommPowerStatusReadReply recvMessage;
    receiveData(&header, &recvMessage);    
    
    if (recvMessage.getMaxModeCount() != 4) throw "powerStatusRead(): invalid number of max modes.";
    
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
