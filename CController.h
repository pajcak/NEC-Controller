#ifndef __CCONTROLLER_H__
#define	__CCONTROLLER_H__

class CController {
public:
    CController();
    virtual ~CController();
//-----------------------API----------------------------
    int getBrightness();
    bool setBrightness(const int & val);
    
private:
};

#endif	/* __CCONTROLLER_H__ */

