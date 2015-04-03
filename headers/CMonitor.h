#ifndef __CCONNECTION_H__
#define	__CCONNECTION_H__

#include "CPacket.h"

class CMonitor {
public:
    CMonitor(const char * srvAddr, const int & port);
    ~CMonitor();
    bool establishConnection();
    void disconnect();
    bool isConnected();
    int sendPacket(const CPacket & packet);
    CPacket * receivePacket(CPacket & expected);
private:
    const char * m_serverAddr;
    const int m_port;
    int m_socketFD;
    struct addrinfo * m_addrInfo;
};


#endif	/* __CCONNECTION_H__ */

