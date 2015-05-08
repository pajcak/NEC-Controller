//    File: CController.h
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

#ifndef __CCONTROLLER_H__
#define	__CCONTROLLER_H__

#include "CMonitor.h"
#include "CParameter.h"
#include <set>


    pthread_mutex_t                     m_monitorsMutex;
    std::map<int, CMonitor*>            m_monitors;
    std::map<std::string, CParameter>   m_parameters;
    
    void initParameters();
    /** CALL THIS METHOD BEFORE USING ANY OTHER OPERATION
     */
    extern "C" void initController     (); 
    /** CALL THIS METHOD AFTER BEING DONE
     */
    extern "C" void destroyController  (); 
//------------------------------------------------------
    extern "C" void addMonitor         (const char * monitorAddr, int port, int monitorID);
    /**
     * @param monitorID - ID of monitor to get the state of connection from
     * @return true if monitor specified by monitorID disconnected successfully, otherwise false
     */
    extern "C" void deleteMonitor      (int monitorID);
    /**
     * @param monitorID - ID of monitor to connect
     */
    extern "C" bool connectMonitor     (int monitorID);
    /**
     * @param monitorID - ID of monitor to disconnect
     */
    extern "C" void disconnectMonitor  (int monitorID);
    /**
     * @return true if all monitors successfully connected, otherwise false
     */
    extern "C" bool connectAll         ();
    
    extern "C" void disconnectAll      ();
    /**
     * @param monitorID - ID of monitor to get the state of connection from
     * @return true if monitor specified by monitorID is connected, otherwise false
     */
    extern "C" bool isConnected        (int monitorID);
//======================================================
//=========================API==========================
//======================================================
    
//=====================PARAMETERS=======================
    
//------------------------------BACKLIGHT------------------------------------------    
    /**
     * @param monitorID - ID of monitor to get the backlight from
     * @return the actual backlight value of monitor specified by monitorID
     */
    extern "C" int  getBacklight       (int monitorID);
    /**
     * @param monitorID - ID of monitor to set the backlight to
     * val - value of backlight to be set (range: 0(dark) -> 100(bright) inclusive)
     */
    extern "C" void setBacklight       (int monitorID, int val);
//------------------------------CONTRAST-------------------------------------------    
    /**
     * @param monitorID - ID of monitor to get the contrast from
     * @return the actual contrast value of monitor specified by monitorID
     */
    extern "C" int  getContrast        (int monitorID);
    /**
     * @param monitorID - ID of monitor to set the contrast to
     * val - value of contrast to be set (range: 0(low) -> 100(high) inclusive)
     */
    extern "C" void setContrast        (int monitorID, int val);
//------------------------------SHARPNESS------------------------------------------    
    /**
     * @param monitorID - ID of monitor to get the sharpness from
     * @return the actual sharpness value of monitor specified by monitorID
     */
    extern "C" int  getSharpness       (int monitorID);
    /**
     * @param monitorID - ID of monitor to set the sharpness to
     * val - value of sharpness to be set (range: 0(dull) -> 24(sharp) inclusive)
     */
    extern "C" void setSharpness       (int monitorID, int val);
//------------------------------BRIGHTNESS-----------------------------------------    
    /**
     * @param monitorID - ID of monitor to get the brightness from
     * @return the actual brightness value of monitor specified by monitorID
     */
    extern "C" int  getBrightness      (int monitorID);
    /**
     * @param monitorID - ID of monitor to set the brightness to
     * val - value of brightness to be set (range: 0(dark) -> 100(bright) inclusive)
     */
    extern "C" void setBrightness      (int monitorID, int val);
//---------------------------------HUE---------------------------------------------    
    /**
     * @param monitorID - ID of monitor to get the hue from
     * @return the actual hue value of monitor specified by monitorID
     */
    extern "C" int  getHue             (int monitorID);
    /**
     * @param monitorID - ID of monitor to set the hue to
     * val - value of hue to be set (range: 0(purplish) -> 100(greenish) inclusive)
     */
    extern "C" void setHue             (int monitorID, int val);
//------------------------------PALENESS-------------------------------------------    
    /**
     * @param monitorID - ID of monitor to get the paleness from
     * @return the actual paleness value of monitor specified by monitorID
     */
    extern "C" int  getPaleness       (int monitorID);
    /**
     * @param monitorID - ID of monitor to set the paleness to
     * val - value of paleness to be set (range: 0(pale) -> 100(deep) inclusive)
     */
    extern "C" void setPaleness       (int monitorID, int val);
//------------------------------COLOR-TEMPERATURE----------------------------------    
    /**
     * @param monitorID - ID of monitor to get the color temperature from
     * @return the actual color temperature value of monitor specified by monitorID
     */
    extern "C" int  getColorTemperature       (int monitorID);
    /**
     * @param monitorID - ID of monitor to set the color temperature to
     * val - value of color temperature to be set (range: 0(2600K) -> 74(10000K) inclusive)
     */
    extern "C" void setColorTemperature       (int monitorID, int val);
//------------------------------GAMMA-CORRECTION-----------------------------------    
    /**
     * @param monitorID - ID of monitor to get the gamma correction from
     * @return the actual gamma correction value of monitor specified by monitorID
     */
    extern "C" int  getGammaCorrection       (int monitorID);
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
    extern "C" void setGammaCorrection       (int monitorID, int val);
//------------------------------VOLUME------------------------------------------    
    /**
     * @param monitorID - ID of monitor to get the volume from
     * @return the actual volume value of monitor specified by monitorID
     */
    extern "C" int  getVolume       (int monitorID);
    /**
     * @param monitorID - ID of monitor to set the volume to
     * val - value of volume to be set (range: 0(whisper) -> 100(loud) inclusive)
     */
    extern "C" void setVolume       (int monitorID, int val);
    
//===================COMMANDS===========================
    /**
     * @param monitorID - ID of monitor to read the status from
     * @return 1: ON\n
     * 2: Stand-by (power save)\n
     * 3: Suspend (power save)\n
     * 4: OFF (same as IR power off)
     */
    extern "C" int powerStatusRead(int monitorID);
    /**
     * 
     * @param monitorID - ID of monitor to control
     * powerMode - 1(ON) or 4(OFF)
     */
    extern "C" void powerControl(int monitorID, int powerMode);

#endif	/* __CCONTROLLER_H__ */

