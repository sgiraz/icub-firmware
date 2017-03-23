/*
 * Copyright (C) 2017 iCub Facility - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * website: www.robotcub.org
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _EMBOT_APP_THECANBOARDINFO_H_
#define _EMBOT_APP_THECANBOARDINFO_H_

#include "embot_common.h"

#include "embot_sys.h"


namespace embot { namespace app {
    
       
    class theCANboardInfo
    {
    public:
        static theCANboardInfo& getInstance()
        {
            static theCANboardInfo* p = new theCANboardInfo();
            return *p;
        }
                
    public:
    
        static const std::uint32_t validityKey = 0x8888dead;
        struct StoredInfo
        {   // contains the basics stored inside some embot::i2h::Storage
            std::uint32_t       key;
            std::uint8_t        canaddress;
            std::uint8_t        boardtype;
            std::uint8_t        ffu[2];
            std::uint8_t        info32[32];
            std::uint8_t        tobefilled[24];
        }; 
        
        enum class Type { none = 255, mtb3 = 33, strain2 = 22};
        
        bool erase();
        
        bool synch(Type type, std::uint8_t adr);
        
        bool get(StoredInfo &info);
        bool set(const StoredInfo &info);
        
        std::uint8_t getCANaddress();
        bool setCANaddress(std::uint8_t adr);
        
        // todo: we could use two different pages for board and userspace. but we should share the buffer ...
        static const std::uint32_t sizeOfUserSpace = 128+256+512; 
        
        // but we can read / write particular areas of max size sizeOfUserSpace. we can use it for storing matrices or ... whatever we want, as long as we dont do mess
        bool write(std::uint32_t address, std::uint32_t size, const void *data);
        bool read(std::uint32_t address, std::uint32_t size, void *data);
        
    private:
        theCANboardInfo(); 
  
    public:
        // remove copy constructors and copy assignment operators
        theCANboardInfo(const theCANboardInfo&) = delete;
        theCANboardInfo(theCANboardInfo&) = delete;
        void operator=(const theCANboardInfo&) = delete;
        void operator=(theCANboardInfo&) = delete;

    private:    
        struct Impl;
        Impl *pImpl;        
    };       


}} // namespace embot { namespace app {


#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------