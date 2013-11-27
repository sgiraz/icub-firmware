
// - include guard ----------------------------------------------------------------------------------------------------
#ifndef _EOSPEEDMETER_HID_H_
#define _EOSPEEDMETER_HID_H_

#ifdef __cplusplus
extern "C" {
#endif

/*  @file       EOspeedmeter_hid.h
    @brief      This header file implements hidden interface to speed meter from slow encoder.
    @author     alessandro.scalzo@iit.it
    @date       19/05/2012
 **/


// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"

// - declaration of extern public interface ---------------------------------------------------------------------------

#include "EOspeedmeter.h"


// - #define used with hidden struct ----------------------------------------------------------------------------------


// - definition of the hidden struct implementing the object ----------------------------------------------------------

/** @struct     EOspeedmeter_hid
    @brief      Hidden definition. Implements private data used only internally by the
                public or private (static) functions of the object and protected data
                used also by its derived objects.
 **/

struct EOspeedmeter_hid
{
    int32_t time;

    int32_t distance;
    int32_t position_last;
    int32_t position_sure;

    eObool_t calibrated;
    int32_t enc_sign;
    int32_t offset;

    int32_t speed_filt;
    int32_t speed;
    int32_t dir;

    eObool_t is_started;
    eObool_t hard_fault;

    uint8_t  first_valid_data;
    //uint16_t invalid_data_cnt;

    int32_t window;
    int32_t distance_filt;
    int32_t distance_pred;

    int32_t delta;
    int32_t delta_filt;
};


// - declaration of extern hidden functions ---------------------------------------------------------------------------


#ifdef __cplusplus
}       // closing brace for extern "C"
#endif

#endif  // include-guard

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------