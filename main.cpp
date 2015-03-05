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
 
#include <iostream>
#include <cstring>
#include <unistd.h>

#include <XBeeModule.h>


int main() {
    
    // max baud rate 57600, don't go higher. match AT configs.
    XBeeModule module("xbeeZB", "/dev/ttyS3", 57600);
    std::cout << module.get_last_error_str() << std::endl;
    
    // callback function. called when a packet is received in response to a
    // transmission
    auto rx_callback = [] (struct xbee *xbee,
                           struct xbee_con *con,
                           struct xbee_pkt **pkt,
                           void **data) -> void {
        
        printf("Response is %d bytes long:\n", (*pkt)->dataLen);
        
        for (auto i = 0; i < (*pkt)->dataLen; i++) {
            printf("%3d: 0x%02X\n", i, (*pkt)->data[i]);
        }
    };
    
    // convert from uint64_t to address struct?
    struct xbee_conAddress dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr)); // must
    dest_addr.addr64_enabled = 1;
    dest_addr.addr64[0] = 0x00;
    dest_addr.addr64[1] = 0x13;
    dest_addr.addr64[2] = 0xA2;
    dest_addr.addr64[3] = 0x00;
    dest_addr.addr64[4] = 0x40;
    dest_addr.addr64[5] = 0x98;
    dest_addr.addr64[6] = 0x79;
    dest_addr.addr64[7] = 0xA0;
    
    // if Local AT, second param should be nullptr.
    module.open_connection("Data", &dest_addr, rx_callback);
    std::cout << module.get_last_error_str() << std::endl;
    
    module.tx("0"); // send 0x30
    std::cout << module.get_last_error_str() << std::endl;
    
    return 0;
}
