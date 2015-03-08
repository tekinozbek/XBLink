# XBLink
XBLink is a program that allows for networking between two computers using XBee
modules. The aim is to send IP packets through TUN devices to create a network.

## Requirements
* Two XBee modules configured properly (see below).
* Linux with access to /dev/net/tun and /dev/urandom (you will also need super
user permissions for `ip` commands).
* g++ >= 4.7 for C++11 (modify makefile if you are using some other compiler).
* [libxbee](https://code.google.com/p/libxbee/) installed in a place where g++
can find it.

## XBee Configuration
Configure your modules using [X-CTU](http://www.digi.com/support/productdetail?pid=3352&type=utilities)
by Digi.

* Recommended baud rate <= 57600.
* Hardware flow control (RTS *and* CTS).
* Both modules should be in API mode. 
* API output mode should be *native*.

**WARNING:** If using XBee Series 2 (ZigBee) modules, one of the modules should
be the *coordinator* and the other one can be either an *end device* or a
*router* (watch out, end devices fall asleep, best to use router mode). Both
modules should have the correct PAN ID set.

## Using XBLink
Run `make` and then execute `xblink` with the arguments explained below for both
computers.

```
xblink [device path] [type] [baud] [dest address] [tun device]
```

* **Device path:** The path to the XBee module (e.g. /dev/ttyS1, /dev/ttyUSB0, etc.).
* **Type:** *xbee1*, *xbee2*, *xbee3*, *xbee5*, *xbee6b* or *xbeeZB* (depending
on which one you have).
* **Baud:** The baud rate you configured for your module.
* **Destination address:** The 64-bit address of the recipient module.
* **TUN device:** Some name for the TUN device, like *tun77*.

Once XBLink is running without errors, you must set the TUN device up and assign
IP addresses for both computers.

1. `ip link set {TUN} up`
2. `ip addr add {IP} dev {TUN}`

where `{TUN}` is the name you gave to your TUN device (like *tun77*) and `{IP}`
is the IP in CIDR format (e.g. 10.0.0.1/24 for first computer and 10.0.0.2/24
for the second one).

## Design details
For the *module type* and *connection mode*, check out the
[libxbee manual](http://attie.github.io/libxbee3/man3/xbee_setup.3.html).

* **XBeeModule:** This class handles some basic module operations such as setup,
connections, transmissions, etc. You must provide the module type, the device
file and baud rate. When opening a connection, you need the connection type and
the destination address, as well as a callback function that handles received
packets. The baud rate and connection mode depends on your configuration.
* **XBeeMessage:** Holds an entire message (not fragments). You should write to
and read from these objects.
* **XBeeMessageHandler:** This is the class that handles splitting and
reassembling fragments. In the usual setup, the callback function, which was
passed to the module when opening a connection, should relay the received
packets to the `XBeeMessageHandler::parse_packet`. Then, when a sequence is
complete, `XBeeMessageHandler` will invoke a callback function passing in the
`XBeeMessage` that was received.

#### Data transmission
Some definitions:
* **Packet:** An XBee packet sent and received by the modules.
* **Payload:** The payload that we send inside XBee packets.
* **Message:** Some data of arbitrary size, which we send as one or multiple
fragments in a payload.

There is a limit for the payload size, which is 100 (XBee Series 1) or 72 (XBee
Series 2) bytes. To transmit data larger than this value, we need to split a
message into fragments. The maximum payload length for XBeeMessageHandler can
be arbitrary, but should not exceed the actual maximum payload length of the
modules.

There is no guarantee that the packets will arrive in the order that they were
sent, so we have to keep track of fragments that belong together. For this, we
use a randomly generated 2-byte sequence ID (one per message) so that we can
reassemble the fragments on the receiving end. We also send the total length of
the message in every fragment so that the receiver upon receiving a fragment can
allocate enough space for the entire message. Finally, we add the offset of the
fragment (0 for first fragment, 2 for second, et cetera).

```
| id (2 bytes) | length (4 bytes) | offset (2 bytes) | ... (usable payload length) |
```

The usable payload length is maximum payload length - 8. There can be maximum
65,536 fragments for a sequence and hence maximum size for an XBeeMessage is
usable payload length * 65,536.

## Credits
[libxbee](https://code.google.com/p/libxbee/) is licensed under LGPL.

[tun_alloc](https://www.kernel.org/doc/Documentation/networking/tuntap.txt) from
the Linux kernel documentations.

## License
Copyright (C) 2015 Tekin Ozbek, Benjamin Yan

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA 02110-1301 USA.
