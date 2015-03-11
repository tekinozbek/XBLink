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

#include <functional>
#include <unordered_map>
#include <cmath>
#include <cstring>
#include <iostream>
#include <fstream>

#include <XBeeMessageHandler.h>
#include <XBeeMessage.h>
#include <XBeeModule.h>

XBeeMessageHandler::XBeeMessageHandler(
    unsigned int max_payload_len,
    std::function<void (XBeeMessage *)> callback
) : callback(callback) {
    
    if (max_payload_len <= HEADER_SIZE)
        throw std::invalid_argument("Maximum payload length too small.");
        
    this->max_payload_len = max_payload_len;
}

void XBeeMessageHandler::parse_packet(struct xbee_pkt** pkt) {
    
    // technically, a proper message should be at least header size
    if ((*pkt)->dataLen < HEADER_SIZE)
        return;
        
    // determine the usable payload length
    unsigned int usable_payload_len = max_payload_len - HEADER_SIZE;
    
    /* there is a design issue here. we either send 4 bytes total length with
     * every payload or 2 bytes of number of fragments. if we send number of
     * fragments, if the last fragment is not full, the recipient wastes at most
     * usable_payload_len - 1 space.
     */
    
    // extract headers from the packet    
    uint16_t sequence_id;
    uint16_t num_fragments;
    uint16_t fragment;
    
    memcpy(&sequence_id, (*pkt)->data, 2);
    memcpy(&num_fragments, (*pkt)->data + 2, 2);
    memcpy(&fragment, (*pkt)->data + 4, 2);
    
    auto search = messages.find(sequence_id);
    
    XBeeMessage* msg;
    
    /* we check if a part of this sequence has already arrived */
    if (search != messages.end())
        msg = search->second;
    
    /* nope, this is a new sequence, add it to the map */
    else
        messages[sequence_id] = msg = new XBeeMessage(
            num_fragments * usable_payload_len
        );
    
    // write the data that has arrived
    msg->write(
        reinterpret_cast<const char *>((*pkt)->data) + HEADER_SIZE,
        fragment * usable_payload_len,
        (*pkt)->dataLen - HEADER_SIZE
    );
    
    if (msg->is_complete() && callback != nullptr) {
        
        callback(msg);
        
        std::cout << "Received sequence [" << std::hex
                  << sequence_id << "]" << std::dec << std::endl;
                  
        messages.erase(sequence_id);
    }
    
    else if (callback == nullptr)
        delete msg;
}

bool XBeeMessageHandler::send_message(XBeeModule& mod, XBeeMessage msg) const {
    
    // determine the usable payload length
    unsigned int usable_payload_len = max_payload_len - HEADER_SIZE;
    
    /* there can only be 65536 fragments, hence a message with greater fragment
     * size cannot be transmitted. */
    if (msg.get_curr_length() > 65536 * usable_payload_len)
        return false;
    
    // determine number of fragments required for the message
    uint16_t num_fragments = ceil(
        static_cast<double>(msg.get_curr_length()) / 
        static_cast<double>(usable_payload_len)
    );
    
    /* the message as a whole is a sequence. so we randomly generate a two byte
     * sequence id. each fragment will have the same sequence id so that the
     * receiver can identify which fragment belongs to which message. */
    uint16_t sequence_id;
    
    std::ifstream frand("/dev/urandom", std::ios::in | std::ios::binary);
    frand.read((char *)&sequence_id, sizeof(sequence_id));
    frand.close();
    
    /* transmit each fragment one by one with the following headers:
     * 
     * | id (2 bytes) | num fragments (2 bytes) | offset (2 bytes) | ... |
     * 
     * if any fragment fails, the function does not attempt to re-transmit and
     * returns false. XBee modules themselves re-try transmission upon failure.
     */
    const char* msg_buf = static_cast<const char *>(msg.get_buffer());
    char* fragment      = new char[max_payload_len];
    uint32_t remaining  = msg.get_curr_length(); // remaining to be sent
    
    std::cout << "Sending sequence ["
              << std::hex << sequence_id << "]" << std::dec << std::endl;
    
    for (uint16_t i = 0; i < num_fragments; ++i) {
         
        // insert sequence id, number of fragments and fragment offset
        memcpy(fragment, &sequence_id, 2);
        memcpy(fragment + 2, &num_fragments, 2);
        memcpy(fragment + 4, &i, 2);
        
        // this is the length of the data we're sending after the headers
        int data_len = remaining > usable_payload_len ?
                                      usable_payload_len : remaining;
                                    
        memcpy(
            fragment + HEADER_SIZE,
            msg_buf + (i * usable_payload_len),
            data_len
        );
        
        // send the fragment, return false if failed
        if (mod.tx(fragment, data_len + HEADER_SIZE) != 0) {
            
            std::cout << "    - Fragment " << static_cast<int>(i + 1)
                      << " failed" << std::endl;
            
            delete fragment;
            return false;
        }
        
        std::cout << "    + Fragment " << static_cast<int>(i + 1)
                  << " sent" << std::endl;
        
        remaining -= data_len;
    }
     
    delete fragment;
    
    return true;
}
