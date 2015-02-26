#ifndef __CCONNECTION_H__
#define	__CCONNECTION_H__

#include "CPacket.h"

class CConnection {
public:
    CConnection(const char * srvAddr, const int & port);
    ~CConnection();
    bool establishConnection();
    void disconnect();
    bool isConnected();
    int sendPacket(const CPacket & packet);
    bool receivePacket(const CPacket & expected);
private:
    const char * m_serverAddr;
    const int m_port;
    int m_socketFD;
    struct addrinfo * m_addrInfo;
};


#endif	/* __CCONNECTION_H__ */

