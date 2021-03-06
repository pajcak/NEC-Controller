//    File: Utils.cpp
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

#include "headers/Utils.h"

int fourBytesToInt( const unsigned char leftmost,
                        const unsigned char left,
                        const unsigned char right,
                        const unsigned char rightmost)
{
    return (int)(
            (((leftmost  >= 'A') ? (leftmost  - 'A' + 10) : (leftmost  - '0')) << 16) 
          + (((left      >= 'A') ? (left      - 'A' + 10) : (left      - '0')) << 8)
          + (((right     >= 'A') ? (right     - 'A' + 10) : (right     - '0')) << 4)
          +  ((rightmost >= 'A') ? (rightmost - 'A' + 10) : (rightmost - '0'))
          );
}
void IntToFourBytes(const int & val, unsigned char bytes[4]) {
	bytes[0] = ((val >> 16) & 15) + '0';
        if (bytes[0] > '9') bytes[0] += 7;
	bytes[1] = ((val >> 8) & 15) + '0';
        if (bytes[1] > '9') bytes[1] += 7;
	bytes[2] = ((val >> 4) & 15) + '0';
        if (bytes[2] > '9') bytes[2] += 7;
	bytes[3] = (val & 15) + '0';
        if (bytes[3] > '9') bytes[3] += 7;
}

