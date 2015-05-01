#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <dlfcn.h>

using namespace std;

// <editor-fold defaultstate="collapsed" desc="CLibLoader definition">

class CLibLoader {
public:

    CLibLoader() {
    }
    bool loadLibrary(const char *);
    bool loadFunctions();
    void ( *m_initController) ();
    void ( *m_destroyController) ();
    void ( *m_addMonitor) (const char *, int, int);
    void ( *m_deleteMonitor) (int);
    bool ( *m_connectMonitor) (int);
    void ( *m_disconnectMonitor) (int);
    bool ( *m_connectAll) ();
    void ( *m_disconnectAll) ();
    bool ( *m_isConnected) (int);
    int ( *m_GetBacklightFunc) (int);
    void ( *m_SetBacklightFunc) (int, int);
    int ( *m_GetContrastFunc) (int);
    void ( *m_SetContrastFunc) (int, int);
    int ( *m_GetSharpnessFunc) (int);
    void ( *m_SetSharpnessFunc) (int, int);
    int ( *m_GetBrightnessFunc) (int);
    void ( *m_SetBrightnessFunc) (int, int);
    int ( *m_GetHueFunc) (int);
    void ( *m_SetHueFunc) (int, int);
    int ( *m_GetPalenessFunc) (int);
    void ( *m_SetPalenessFunc) (int, int);
    int ( *m_GetColorTemperatureFunc) (int);
    void ( *m_SetColorTemperatureFunc) (int, int);
    int ( *m_GetGammaCorrectionFunc) (int);
    void ( *m_SetGammaCorrectionFunc) (int, int);
    int ( *m_GetVolumeFunc) (int);
    void ( *m_SetVolumeFunc) (int, int);
    int ( *m_PowerStatusReadFunc) (int);
    void ( *m_PowerControlFunc) (int, int);
private:
    void * m_Library;
};

bool CLibLoader::loadLibrary(const char * libName) {
    m_Library = dlopen(libName, RTLD_NOW | RTLD_GLOBAL);
    if (m_Library == NULL) {
        cout << "-- " << dlerror() << endl;
        return false;
    }
    return loadFunctions();
}

bool CLibLoader::loadFunctions() {
    m_initController = (void (*)()) dlsym(m_Library, "initController");
    if (!m_initController) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_destroyController = (void (*)()) dlsym(m_Library, "destroyController");
    if (!m_destroyController) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_addMonitor = (void (*)(const char*, int, int)) dlsym(m_Library, "addMonitor");
    if (!m_addMonitor) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_deleteMonitor = (void (*)(int)) dlsym(m_Library, "deleteMonitor");
    if (!m_deleteMonitor) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_connectMonitor = (bool (*)(int)) dlsym(m_Library, "connectMonitor");
    if (!m_connectMonitor) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_disconnectMonitor = (void (*)(int)) dlsym(m_Library, "disconnectMonitor");
    if (!m_disconnectMonitor) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_connectAll = (bool (*)()) dlsym(m_Library, "connectAll");
    if (!m_connectAll) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_disconnectAll = (void (*)()) dlsym(m_Library, "disconnectAll");
    if (!m_disconnectAll) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_isConnected = (bool (*)(int)) dlsym(m_Library, "isConnected");
    if (!m_isConnected) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_GetBacklightFunc = (int (*)(int)) dlsym(m_Library, "getBacklight");
    if (!m_GetBacklightFunc) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_SetBacklightFunc = (void (*)(int, int)) dlsym(m_Library, "setBacklight");
    if (!m_SetBacklightFunc) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_GetContrastFunc = (int (*)(int)) dlsym(m_Library, "getContrast");
    if (!m_GetContrastFunc) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_SetContrastFunc = (void (*)(int, int)) dlsym(m_Library, "setContrast");
    if (!m_SetContrastFunc) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_GetSharpnessFunc = (int (*)(int)) dlsym(m_Library, "getSharpness");
    if (!m_GetSharpnessFunc) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_SetSharpnessFunc = (void (*)(int, int)) dlsym(m_Library, "setSharpness");
    if (!m_SetSharpnessFunc) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_GetBrightnessFunc = (int (*)(int)) dlsym(m_Library, "getBrightness");
    if (!m_GetBrightnessFunc) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_SetBrightnessFunc = (void (*)(int, int)) dlsym(m_Library, "setBrightness");
    if (!m_SetBrightnessFunc) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_GetHueFunc = (int (*)(int)) dlsym(m_Library, "getHue");
    if (!m_GetHueFunc) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_SetHueFunc = (void (*)(int, int)) dlsym(m_Library, "setHue");
    if (!m_SetHueFunc) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_GetPalenessFunc = (int (*)(int)) dlsym(m_Library, "getPaleness");
    if (!m_GetPalenessFunc) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_SetPalenessFunc = (void (*)(int, int)) dlsym(m_Library, "setPaleness");
    if (!m_SetPalenessFunc) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_GetColorTemperatureFunc = (int (*)(int)) dlsym(m_Library, "getColorTemperature");
    if (!m_GetColorTemperatureFunc) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_SetColorTemperatureFunc = (void (*)(int, int)) dlsym(m_Library, "setColorTemperature");
    if (!m_SetColorTemperatureFunc) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_GetGammaCorrectionFunc = (int (*)(int)) dlsym(m_Library, "getGammaCorrection");
    if (!m_GetGammaCorrectionFunc) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_SetGammaCorrectionFunc = (void (*)(int, int)) dlsym(m_Library, "setGammaCorrection");
    if (!m_SetGammaCorrectionFunc) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_GetVolumeFunc = (int (*)(int)) dlsym(m_Library, "getVolume");
    if (!m_GetVolumeFunc) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_SetVolumeFunc = (void (*)(int, int)) dlsym(m_Library, "setVolume");
    if (!m_SetVolumeFunc) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    m_PowerStatusReadFunc = (int (*)(int)) dlsym(m_Library, "powerStatusRead");
    if (!m_PowerStatusReadFunc) {
        cout << "-- " << dlerror() << endl;
        return false;
    }
    
    m_PowerControlFunc = (void (*)(int, int)) dlsym(m_Library, "powerControl");
    if (!m_PowerControlFunc) {
        cout << "-- " << dlerror() << endl;
        return false;
    }

    return true;
}// </editor-fold>

