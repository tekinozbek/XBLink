/* 
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
 
// 0x0013A200409879A0 -- ROUTER
// 0x0013A200409E0550 -- COORDINATOR
 
#include <iostream>
#include <cstring>
#include <unistd.h>

#include <XBeeModule.h>
#include <XBeeMessageHandler.h>

XBeeMessageHandler* handler;

/* This function is called by libxbee when a new packet is received. The packet
 * is then passed to an XBeeMessageHandler that forms XBeeMessage objects
 * from single or multipart packets. */
void rx_handler(struct xbee* xbee,
                struct xbee_con* con,
                struct xbee_pkt** pkt,
                void** data) {
    
    handler->parse_packet(pkt);
}

int main() {
    
    //XBeeModule module("xbeeZB", "/dev/ttyS1", 57600);
    //module.open_connection("Data", 0x0013A200409E0550, rx_handler);

    return 0;
}
