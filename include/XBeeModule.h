/* File     : XBeeModule.h
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
 
#ifndef XBLINK_XBEEMODULE_H
#define XBLINK_XBEEMODULE_H

#include <string>
#include <cstdint>

#include <XBeeMessage.h>
#include <xbee.h>

typedef void (*rx_handler_f)(struct xbee *,
                             struct xbee_con *,
                             struct xbee_pkt **,
                             void **);

class XBeeModule {
    
    public:
        /* CONSTRUCTOR
         *      XBeeModule
         * 
         * DESCRIPTION
         *      Sets up an XBee module for connection.
         * 
         * PARAMETERS
         *      type        Type of module (xbee1, xbeeZB, etc.).
         *      block       The path to the devide (/dev/ttyUSB0, /dev/ttyS1, etc.).
         *      baud        The baud rate of the module.
         */
        XBeeModule(std::string type,
                   std::string device,
                   uint32_t baud);
                   
        /* DESTRUCTOR
         *      ~XBeeModule
         * 
         * DESCRIPTION
         *      Releases all allocated memory.
         * 
         * PARAMETERS
         *      type        Type of module (xbee1, xbeeZB, etc.).
         *      block       The path to the devide (/dev/ttyUSB0, /dev/ttyS1, etc.).
         *      baud        The baud rate of the module.
         */
        ~XBeeModule();
        
        /* FUNCTION
         *      open_connection
         * 
         * DESCRIPTION
         *      Opens a connection with the destination address. If Local AT mode,
         *      then address can be a nullptr.
         * 
         * PARAMETERS
         *      mode        The connection mode.
         *      address     Destination module address (0x0013A2... 8 bytes).
         *      callback    This callback function is invoked when a packet is
         *                  received.
         */
        void open_connection(std::string mode,
                             uint64_t address,
                             rx_handler_f callback);
        
        /* FUNCTION
         *      tx
         * 
         * DESCRIPTION
         *      Transmits data to the recipient module.
         * 
         * PARAMETERS
         *      msg     The message as XBeeMessage object.
         */
        void tx(XBeeMessage& msg);
        
        /* FUNCTION
         *      get_last_error
         * 
         * RETURN VALUE
         *      Returns the last encountered error xbee_err enum.
         */
        xbee_err get_last_error() const;
        
        /* FUNCTION
         *      get_last_error_str()
         * 
         * RETURN VALUE
         *      Returns the last encountered error as human-readable string.
         */
        std::string get_last_error_str() const;
        
        XBeeModule() = delete; // remove default constructor
        
    private:
        struct xbee*        xbee = nullptr;
        struct xbee_con*    xbee_connection = nullptr;
        xbee_err            last_error;
        uint8_t             max_packet_length;
     
};
 
#endif