CLibLoader g_Library;

void addCorrectMonitors() {
    g_Library.m_addMonitor("10.0.0.100", 7142, 1);
    g_Library.m_addMonitor("10.0.0.101", 7142, 2);
    g_Library.m_addMonitor("10.0.0.102", 7142, 3);
    g_Library.m_addMonitor("10.0.0.103", 7142, 4);
    g_Library.m_addMonitor("10.0.0.104", 7142, 5);
    g_Library.m_addMonitor("10.0.0.105", 7142, 6);
    g_Library.m_addMonitor("10.0.0.106", 7142, 7);
    g_Library.m_addMonitor("10.0.0.107", 7142, 8);
    g_Library.m_addMonitor("10.0.0.108", 7142, 9);
    g_Library.m_addMonitor("10.0.0.109", 7142, 10);
    g_Library.m_addMonitor("10.0.0.110", 7142, 11);
    g_Library.m_addMonitor("10.0.0.111", 7142, 12);
    g_Library.m_addMonitor("10.0.0.112", 7142, 13);
    g_Library.m_addMonitor("10.0.0.113", 7142, 14);
    g_Library.m_addMonitor("10.0.0.114", 7142, 15);
    g_Library.m_addMonitor("10.0.0.115", 7142, 16);
    g_Library.m_addMonitor("10.0.0.116", 7142, 17);
    g_Library.m_addMonitor("10.0.0.117", 7142, 18);
    g_Library.m_addMonitor("10.0.0.118", 7142, 19);
    g_Library.m_addMonitor("10.0.0.119", 7142, 20);
}

bool testAddNDeleteMonitors() {
    try {
        addCorrectMonitors();
        g_Library.m_addMonitor("1.1.1.1", 11214, 1);
    } catch (char const * str) {
        cout << "-- " << str << endl;
        return false;
    } catch (...) {
        cout << "-- unknown exception thrown" << endl;
        return false;
    }
    return true;
}

int main () {
    cout << "%START% - LOAD LIBRARY" << endl;
    if (g_Library.loadLibrary("./controller.so")) cout << "-- OK" << endl;
    else return ( EXIT_FAILURE );
    cout << "%FINISH% - LOAD LIBRARY" << endl;
    
    cout << endl;
    
    cout << "%START% - ADD and DELETE MONITOR" << endl;
    if (testAddNDeleteMonitors()) cout << "-- OK" << endl;
    else return ( EXIT_FAILURE );
    cout << "%FINISH% - ADD and DELETE MONITOR" << endl;
    
    return ( EXIT_SUCCESS );
}