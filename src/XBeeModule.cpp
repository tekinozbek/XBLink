/* File     : XBeeModule.cpp
 * Author(s): Tekin Ozbek <tekin@tekinozbek.com>
 * 
 * 
 * XBLink - Networking between two computers using XBee modules.
 * 
 * Copyright (C) 2015 Tekin Ozbek, Benjamin Yan
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <cstring>
#include <iostream>
#include <functional>

#include <XBeeModule.h>
#include <XBeeMessage.h>

XBeeModule::XBeeModule(std::string type, std::string device, uint32_t baud) {

    last_error = xbee_setup(&xbee, type.c_str(), device.c_str(), baud);
}

XBeeModule::~XBeeModule() {
    
    if (connection != nullptr)
        xbee_conEnd(connection);
        
    xbee_shutdown(xbee);
}

xbee_err XBeeModule::get_last_error() const {
    
    return last_error;
}

std::string XBeeModule::get_last_error_str() const {
    
    return xbee_errorToStr(last_error);
}

void XBeeModule::open_connection(std::string mode,
                                 uint64_t address,
                                 rx_handler_f callback) {
                                     
    if (mode != "Local AT") {
    
        /* convert address into xbee_conAddress structure */
        struct xbee_conAddress dest_addr;
        memset(&dest_addr, 0, sizeof(dest_addr));
        dest_addr.addr64_enabled = 1;
        
        /* convert 64 bit unsigned integer to addr64 array format */
        for (auto i = 7; i >= 0; i--) {
            
            dest_addr.addr64[i] = (unsigned char)(address & 0xFF);
            address >>= 8;
        }
         
        last_error = xbee_conNew(xbee, &connection, mode.c_str(), &dest_addr);
    }
    
    /* For Local AT connections, destination address must be nullptr */
    else {
        
        last_error = xbee_conNew(xbee, &connection, mode.c_str(), nullptr);
    }
        
    if (last_error == XBEE_ENONE)
        xbee_conCallbackSet(connection, callback, nullptr);
}

int XBeeModule::tx(const char* buf, unsigned int len) {
    
    // this return value depends on the module. check the datasheet
    unsigned char ret;
    
    last_error = xbee_connTx(
        connection,&ret, reinterpret_cast<const unsigned char *>(buf), len
    );
    
    return ret;
}
