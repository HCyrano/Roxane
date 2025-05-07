/*
 *  RXTools.hpp
 *  Roxane
 *
 *  Created by Bruno Causse on 04/11/08.
 *  Copyleft 2008-2025 personnel.
 *
 */

#ifndef RXTOOLS_HPP
#define RXTOOLS_HPP

#include <string>
#include <sys/time.h>
#include <sstream>

static inline unsigned long long _cntlzll(const unsigned long long Mask)
{
//	unsigned long long Ret;
//	__asm__
//	(
//	 "bsrq %[Mask], %[Ret]"
//	 :[Ret] "=r" (Ret)
//	 :[Mask] "mr" (Mask)
//	 );
//	
//	return Ret^63;
    
    return __builtin_clzll(Mask);
}


static inline unsigned long long _bsrll(const unsigned long long Mask)
{
//	unsigned long long Ret;
//	__asm__
//	(
//	 "bsrq %[Mask], %[Ret]"
//	 :[Ret] "=r" (Ret)
//	 :[Mask] "mr" (Mask)
//	 );
//	
//	return Ret;
    
    return __builtin_clzll(Mask)^63;
}



//static inline unsigned long long _popcntll(const unsigned long long Mask)
//{
//    unsigned long long Ret = Mask
//        - ((Mask >> 1) & 0x7777777777777777ULL)
//        - ((Mask >> 2) & 0x3333333333333333ULL)
//        - ((Mask >> 3) & 0x1111111111111111ULL);
//        
//    return ((Ret + (Ret >> 4)) & 0x0F0F0F0F0F0F0F0FULL) % 0xFFULL;
//   
// }


//static inline unsigned long long _popcntll(const unsigned long long mask)
//{
//
//	unsigned long long ret =  mask - ((mask >> 1) & 0x5555555555555555ULL);
//	ret  = ((ret >> 2) & 0x3333333333333333ULL) + (ret & 0x3333333333333333ULL);
//	ret  = ((ret >> 4) + ret) & 0x0f0f0f0f0f0f0f0fULL;
//	ret *= 0x0101010101010101ULL;
//
//	return  ret >> 56;
// }

static inline std::string toHMS(double t) {
	
	std::ostringstream buffer;
	buffer.fill('0');
	
	buffer << std::setw(2) << static_cast<int>(t/(3600)) << ':';
	buffer << std::setw(2) << static_cast<int>(t/(60)) %(60) << ':';
	buffer << std::setw(2) << static_cast<int>(t) %(60) << '.';
	buffer << std::setw(2) << static_cast<int>(t*100)%100;
	
	return buffer.str();
}

// get_system_time() returns the current system time, measured in
// milliseconds.
static inline int get_system_time() {
	struct timeval t;
	gettimeofday(&t, NULL);
	return static_cast<int>(t.tv_sec*1000 + t.tv_usec/1000); 
}

#endif
