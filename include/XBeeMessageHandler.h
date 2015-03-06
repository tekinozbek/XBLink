/* File     : XBeeMessageHandler.h
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

#ifndef XBLINK_XBEEMESSAGEHANDLER_H
#define XBLINK_XBEEMESSAGEHANDLER_H

#include <map>
#include <cstdint>

#include <XBeeMessage.h>
#include <xbee.h>

class XBeeMessageHandler {
    
    public:
        /* CONSTRUCTOR
         *      XBeeMessageHandler
         * 
         * PARAMETERS
         *      callback    This function is invoked when a full XBeeMessage
         *                  is received.
         */
        XBeeMessageHandler(std::function<void(XBeeMessage)> callback);
    
        /* FUNCTION
         *      parse_packet
         * 
         * DESCRIPTION
         *      Parses a packet into an XBeeMessage. Once complete, it will
         *      invoke the callback function.
         * 
         *      If receiving a multipart message, the callback function is not
         *      called until the entirety of message is received.
         * 
         * PARAMETERS
         *      pkt         The xbee_pkt that has arrived.
         */
        void parse_packet(struct xbee_pkt** pkt);
        
        XBeeMessageHandler() = delete;

    private:
        std::map<uint16_t, XBeeMessage>     messages;
        std::function<void(XBeeMessage)>    callback;
    
};

#endif
