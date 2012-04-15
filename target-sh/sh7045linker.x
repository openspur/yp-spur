/*
 Linker script of SH7045 for YPSpur
  Written by Atsushi Watanabe
  Robot Platform Project of Intelligent Robot Laboratory, University of Tsukuba
*/

/*
 Copyright 2011 Intelligent Robot Laboratory, University of Tsukuba. 
  All rights reserved.

 Redistribution and use in source and binary forms, with or without 
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this 
   list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR 
 IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO 
 EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 NOTE:
  This linker script does not use external ram
*/

OUTPUT_ARCH( sh )
ENTRY( "_reset_vector" )
MEMORY
{
	vect( R )      : ORIGIN = 0x00000000, LENGTH = 0x00000280
	dtc_vect( R )  : ORIGIN = 0x00000400, LENGTH = 0x00000060
	rom( RX )      : ORIGIN = 0x00000480, LENGTH = 0x0003FFFF-0x480
	ram( RWX )     : ORIGIN = 0xFFFFF000, LENGTH = 0x00000FFF
}

SECTIONS
{
	.vector :
	{
		. = 0x0000 * 4;
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( 0 ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( 0 ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		. = 0x0010 * 4;
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		. = 0x0020 * 4;
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		. = 0x0030 * 4;
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		. = 0x0040 * 4;
		LONG( DEFINED( _int_irq0 ) ? ABSOLUTE( _int_irq0 ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_irq1 ) ? ABSOLUTE( _int_irq1 ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_irq2 ) ? ABSOLUTE( _int_irq2 ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_irq3 ) ? ABSOLUTE( _int_irq3 ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_irq4 ) ? ABSOLUTE( _int_irq4 ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_irq5 ) ? ABSOLUTE( _int_irq5 ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_irq6 ) ? ABSOLUTE( _int_irq6 ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_irq7 ) ? ABSOLUTE( _int_irq7 ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_dei0 ) ? ABSOLUTE( _int_dei0 ) : ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_dei1 ) ? ABSOLUTE( _int_dei1 ) : ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		. = 0x0050 * 4;
		LONG( DEFINED( _int_dei2 ) ? ABSOLUTE( _int_dei2 ) : ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_dei3 ) ? ABSOLUTE( _int_dei3 ) : ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tgi0a ) ? ABSOLUTE( _int_tgi0a ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tgi0b ) ? ABSOLUTE( _int_tgi0b ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tgi0c ) ? ABSOLUTE( _int_tgi0c ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tgi0d ) ? ABSOLUTE( _int_tgi0d ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tci0v ) ? ABSOLUTE( _int_tci0v ) : ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		. = 0x0060 * 4;
		LONG( DEFINED( _int_tgi1a ) ? ABSOLUTE( _int_tgi1a ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tgi1b ) ? ABSOLUTE( _int_tgi1b ) : ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tci1v ) ? ABSOLUTE( _int_tci1v ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tci1u ) ? ABSOLUTE( _int_tci1u ) : ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tgi2a ) ? ABSOLUTE( _int_tgi2a ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tgi2b ) ? ABSOLUTE( _int_tgi2b ) : ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tci2v ) ? ABSOLUTE( _int_tci2v ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tci2u ) ? ABSOLUTE( _int_tci2u ) : ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		. = 0x0070 * 4;
		LONG( DEFINED( _int_tgi3a ) ? ABSOLUTE( _int_tgi3a ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tgi3b ) ? ABSOLUTE( _int_tgi3b ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tgi3c ) ? ABSOLUTE( _int_tgi3c ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tgi3d ) ? ABSOLUTE( _int_tgi3d ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tci3v ) ? ABSOLUTE( _int_tci3v ) : ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tgi4a ) ? ABSOLUTE( _int_tgi4a ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tgi4b ) ? ABSOLUTE( _int_tgi4b ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tgi4c ) ? ABSOLUTE( _int_tgi4c ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tgi4d ) ? ABSOLUTE( _int_tgi4d ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tci4v ) ? ABSOLUTE( _int_tci4v ) : ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		. = 0x0080 * 4;
		LONG( DEFINED( _int_eri0 ) ? ABSOLUTE( _int_eri0 ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_rxi0 ) ? ABSOLUTE( _int_rxi0 ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_txi0 ) ? ABSOLUTE( _int_txi0 ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tei0 ) ? ABSOLUTE( _int_tei0 ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_eri1 ) ? ABSOLUTE( _int_eri1 ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_rxi1 ) ? ABSOLUTE( _int_rxi1 ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_txi1 ) ? ABSOLUTE( _int_txi1 ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_tei1 ) ? ABSOLUTE( _int_tei1 ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_adi0 ) ? ABSOLUTE( _int_adi0 ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_adi1 ) ? ABSOLUTE( _int_adi1 ) : ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_swdtce ) ? ABSOLUTE( _int_swdtce ) : ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		. = 0x0090 * 4;
		LONG( DEFINED( _int_cmi0 ) ? ABSOLUTE( _int_cmi0 ) : ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_cmi1 ) ? ABSOLUTE( _int_cmi1 ) : ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_iti ) ? ABSOLUTE( _int_iti ) : ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_cmi ) ? ABSOLUTE( _int_cmi ) : ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( ABSOLUTE( _reset_vector ) )
		LONG( DEFINED( _int_oei ) ? ABSOLUTE( _int_oei ) : ABSOLUTE( _reset_vector ) )
		FILL( 0xff )
	} > vect

	.dtc : 
	{
		. = 0x0000;	/* MTU CH4 */
		SHORT( DEFINED( _dtc_tgi4a ) ? ABSOLUTE( ( _dtc_tgi4a ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_tgi4b ) ? ABSOLUTE( ( _dtc_tgi4b ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_tgi4c ) ? ABSOLUTE( ( _dtc_tgi4c ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_tgi4d ) ? ABSOLUTE( ( _dtc_tgi4d ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_tci4v ) ? ABSOLUTE( ( _dtc_tci4v ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		. = 0x000A;	/* MTU CH3 */
		SHORT( DEFINED( _dtc_tgi3a ) ? ABSOLUTE( ( _dtc_tgi3a ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_tgi3b ) ? ABSOLUTE( ( _dtc_tgi3b ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_tgi3c ) ? ABSOLUTE( ( _dtc_tgi3c ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_tgi3d ) ? ABSOLUTE( ( _dtc_tgi3d ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		. = 0x0012;	/* MTU CH2 */
		SHORT( DEFINED( _dtc_tgi2a ) ? ABSOLUTE( ( _dtc_tgi2a ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_tgi2b ) ? ABSOLUTE( ( _dtc_tgi2b ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		. = 0x0016;	/* MTU CH1 */
		SHORT( DEFINED( _dtc_tgi1a ) ? ABSOLUTE( ( _dtc_tgi1a ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_tgi1b ) ? ABSOLUTE( ( _dtc_tgi1b ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		. = 0x001A;	/* MTU CH0 */
		SHORT( DEFINED( _dtc_tgi0a ) ? ABSOLUTE( ( _dtc_tgi0a ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_tgi0b ) ? ABSOLUTE( ( _dtc_tgi0b ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_tgi0c ) ? ABSOLUTE( ( _dtc_tgi0c ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_tgi0d ) ? ABSOLUTE( ( _dtc_tgi0d ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		. = 0x0022;	/* AD */
		SHORT( DEFINED( _dtc_adi ) ? ABSOLUTE( ( _dtc_adi ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		. = 0x0024;	/* IRQ */
		SHORT( DEFINED( _dtc_irq0 ) ? ABSOLUTE( ( _dtc_irq0 ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_irq1 ) ? ABSOLUTE( ( _dtc_irq1 ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_irq2 ) ? ABSOLUTE( ( _dtc_irq2 ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_irq3 ) ? ABSOLUTE( ( _dtc_irq3 ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_irq4 ) ? ABSOLUTE( ( _dtc_irq4 ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_irq5 ) ? ABSOLUTE( ( _dtc_irq5 ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_irq6 ) ? ABSOLUTE( ( _dtc_irq6 ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_irq7 ) ? ABSOLUTE( ( _dtc_irq7 ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		. = 0x0034;	/* CMT */
		SHORT( DEFINED( _dtc_cmi0 ) ? ABSOLUTE( ( _dtc_cmi0 ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_cmi1 ) ? ABSOLUTE( ( _dtc_cmi1 ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		. = 0x0038;	/* SCI */
		SHORT( DEFINED( _dtc_rxi0 ) ? ABSOLUTE( ( _dtc_rxi0 ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_txi0 ) ? ABSOLUTE( ( _dtc_txi0 ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_rxi1 ) ? ABSOLUTE( ( _dtc_rxi1 ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		SHORT( DEFINED( _dtc_txi1 ) ? ABSOLUTE( ( _dtc_txi1 ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
		. = 0x0040;	/* BSC */
		SHORT( DEFINED( _dtc_cmi ) ? ABSOLUTE( ( _dtc_cmi ) & 0xFFFF ) : ABSOLUTE( _reset_vector ) )
	} > dtc_vect

	.readonly : 
	{
		CREATE_OBJECT_SYMBOLS
		*( .text )
		*( .text.* )
		*( .rodata )
		*( .rodata.* )
		_data_img_begin = .;
	} > rom

	.readwrite : 
	AT ( _data_img_begin ) 
	{
		_data_begin    = .;
		*( .data )
		_data_end    = .;
		_bss_begin    = .;
		*( .bss )
		_bss_end    = .;
	} > ram
}

