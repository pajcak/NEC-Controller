//    File: Utils.h
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

#ifndef __UTILS_H__
#define	__UTILS_H__

int fourBytesToInt( const unsigned char leftmost,
                        const unsigned char left,
                        const unsigned char right,
                        const unsigned char rightmost);
void IntToFourBytes(const int & val, unsigned char bytes[4]);
#endif	/* __UTILS_H__ */

