#include "headers/CMonitor.h"   
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

/**
 * \brief Method sends byte buffer from packet to socket file descriptor.
 * \param packet CPacket to be sent.
 * \return -1 fail\n
 * 0 nothing was written\n
 * >0 number of bytes written
 */
int CMonitor::sendPacket(const CPacket & packet) {
    const unsigned char * buffer = packet.getBuffer().c_str();
    return write(m_socketFD, buffer, strlen((const char *) buffer));
}

bool CMonitor::receivePacket(CPacket & expected) {
    unsigned char buffer [256];

    /* returns -1 if error occured
     * returns the number of bytes read
     **/
    int length = read(m_socketFD, buffer, sizeof (buffer));
    if (length < 0) {
        throw "Read() failed...\n";
        return false;
    }
    /*vyzobnout nejdriv prvnich par bytu z bufferu (tedy header) a z nej zjistit
     *delku message, a pak az kontrolovat, jestli je prijatej buffer spravne dlouhej
     * a naparsovat ho hezky (asi tim, ze celej buffer predam do urcite message (podle typu v headeru))
     **/
    /*PICU (to nahore) , radeji tady nechat prijeti packetu, nacpat data do expected
     a expected.msg a tam si to samo rozparsuje a zjisti, zdali cajk, nebo ne*/
    
    // !!!check if length is at least equal to requested packet length
    for (int i = 0; i < length; i++) {
        printf("rec: 0x%02x | %c\n", buffer[i], buffer[i]);
    }
    printf("\n");

//DEBUG print
//    write(STDOUT_FILENO, buffer, length);
    return true;
}

