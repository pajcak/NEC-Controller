#ifndef __CCONTROLLER_H__
#define	__CCONTROLLER_H__

#include "CMonitor.h"
#include "CParameter.h"
#include <set>

class CController {
public:
    CController             ();
    CController             (const char * monitorAddr);
    virtual ~CController    ();
    
        
    /** CALL THIS METHOD BEFORE USING ANY OTHER OPERATION
     */
    void initController     (); 
    /** CALL THIS METHOD AFTER BEING DONE
     */
    void destroyController  (); 
//------------------------------------------------------
    void addMonitor         (const char * monitorAddr, int port, int monitorID);
    /**
     * @param monitorID - ID of monitor to get the state of connection from
     * @return true if monitor specified by monitorID disconnected successfully, otherwise false
     */
    void deleteMonitor      (int monitorID);
    /**
     * @param monitorID - ID of monitor to connect
     */
    bool connectMonitor     (int monitorID);
    /**
     * @param monitorID - ID of monitor to disconnect
     */
    void disconnectMonitor  (int monitorID);
    /**
     * @return true if all monitors successfully connected, otherwise false
     */
    bool connectAll         ();
    
    void disconnectAll      ();
    /**
     * @param monitorID - ID of monitor to get the state of connection from
     * @return true if monitor specified by monitorID is connected, otherwise false
     */
    bool isConnected        (int monitorID);
//======================================================
//=========================API==========================
//======================================================
    
//=====================PARAMETERS=======================
    
//------------------------------BACKLIGHT------------------------------------------    
    /**
     * @param monitorID - ID of monitor to get the backlight from
     * @return the actual backlight value of monitor specified by monitorID
     */
    int  getBacklight       (int monitorID);
    /**
     * @param monitorID - ID of monitor to set the backlight to
     * val - value of backlight to be set (range: 0(dark) -> 100(bright) inclusive)
     */
    void setBacklight       (int monitorID, int val);
//------------------------------CONTRAST-------------------------------------------    
    /**
     * @param monitorID - ID of monitor to get the contrast from
     * @return the actual contrast value of monitor specified by monitorID
     */
    int  getContrast        (int monitorID);
    /**
     * @param monitorID - ID of monitor to set the contrast to
     * val - value of contrast to be set (range: 0(low) -> 100(high) inclusive)
     */
    void setContrast        (int monitorID, int val);
//------------------------------SHARPNESS------------------------------------------    
    /**
     * @param monitorID - ID of monitor to get the sharpness from
     * @return the actual sharpness value of monitor specified by monitorID
     */
    int  getSharpness       (int monitorID);
    /**
     * @param monitorID - ID of monitor to set the sharpness to
     * val - value of sharpness to be set (range: 0(dull) -> 24(sharp) inclusive)
     */
    void setSharpness       (int monitorID, int val);
//------------------------------BRIGHTNESS-----------------------------------------    
    /**
     * @param monitorID - ID of monitor to get the brightness from
     * @return the actual brightness value of monitor specified by monitorID
     */
    int  getBrightness      (int monitorID);
    /**
     * @param monitorID - ID of monitor to set the brightness to
     * val - value of brightness to be set (range: 0(dark) -> 100(bright) inclusive)
     */
    void setBrightness      (int monitorID, int val);
//---------------------------------HUE---------------------------------------------    
    /**
     * @param monitorID - ID of monitor to get the hue from
     * @return the actual hue value of monitor specified by monitorID
     */
    int  getHue             (int monitorID);
    /**
     * @param monitorID - ID of monitor to set the hue to
     * val - value of hue to be set (range: 0(purplish) -> 100(greenish) inclusive)
     */
    void setHue             (int monitorID, int val);
//------------------------------PALENESS-------------------------------------------    
    /**
     * @param monitorID - ID of monitor to get the paleness from
     * @return the actual paleness value of monitor specified by monitorID
     */
    int  getPaleness       (int monitorID);
    /**
     * @param monitorID - ID of monitor to set the paleness to
     * val - value of paleness to be set (range: 0(pale) -> 100(deep) inclusive)
     */
    void setPaleness       (int monitorID, int val);
//------------------------------COLOR-TEMPERATURE----------------------------------    
    /**
     * @param monitorID - ID of monitor to get the color temperature from
     * @return the actual color temperature value of monitor specified by monitorID
     */
    int  getColorTemperature       (int monitorID);
    /**
     * @param monitorID - ID of monitor to set the color temperature to
     * val - value of color temperature to be set (range: 0(2600K) -> 74(10000K) inclusive)
     */
    void setColorTemperature       (int monitorID, int val);
//------------------------------GAMMA-CORRECTION-----------------------------------    
    /**
     * @param monitorID - ID of monitor to get the gamma correction from
     * @return the actual gamma correction value of monitor specified by monitorID
     */
    int  getGammaCorrection       (int monitorID);
    /**
     * @param monitorID - ID of monitor to set the gamma correction to
     * val - value of gamma correction to be set (range:\n
     *  1 -> Native Gamma\n
     *  4 -> Gamma=2.2\n
     *  5 -> Gamma=2.4\n
     *  6 -> S Gamma\n
     *  7 -> DICOM SIM\n
     *  8 -> Programmable)\n
     */
    void setGammaCorrection       (int monitorID, int val);
//------------------------------VOLUME------------------------------------------    
    /**
     * @param monitorID - ID of monitor to get the volume from
     * @return the actual volume value of monitor specified by monitorID
     */
    int  getVolume       (int monitorID);
    /**
     * @param monitorID - ID of monitor to set the volume to
     * val - value of volume to be set (range: 0(whisper) -> 100(loud) inclusive)
     */
    void setVolume       (int monitorID, int val);
    
//===================COMMANDS===========================
    /**
     * @param monitorID - ID of monitor to read the status from
     * @return 1: ON\n
     * 2: Stand-by (power save)\n
     * 3: Suspend (power save)\n
     * 4: OFF (same as IR power off)
     */
    int powerStatusRead(int monitorID);
    /**
     * 
     * @param monitorID - ID of monitor to control
     * powerMode - 1(ON) or 4(OFF)
     */
    void powerControl(int monitorID, int powerMode);
    
private:
    pthread_mutex_t m_monitorsMutex;
    std::map<int, CMonitor*> m_monitors;
    
    void initParameters();
    std::map<std::string, CParameter> m_parameters;
};

#endif	/* __CCONTROLLER_H__ */

