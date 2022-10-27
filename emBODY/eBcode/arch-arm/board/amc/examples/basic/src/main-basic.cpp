/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 */


#include "stm32hal.h"
#include "bsp_basic.h"


// stm32hal requires two functions: one which inits and one that gets the 1 ms time.
static void stm32hal_tick1msecinit();
static uint32_t stm32hal_tick1msecget();
static const stm32hal_config_t systickcfg = {
    stm32hal_tick1msecinit,
    stm32hal_tick1msecget};

/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;
uint32_t EraseError = 0, cm4_buffer_size = 0;
unsigned char buff_sec_to_erase[1024];
unsigned char buff_sec_to_update[1024];
unsigned char data_to_write = 0x20;


unsigned char acCM4_Code[1024];

// --------------------------------------------------------------------------------------------------------------------

int main(void)
{ 
    uint32_t i;
    uint32_t Address;
    uint8_t led_failure_status = 0; // 0 no failure, 1 failure.
    
    stm32hal_init(&systickcfg);
    
    HAL_Delay(500);
    
    bsp_basic_itm_puts("hello world");
    
    bsp_basic_led_init();
    bsp_basic_itm_puts("led initted");
        
    /* Unlock the Flash to enable the flash control register access */
    HAL_FLASH_Unlock();
    
    
    for (int k = 0; k < 5; k++)
    {
        memset(buff_sec_to_erase, 0x66, 1024);
        memset(buff_sec_to_update, 0x99, 1024);
        memset(acCM4_Code, data_to_write, 1024);
        uint32_t *buffer_address = (uint32_t *)acCM4_Code;

        /* Fill EraseInit structure*/
        //  EraseInitStruct.TypeErase     = FLASH_TYPEERASE_MASSERASE;
        EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
        EraseInitStruct.Sector = FLASH_SECTOR_3;
        EraseInitStruct.NbSectors = 1;
        EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
        EraseInitStruct.Banks = FLASH_BANK_1;
        
        clearFlashFlag();

        if (HAL_FLASHEx_Erase(&EraseInitStruct, &EraseError) != HAL_OK)
        {
            /*
              Error occurred while sector erase.
              User can add here some code to deal with this error.
              EraseError will contain the faulty sector and then to know the code error on this sector,
              user can call function 'HAL_FLASH_GetError()'
            */
            led_failure_status = 1;
            Error_Handler();
        }

        // clear all flags before you write it to flash
        //  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGSERR);
        //__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGSERR);


        /* Program CM4 code */
        cm4_buffer_size = (sizeof(acCM4_Code) / (sizeof(*acCM4_Code)));

        /* Initialize Address to Bank2 start adress 0x08100000*/
        Address = 0x08060000; // FLASH_BANK1_SECTOR3;

        // check for correct erase
        for (i = 0; i < cm4_buffer_size; i++)
        {
            if (*((uint8_t *)Address) != 0xFF)
            {
                led_failure_status = 1;
                Error_Handler();
            }
            else
            {
                buff_sec_to_erase[i] = *((uint8_t *)Address);
                Address++;
            }
        }

        i = (cm4_buffer_size / 32) + 1;
        Address = 0x08060000; // FLASH_BANK1_SECTOR3;

        while (i != 0)
        {
            /* Program Flash Word (size 256 bits) */
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, Address, (uint32_t)buffer_address) == HAL_OK)
            {
                /* Increment for the next Flash word*/
                Address = Address + 32;
                buffer_address = buffer_address + 8;
                i--;
            }
            else
            {
                /* Error occurred while writing data in Flash memory.
                   User can add here some code to deal with this error */
                led_failure_status = 1;
                Error_Handler();
            }
        }

        /* Check programed code before starting the CPU2 (Cortex-M4) */
        // BSP_LED_Toggle(LED1);
        Address = 0x08060000; // FLASH_BANK2_BASE;
        for (i = 0; i < cm4_buffer_size; i++)
        {
            if (*((uint8_t *)Address) != acCM4_Code[i])
            {
                led_failure_status = 1;
                Error_Handler();
            }
            else
            {
                buff_sec_to_update[i] = *((uint8_t *)Address);
                Address++;
            }
        }

        // wait 5 seconds
        HAL_Delay(5 * 1000);

        data_to_write++;
    }

    for (;;)
    {
        HAL_Delay(500);
        bsp_basic_led_toggle();
        bsp_basic_itm_puts("led toggled");
    }    
}

// -------------------------------------------------------------------------------------------------------------------

// we need to provide stm32hal a timebase in units of 1 ms (as the hal driver by stm requires).
// we can use what we want. typically it is used the systick timer which increments a volatile
// counter. the systick is initted @ 1 khz

static volatile uint64_t s_1mstickcount = 0; // it must be volatile
constexpr uint32_t s_rate1khz = 1000;


#ifdef __cplusplus
extern "C"
{
#endif
    void SysTick_Handler(void)
    {
        s_1mstickcount++;
    }
#ifdef __cplusplus
} // closing brace for extern "C"
#endif

static void stm32hal_tick1msecinit()
{
    HAL_SYSTICK_Config(SystemCoreClock/s_rate1khz);
}

static uint32_t stm32hal_tick1msecget()
{
    return s_1mstickcount;
}


// this macro is seen through stm32hal.h
//#if defined(STM32HAL_removeWEAK_ETH)
//// others, required because they have been removed from stm32hal.h7.stm32h745disco.lib
// void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth) {}
// void HAL_ETH_PMTCallback(ETH_HandleTypeDef *heth) {}
// void HAL_ETH_TxCpltCallback(ETH_HandleTypeDef *heth) {}
//#endif

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------



