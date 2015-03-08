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
 
#include <cstring>
#include <cstdint>
 
#include <XBeeMessage.h>

XBeeMessage::XBeeMessage(uint32_t length)
    : buffer(new char[length]), length(length), curr_length(0) { }
    
void XBeeMessage::write(const char* buf, uint32_t pos, uint32_t n) {
    
    memmove(buffer.get() + pos, buf, n);
    
    // increase current length by n bytes
    curr_length += n;
}

const char* XBeeMessage::get_buffer() const {
    
    return buffer.get();
}

uint32_t XBeeMessage::get_length() const {
    
    return length;
}

uint32_t XBeeMessage::get_curr_length() const {
    
    return curr_length;
}

bool XBeeMessage::is_complete() const {
    
    return curr_length == length;
}
