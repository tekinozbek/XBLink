/* 
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
 
// 0x0013A200409879A0 -- ROUTER
// 0x0013A200409E0550 -- COORDINATOR
 
#include <iostream>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <cstdint>
#include <memory>

#include <XBeeModule.h>
#include <XBeeMessageHandler.h>
#include <XBeeMessage.h>

#include <linux/if_tun.h>
#include <net/if.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>

/* from linux kernel docs */
int tun_alloc(char *dev)
{
  struct ifreq ifr;
  int fd, err;

  if( (fd = open("/dev/net/tun", O_RDWR)) < 0 )
     return -1;

  memset(&ifr, 0, sizeof(ifr));

  /* Flags: IFF_TUN   - TUN device (no Ethernet headers) 
   *        IFF_TAP   - TAP device  
   *
   *        IFF_NO_PI - Do not provide packet information  
   */ 
  ifr.ifr_flags = IFF_TUN | IFF_NO_PI; 
  if( *dev )
     strncpy(ifr.ifr_name, dev, IFNAMSIZ);

  if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
     close(fd);
     return -2;
  }

  return fd;
}  

using namespace std;

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

int main(int argc, char* argv[]) {
    
    /* argv[ ]
     *      1:  path to device (e.g. /dev/ttyS1, /dev/ttyUSB0, etc.)
     *      2:  destination address as 64 bit hexadecimal
     *      3:  TUN device name (tun23, tun88, whatever)
     *      4:  ip address in CIDR format (10.0.0.2/24)
     */
    if (argc < 4) {
        
        cout << "usage: " << argv[0] << " [device] [address] [tun]" << endl;
        return -1;
    }
    
    /* open a tun device */
    int fd = tun_alloc(argv[3]);
    
    /* set up connection to module and open connection with remote device */
    XBeeModule module("xbeeZB", argv[1], 9600);
    module.open_connection(
        "Data",
        (uint64_t)std::stoull(argv[2], nullptr, 16),
        rx_handler
    );
    
    /* set up the message handler */
    handler = new XBeeMessageHandler(72, [&fd] (XBeeMessage* msg) -> void {
        
        write(fd, msg->get_buffer(), msg->get_curr_length());
        delete msg;
    });
    
    /* read from the tun device */
    char buf[1500];
    int read_size = 0;
    
    while ((read_size = read(fd, buf, 1500)) >= 0) {
    
        XBeeMessage msg(read_size);
        msg.write(buf, 0, read_size);
        handler->send_message(module, msg);
    }

    return 0;
}
