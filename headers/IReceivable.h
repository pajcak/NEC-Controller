//    File: IReceivable.h
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

#include "CAbstractMessage.h"
#ifndef __IRECEIVABLE_H__
#define	__IRECEIVABLE_H__

class IReceivable {    
public:
    virtual void initWithRawData(const unsigned char * _buffer) = 0;
};

#endif	/* __IRECEIVABLE_H__ */

