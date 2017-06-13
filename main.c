/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using MPLAB(c) Code Configurator

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15.1
        Device            :  PIC18F26K80
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include "mcc_generated_files/mcc.h"
#include "interrupt_manager.h"
#include "i2c.h"



 I2C_MESSAGE_STATUS status = I2C_MESSAGE_PENDING;
 uint8_t    writeBuffer[3]= {0x55,0xCA,0xFE};
 uint16_t   slaveDeviceAddress = 0x57;

 uint8_t command = 0x12;
 volatile uint16_t delay = 100;
/*
                         Main application
 */
 
 /**
 ** Reads I2C Register from sensor
 **
 ** \details
 **
 ** \return    Nothing
 **************************************************************************************/
I2C_MESSAGE_STATUS
SensorRead (
            uint8_t  regadd,             /**< Register Address  */
            uint8_t  *data,              /**< Data buffer  */
            uint8_t     n_bytes);            /**< Number of bytes to read */

uint8_t sensor_data = 0;
void InterruptHandlerHigh (void);
void main(void)
{
    int idx;
    // Initialize the device
    SYSTEM_Initialize();

  //  TMR0_SetInterruptHandler(&TMR0_ToggleLED);
    // If using interrupts in PIC18 High/Low Priority Mode you need to enabl\e the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable high priority global interrupts
    //INTERRUPT_GlobalInterruptHighEnable();

    // Enable low priority global interrupts.
    //INTERRUPT_GlobalInterruptLowEnable();

    // Disable high priority global interrupts
    //INTERRUPT_GlobalInterruptHighDisable();

    // Disable low priority global interrupts.
    //INTERRUPT_GlobalInterruptLowDisable();

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();
    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    // write one byte to EEPROM (3 is the number of bytes to write)
    CEN_SetLow();
    do
    {
        delay--;
    }while(delay > 0);

      SensorRead (0x00,  &sensor_data, 1);
    

    while (1)
    {
        // Add your application code
    }
}




/**
 ** Reads I2C Register from sensor
 **
 ** \details
 **
 ** \return    Nothing
 **************************************************************************************/
I2C_MESSAGE_STATUS
SensorRead (
            uint8_t  regadd,             /**< Register Address  */
            uint8_t  *data,              /**< Data buffer  */
            uint8_t  n_bytes)            /**< Number of bytes to read */
{

  uint8_t RegAddr;
  RegAddr = regadd;
      I2C_MasterWrite(&RegAddr,n_bytes,0x57,&status);

      I2C_MasterRead(data,n_bytes,0x57,&status);

  return status;
}

  




//----------------------------------------------------------------------------
// High priority interrupt routine

#pragma code InterruptHandlerHigh = 0x08
#pragma interrupt InterruptHandlerHigh

void
InterruptHandlerHigh ()
{
INTERRUPT_InterruptManager();
STA_LED_Toggle();
}




/**
 End of File
*/