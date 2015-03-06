/* File     : XBeeModule.cpp
 * Author(s): Tekin Ozbek <tekin@tekinozbek.com>
 * 
 * 
 * XBLink - Networking between two computers using XBee modules.
 * 
 * Copyright (C) 2015 Tekin Ozbek <tekin@tekinozbek.com>,
 *                    Benjamin Yan <benjamin.yan@carleton.ca>
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
    
    /* series 1 is 100 bytes, series 2 is 72 bytes. this is required so that
     * we can send a larger payload in multiple packets. */
    max_packet_length = type == "xbeeZB" ? 72 : 100;
    
    last_error = xbee_setup(&xbee, type.c_str(), device.c_str(), baud);
}

XBeeModule::~XBeeModule() {
    
    if (xbee_connection != nullptr)
        xbee_conEnd(xbee_connection);
        
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
                                     
    /* convert address into xbee_conAddress structure */
    struct xbee_conAddress dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.addr64_enabled = 1;
    
    /* get each byte from address into the struct */
    for (auto i = 7; i >= 0; i--) {
        
        dest_addr.addr64[i] = (unsigned char)(address & 0xFF);
        address >>= 8;
    }
     
    last_error = xbee_conNew(xbee, &xbee_connection, mode.c_str(), &dest_addr);
    
    if (last_error == XBEE_ENONE)
        xbee_conCallbackSet(xbee_connection, callback, nullptr);
}

void XBeeModule::tx(XBeeMessage& msg) {
    
    // TODO: write this
}
