//__________________________________________________________________________________________________
//  Mention CONFIDENTIAL: Copyright (C) 2008 Mention Inc. All Rights Reserved.
//  
//  endian.h
//  
//  Description: For determaning which byte order we are using
//  
//  
//__________________________________________________________________________________________________

#ifndef ENDIAN_H
#define ENDIAN_H

#define BIT64MASK(pos) (U64(0xff)<<(pos*8))

#define FLIPBYTES2(swapit,n) (swapit ? (((n)&0xff00)>>8) | (((n)&0x00ff)<<8) : (n))
#define FLIPBYTES4(swapit,n) (swapit ? (((n)&0xff000000)>>24) | (((n)&0x00ff0000)>>8) | (((n)&0x0000ff00)<<8) | (((n)&0x000000ff)<<24) : (n))
#define FLIPBYTES8(swapit,n) (swapit ? (((n)&BIT64MASK(7))>>56) |(((n)&BIT64MASK(6))>>40) | (((n)&BIT64MASK(5))>>24) | (((n)&BIT64MASK(4))>>8) | (((n)&BIT64MASK(3))<<8) | (((n)&BIT64MASK(2))<<24) | (((n)&BIT64MASK(1))<<40) | (((n)&BIT64MASK(0))<<56) : (n))

	// Which byte order is this machine
	enum MachineByteEndian_e 
	{
		e_LittleEndian,
		e_BigEndian
	};


#endif // ENDIAN_H
