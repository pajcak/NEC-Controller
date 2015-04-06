
#ifndef __UTILS_H__
#define	__UTILS_H__

int fourBytesToInt( const unsigned char leftmost,
                        const unsigned char left,
                        const unsigned char right,
                        const unsigned char rightmost);
void IntToFourBytes(const int & val, unsigned char bytes[4]);
#endif	/* __UTILS_H__ */

