/*
 *  LCD module "SB1602E" library
 *
 *  SB1602E is an I2C based low voltage text LCD panel (based Sitronix ST7032 chip)
 *  The module by StrawberryLinux
 *  http://strawberry-linux.com/catalog/items?code=27002 (Online shop page (Japanese))
 *  http://strawberry-linux.com/pub/ST7032i.pdf (datasheet of the chip)
 *
 *  This is a library to operate this module easy.
 *
 *  Copyright (c) 2010 Tedd OKANO
 *  Released under the MIT License: http://mbed.org/license/mit
 *
 *  revision 1.0  22-Jan-2010   a. 1st release
 *  revision 1.1  23-Jan-2010   a. class name has been changed from lcd_SB1602E to TextLCD_SB1602E
 *                               b. printf() added
 *                              c. copyright notice added
 */

#ifndef        MBED_TextLCD_SB1602E
#define        MBED_TextLCD_SB1602E

#include    <stdarg.h>
#include    "mbed.h"
#include    "I2cBusDevice.h"

//  SB1602E IIC address

const char    SB1602E_addr = 0x7C;

//  SB1602E initialization command sequence

#ifdef INIT_VALUE_DATASHEET_ORIGINAL

const char    Comm_FunctionSet_Normal      = 0x38;
const char    Comm_FunctionSet_Extended    = 0x39;
const char    Comm_InternalOscFrequency    = 0x14;
const char    Comm_ContrastSet             = 0x78;
const char    Comm_PwrIconContrast         = 0x5E;
const char    Comm_FollowerCtrl            = 0x6A;
const char    Comm_DisplayOnOff            = 0x0C;
const char    Comm_ClearDisplay            = 0x01;
const char    Comm_EntryModeSet            = 0x06;

#else

const char    Comm_FunctionSet_Normal      = 0x38;
const char    Comm_FunctionSet_Extended    = 0x39;
const char    Comm_InternalOscFrequency    = 0x14;
const char    Comm_ContrastSet             = 0x70;
const char    Comm_PwrIconContrast         = 0x5C;
const char    Comm_FollowerCtrl            = 0x60;
const char    Comm_DisplayOnOff            = 0x0C;
const char    Comm_ClearDisplay            = 0x01;
const char    Comm_EntryModeSet            = 0x04;
const char    Comm_ReturnHome              = 0x02;

#endif

//  SB1602E general commands

const char    Comm_SetDDRAMAddress        = 0x80;
const char    DDRAMAddress_Ofst[]         = { 0x00, 0x40 };

const char    Comm_SetCGRAM               = 0x40;

//  SB1602E setting values

const char    default_Contrast            = 0x35;

const char    COMMAND                     = 0x00;
const char    DATA                        = 0x40;

const char    MaxCharsInALine             = 0x10;    //    buffer deoth for one line (no scroll function used)

const char    init_seq0_length  = 7;
const char    init_seq0[ init_seq0_length ]
=    {
    Comm_FunctionSet_Normal,
    Comm_ReturnHome,             //    This may be required to reset the scroll function
    Comm_FunctionSet_Extended,
    Comm_InternalOscFrequency,
    Comm_ContrastSet            | ( default_Contrast       & 0xF),
    Comm_PwrIconContrast        | ((default_Contrast >> 4) & 0x3),
    Comm_FollowerCtrl           | 0x0A,

};
// required 30us interval

const char    init_seq1_length  = 3;
const char    init_seq1[ init_seq1_length ]
=    {
    Comm_DisplayOnOff,
    Comm_ClearDisplay,
    Comm_EntryModeSet,
};
// required 30us, 2ms interval


class TextLCD_SB1602E : I2cBusDevice {
public:

    explicit TextLCD_SB1602E( I2C *i2c, char dev_address = SB1602E_addr, char *init_massage = NULL ) : I2cBusDevice( i2c, dev_address ) {
        wait( 0.04 );    //    interval after hardware reset

        for ( int i = 0; i < init_seq0_length; i++ ) {
            lcd_command( init_seq0[ i ] );
            wait( 30e-6 );
        }

        wait( 0.2 );

        for ( int i = 0; i < init_seq1_length; i++ ) {
            lcd_command( init_seq1[ i ] );
            wait( 2e-3 );
        }

        if ( init_massage )
            puts( 0, init_massage );

        set_CGRAM( 7, '\x1F' );

        curs[ 0 ]    = 0;
        curs[ 1 ]    = 0;
    }


    ~TextLCD_SB1602E() {
    }

    void clear( void ) {
        lcd_command( Comm_ClearDisplay );
        wait( 2e-3 );
        curs[ 0 ]    = 0;
        curs[ 1 ]    = 0;
    }


    void put_custom_char( char c_code, const char *cg, char x, char y ) {
        for ( int i = 0; i < 5; i++ ) {
            set_CGRAM( c_code, cg );
            putcxy( c_code, x, y );
        }
    }

    void contrast( char contrast ) {
        lcd_command( Comm_FunctionSet_Extended );
        lcd_command( Comm_ContrastSet         |  (contrast     & 0x0f) );
        lcd_command( Comm_PwrIconContrast     | ((contrast>>4) & 0x03) );
        lcd_command( Comm_FunctionSet_Normal   );
    }

    void set_CGRAM( char char_code, const char* cg ) {
        for ( int i = 0; i < 8; i++ ) {
            lcd_command( (Comm_SetCGRAM | (char_code << 3) | i) );
            lcd_data( *cg++ );
        }
    }

    void set_CGRAM( char char_code, char v ) {
        char    c[ 8 ];

        for ( int i = 0; i < 8; i++ )
            c[ i ]    = v;

        set_CGRAM( char_code, c );
    }

    void putcxy( char c, char x, char y ) {
        if ( (x >= MaxCharsInALine) || (y >= 2) )
            return;

        lcd_command( (Comm_SetDDRAMAddress | DDRAMAddress_Ofst[ y ]) + x );
        lcd_data( c );
    }

    void putc( char line, char c ) {
        if ( (c == '\n') || (c == '\r') ) {
            clear_lest_of_line( line );
            curs[ line ]    = 0;
            return;
        }

        putcxy( c, curs[ line ]++, line );
    }

    void puts( char line, char *s ) {
        while ( char c    = *s++ )
            putc( line, c );
    }

    void printf( char line, char *format, ... ) {
        char        s[ 32 ];
        va_list        args;

        va_start( args, format );
        vsnprintf( s, 32, format, args );
        va_end( args );

        puts( line, s );
    }

private:
    char    curs[2];

    void clear_lest_of_line( char line ) {
        for ( int i = curs[ line ]; i < MaxCharsInALine; i++ )
            putcxy( ' ', i, line );
    }

    int lcd_write( char first, char second ) {
        char cmd[2];

        cmd[ 0 ]    = first;
        cmd[ 1 ]    = second;

        return ( write( cmd, 2 ) );

    }

    int lcd_command( char command ) {
        return ( lcd_write( COMMAND, command ) );
    }

    int lcd_data( char data ) {
        return ( lcd_write( DATA, data ) );
    }
}
;

#endif








