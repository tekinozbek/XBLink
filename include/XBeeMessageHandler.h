/* File     : XBeeMessageHandler.h
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

#ifndef XBLINK_XBEEMESSAGEHANDLER_H
#define XBLINK_XBEEMESSAGEHANDLER_H

#include <unordered_map>
#include <cstdint>
#include <functional>

#include <XBeeMessage.h>
#include <XBeeModule.h>
#include <xbee.h>

class XBeeMessageHandler {
    
    public:
        /* CONSTRUCTOR
         *      XBeeMessageHandler
         * 
         * PARAMETERS
         *      max_payload_len Maximum length of the payload. This must be
         *                      equal on both the transmitting and the
         *                      receiving end. Must be greater than HEADER_SIZE.
         *      callback        This function is invoked when a full XBeeMessage
         *                      is received. The parameter to this function
         *                      is a pointer to an XBeeMessage, which should be
         *                      freed when done.
         */
        XBeeMessageHandler(unsigned int max_payload_len,
                           std::function<void (XBeeMessage *)> callback);
    
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
         *      pkt             The xbee_pkt that has arrived.
         */
        void parse_packet(struct xbee_pkt** pkt);
        
        /* FUNCTION
         *      send_message
         * 
         * DESCRIPTION
         *      Sends an XBeeMessage to the recipient. If the data is large,
         *      it is sent as fragments which will be reassembled by the
         *      XBeeMessageHandler on the receiving end.
         * 
         * PARAMETERS
         *      mod             The XBeeModule that will be used to transmit
         *                      the message.
         *      msg             The message that will be transmitted. If large,
         *                      it will be sent as fragments.
         * 
         * RETURN VALUES
         *      Returns true if sent successfully, false otherwise. If any
         *      fragment of the message fails to send, this function will return
         *      false. There is no re-transmission attempt since XBee modules
         *      themselves attempt several times before quitting.
         */
        bool send_message(XBeeModule& mod, XBeeMessage msg) const;

        /* STATIC CONSTANT DATA MEMBER
         *      HEADER_SIZE
         * 
         * DESCRIPTION
         *      The number of bytes used as header to each payload.
         */
        static const unsigned int HEADER_SIZE = 8;

    private:
        std::unordered_map<uint16_t, XBeeMessage *>   messages;
        std::function<void (XBeeMessage *)>           callback;
        unsigned int                                  max_payload_len;
};

#endif
