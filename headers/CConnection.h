#ifndef __CCONNECTION_H__
#define	__CCONNECTION_H__

class CConnection {
public:
    CConnection(const char * srvAddr, const int & port);
    bool connect();
    void disconnect();
    bool isConnected();
    void sendPacket(const CPacket & packet);
    CPacket receivePaket();
private:
    const char * serverAddr;
    const int port;
    int socket;
};


#endif	/* __CCONNECTION_H__ */

