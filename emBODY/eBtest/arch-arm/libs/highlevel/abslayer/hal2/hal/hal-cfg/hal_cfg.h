/*
 * Copyright (C) 2011 Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
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
#ifndef _HAL_CFG_H_
#define _HAL_CFG_H_

// --------------------------------------------------------------------------------------------------------------------
//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------

// <h> Configuration of HAL
// <i> It holds configuration for objects used in OSAl


// <h> Target 
// <i> Define the target 

//   <o> CPU family         <0=>   Cortex M3  <1=>   Cortex M4
//   <i> Only Cortex M3 / M4 are now supported.
#ifndef HAL_CPUFAM
 #define HAL_CPUFAM      1
#endif

//   <o> CPU type  <0x00=>   STM32F1x   <0x01=>   STM32F4x
//   <i> Only STM32F1x / STM32F4x are now supported.
#ifndef HAL_CPUTYPE
 #define HAL_CPUTYPE       0x01
#endif


//   <o> CPU frequency  <72000000=>   72 MHz    <168000000=> 168 MHz
//   <i> Only 72 / 168 MHz are supported for now.
#ifndef HAL_CPUFREQ
 #define HAL_CPUFREQ       168000000
#endif

// </h>Target


// <h> System 
// <i> sssssssss

//   <o> stack size         <0x0-0xFFFFFFFF:8>
//   <i> define how much stack you want.
#ifndef HAL_SYS_STACKSIZE
 #define HAL_SYS_STACKSIZE      0x00001000
#endif

//   <o> heap size         <0x0-0xFFFFFFFF:8>
//   <i> define how much heap you want.
#ifndef HAL_SYS_HEAPSIZE
 #define HAL_SYS_HEAPSIZE      0x00008000
#endif


// </h>System

// <h> GPIO 
// <i> sssssssss

//   <h> Nothing to configure in GPIO part
//   </h>Nothing to configure in GPIO part 


// </h>GPIO


// <h> FLASH 
// <i> sssssssss

//   <h> Nothing to configure in FLASH part
//   </h>Nothing to configure in FLASH part 


// </h>FLASH


// <h> Display 
// <i> it si important disable display on ems001, because there isn't

//   <o> SPI Display 320x240  <0=>   Disable  <1=> Enable
//   <i> only SPI is supported for now.
#ifndef HAL_SPIDISPLAY_ENABLE
 #define HAL_SPIDISPLAY_ENABLE  0     
#endif


// </h>Display


// <e> Ethernet 

#define HAL_ETH_ON      1

// <o> ETH mode         <1=> ETH w/ ISR + DMA 
// <i> sssssssss

#ifndef HAL_ETH_MODE
 #define HAL_ETH_MODE  1     
#endif

#if (0 == HAL_ETH_ON)
 #define HAL_ETH_ENABLE  0     
#else
 #define HAL_ETH_ENABLE  HAL_ETH_MODE
#endif

//   <o> Size of DMA TX buffers  <1=> One frame  <2=> Two frames <3=> Three frames <4=> Four frames
//   <i> One ETH frame is 1536 bytes
#ifndef HAL_ETH_DMA_TX_BUF
 #define HAL_ETH_DMA_TX_BUF  3     
#endif


//   <o> Size of DMA RX buffers  <1=> One frame  <2=> Two frames <3=> Three frames <4=> Four frames
//   <i> One ETH frame is 1536 bytes
#ifndef HAL_ETH_DMA_RX_BUF
 #define HAL_ETH_DMA_RX_BUF  2     
#endif

// </e>Ethernet


// <e> CAN 1          
// <i> sssssssss

#ifndef HAL_CAN1_ENABLE
 #define HAL_CAN1_ENABLE  1     
#endif


//   <o> Max can frames in input buffer <1-16>
//   <i> Default: 4
#define HAL_CAN1_INPBUFFCAPACITY     10

//   <o> Max can frames in output buffer <1-16>
//   <i> Default: 4
#define HAL_CAN1_OUTBUFFCAPACITY     16


// </e> CAN 0  



// <e> CAN 2          
// <i> sssssssss

#ifndef HAL_CAN2_ENABLE
 #define HAL_CAN2_ENABLE  1     
#endif


//   <o> Max can frames in input buffer <1-16>
//   <i> Default: 4
#define HAL_CAN2_INPBUFFCAPACITY     10

//   <o> Max can frames in output buffer <1-16>
//   <i> Default: 4
#define HAL_CAN2_OUTBUFFCAPACITY     10


// </e> CAN 2  


// <e> SPI 1          
// <i> SPI1 peripheral

#ifndef HAL_SPI1_ENABLE
 #define HAL_SPI1_ENABLE  0     
#endif

//   <h> Nothing to configure in SPI1 part
//   </h>Nothing to configure in SPI1 part 

// </e> SPI 1    


// <e> SPI 2          
// <i> SPI2 peripheral

#ifndef HAL_SPI2_ENABLE
 #define HAL_SPI2_ENABLE  0     
#endif

//   <h> Nothing to configure in SPI2 part
//   </h>Nothing to configure in SPI2 part 

// </e> SPI 2    


// <e> SPI 3          
// <i> SPI3 peripheral

#ifndef HAL_SPI3_ENABLE
 #define HAL_SPI3_ENABLE  0     
#endif

//   <h> Nothing to configure in SPI3 part
//   </h>Nothing to configure in SPI3 part 

// </e> SPI 3    


// <h> Timers          
// <i> Timer peripheral

//   <o> Max number of timers <0-7>
//   <i> Default: 2
#define HAL_TIMERS_NUMBER     4

// </h> Timers    


// </h>






// --------------------------------------------------------------------------------------------------------------------
//------------- <<< end of configuration section >>> ------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------



#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

