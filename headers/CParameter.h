//    File: CParameter.h
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

#ifndef __CPARAMETER_H__
#define	__CPARAMETER_H__

#include <map>
#include <string>

class IRange {
public:
    virtual ~IRange() {}
    virtual IRange * clone() const = 0;
    virtual bool contains(const int & value) const = 0;
private:
};
class CRangeInterval : public IRange {
public:
    CRangeInterval(const CRangeInterval & other) 
    : m_minValue(other.m_minValue), m_maxValue(other.m_maxValue),
      m_minDescription(other.m_minDescription),
      m_maxDescription(other.m_maxDescription)
    {
    }
    CRangeInterval(const int minV, const std::string minD,
                    const int maxV, const std::string maxD)
    : m_minValue(minV), m_maxValue(maxV),
            m_minDescription(minD), m_maxDescription(maxD)
    {
    }
    CRangeInterval * clone() const {
        return new CRangeInterval(*this);
    }
    bool contains (const int & value) const {
        return (value >= m_minValue && value <= m_maxValue);
    }
private:
    const int m_minValue;
    const int m_maxValue;
    const std::string m_minDescription;
    const std::string m_maxDescription;
};
class CRangeEnum : public IRange {
public:
    CRangeEnum() {}
    CRangeEnum(const CRangeEnum & other)
    : m_values(other.m_values) 
    {
    }
    CRangeEnum * clone() const {
        return new CRangeEnum(*this);
    }
    void add(const int value, const std::string description) {
        m_values.insert(std::pair<int, const std::string> (value, description));
    }
    void remove(const int value) {
        m_values.erase(value);
    }
    bool contains (const int & value) const {
        return (m_values.find(value) != m_values.end());
    }
private:
    std::map<int, const std::string> m_values;
};


class CParameter {
public:
    CParameter(
            const unsigned char opCodePage0,
            const unsigned char opCodePage1,
            const unsigned char opCode0,
            const unsigned char opCode1,
            const IRange * range);
    CParameter(const CParameter &);
    ~CParameter();
    unsigned char m_opCodePage[2];
    unsigned char m_opCode[2];
    const IRange * m_range;
};

#endif	/* __CPARAMETER_H__ */

