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

#ifndef XBLINK_XBEEMESSAGE_H
#define XBLINK_XBEEMESSAGE_H

#include <memory>

class XBeeMessage {
    
    public:
        /* CONSTRUCTOR
         *      XBeeMessage
         * 
         * DESCRIPTION
         *      Allocates length for the buffer.
         * 
         * PARAMETERS
         *      length      Number of expected bytes for this message. Memory will
         *                  be allocated for this amount.
         */
        XBeeMessage(uint32_t length);
        
        XBeeMessage(
        
        /* FUNCTION
         *      write
         * 
         * DESCRIPTION
         *      Writes 'n' data starting from position 'pos'.
         * 
         * PARAMETERS
         *      buf     The buffer that the data will be taken from.
         *      pos     The position in the message where writing will begin.
         *      n       Number of bytes to copy from buf to this message.
         */
        void write(const char* buf, uint32_t pos, uint32_t n);
        
        /* FUNCTION
         *      get_buffer
         * 
         * RETURN VALUE
         *      Returns pointer to the buffer.
         */
        const char* get_buffer() const;
        
        /* FUNCTION
         *      get_length
         * 
         * RETURN VALUE
         *      Returns the expected (allocated) length of the message.
         */
        uint32_t get_length() const;
        
        /* FUNCTION
         *      get_curr_length
         * 
         * RETURN VALUE
         *      Returns the number of bytes written to the buffer.
         */
        uint32_t get_curr_length() const;
        
        /* FUNCTION
         *      is_complete
         * 
         * RETURN VALUE
         *      Returns true if message is complete, false otherwise.
         */
        bool is_complete() const;
        
        XBeeMessage() = delete;

    private:
        std::shared_ptr<char>   buffer;
        uint32_t                length;
        uint32_t                curr_length;
    
};

#endif
