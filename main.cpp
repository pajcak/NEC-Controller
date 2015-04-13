#include <string>

#include "headers/CController.h"
#include <iostream>
#include <vector>
#include <pthread.h>
using namespace std;

vector <int> g_monitorIDs;

pthread_mutex_t g_printMtx;

void * delFunc (CController * c) {
    pthread_mutex_lock(&g_printMtx);
    printf("DELETEFunc\n");
    pthread_mutex_unlock(&g_printMtx);
    c->deleteMonitor(1);
    return NULL;
}
void * thrFunc (CController * c) {
    printf("threadFunc\n");
//    int res = 0;
    try {
        c->setBrightness(1, 80);
//        res = c->setBrightness(1);
    } catch (char const * s) {
        pthread_mutex_lock(&g_printMtx);
        printf("CAUGHT: %s\n", s);
        pthread_mutex_unlock(&g_printMtx);
        return NULL;
    }
//    pthread_mutex_lock(&g_printMtx);
//    printf("Get: %d\n", res);
//    pthread_mutex_unlock(&g_printMtx);
    return NULL;
}

void multiAccess(int thrCnt, CController * c) {
    pthread_t * threads = new pthread_t[thrCnt];
    pthread_mutex_init(&g_printMtx, NULL);
    pthread_attr_t threadAttr;
    pthread_attr_init(&threadAttr);
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);

    pthread_t delThr;
    for (int i = 0; i < thrCnt; i++) {
        pthread_create(&threads[i], NULL, (void*(*)(void*)) thrFunc, c);
    }
    pthread_create(&delThr, NULL, (void*(*)(void*)) delFunc, c);
    
    
    for (int i = 0; i < thrCnt; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_join(delThr, NULL);
    
    
    pthread_mutex_destroy(&g_printMtx);
    pthread_attr_destroy(&threadAttr);  
    delete [] threads;
}

int main(int argc, char * argv []) {
    CController c;
    
    c.initController();
    
    try {
        c.addMonitor("127.0.0.1", 12345, 1);
        
        c.connectAll();
        multiAccess(10, &c);
        c.disconnectAll();
        
        c.destroyController();
    
    } catch (const char * x) {
        printf("MAIN: caught:%s\n", x);
    }
    return 0;
}
