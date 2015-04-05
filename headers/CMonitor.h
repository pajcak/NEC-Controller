#ifndef __CCONNECTION_H__
#define	__CCONNECTION_H__

#include "CPacket.h"
#include "MsgIncoming.h"

class CMonitor {
public:
    CMonitor(const char * srvAddr, const int & port);
    ~CMonitor();
    bool establishConnection();
    void disconnect();
    bool isConnected();

/** \Brief Blocking, sends CMsgGetCurrParam and returns received data in CMsgGetCurrParamReply.
 * If NullMsg was received, simply return sNULL, if error, throw exception
 * \return CMsgGetCurrParamReply if correct data were received.
 * If incorrect data arrived, exception is thrown.
 * If nullMsg was received, exception is thrown.
 */
CAbstractMessage * getParameter(const CAbstractMessage * msg);

/** \Brief Blocking, sends CMsgSetParam and returns received data in CMsgSetParamReply.
 * If NullMsg was received, simply return sNULL, if error, throw exception
 * \return CMsgSetParamReply if correct data were received.
 * If incorrect data arrived, exception is thrown.
 * If nullMsg was received, it exception is thrown.
 */
CAbstractMessage *      setParameter(const CAbstractMessage * param);
/*now methods for each command*/

void                    saveCurrentSettings(void);
CAbstractMessage *      powerStatusRead(void);

private:
    void sendPacket(const unsigned char & DESTINATION,
                    const unsigned char & MSG_TYPE,
                    const CAbstractMessage * msg);
    void receiveData(CHeader * header, CAbstractMessage * msg);
    
    const char * m_serverAddr;
    const int m_port;
    int m_socketFD;
    struct addrinfo * m_addrInfo;
};


#endif	/* __CCONNECTION_H__ */

