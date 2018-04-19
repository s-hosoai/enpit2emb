/*
 *  I2C device base class
 *
 *  A base class for all I2C devices.
 *  This manages the device address and transfers
 *
 *  Copyright (c) 2010 Tedd OKANO
 *  Released under the MIT License: http://mbed.org/license/mit
 *
 *  revision 1.0  15-Jan-2010   a. 1st release
 *  revision 1.1  23-Jan-2010   a. The word "MBED_I2cBusDevice" is used instead of _I2cBusDevice_ to avoid symbol conflict
 *                              b. copyright notice added
 */

#ifndef    MBED_I2cBusDevice
#define    MBED_I2cBusDevice

#include    "mbed.h"

class I2cBusDevice {
public:

    I2cBusDevice( I2C *i2c, char dev_address ) {
        bus          = i2c;
        device       = dev_address;
    }

    ~I2cBusDevice() {
    }

    int write( char *data, int length ) {
        return ( bus->write( device, data, length) );
    }

    int read( char *data, int length ) {
        return ( bus->read( device, data, length) );
    }

    int read( char reg_ptr, char *data, int length ) {
        if ( bus->write( device, &reg_ptr, 1 ) )
            return ( 1 );

        if ( bus->read( device, data, length ) )
            return ( 1 );

        return ( 0 );
    }

protected:
    I2C     *bus;
    char    device;

private:
    static char    i2c_error;
}
;

#endif
