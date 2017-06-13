/**
  CCP5 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    ccp5.c

  @Summary
    This is the generated driver implementation file for the CCP5 driver using MPLAB(c) Code Configurator

  @Description
    This source file provides implementations for driver APIs for CCP5.
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

/**
  Section: Included Files
*/

#include <p18cxxx.h>
#include "pwm5.h"

/**
  Section: Macro Declarations
*/

#define PWM5_INITIALIZE_DUTY_VALUE    511

/**
  Section: PWM Module APIs
*/

void PWM5_Initialize(void)
{
    // Set the PWM to the options selected in the MPLAB(c) Code Configurator

    // CCP5M PWM; DC5B 3; 
    CCP5CON = 0x3C;
    
    // CCPR5L 127; 
    CCPR5L = 0x7F;
    
    // CCPR5H 0; 
    CCPR5H = 0x00;
    
    // Selecting Timer 4
    CCPTMRSbits.C5TSEL = 0x1;
}

void PWM5_LoadDutyValue(uint16_t dutyValue)
{
   // Writing to 8 MSBs of pwm duty cycle in CCPRL register
    CCPR5L = ((dutyValue & 0x03FC)>>2);
    
   // Writing to 2 LSBs of pwm duty cycle in CCPCON register
    CCP5CON = (CCP5CON & 0xCF) | ((dutyValue & 0x0003)<<4);
}

/**
 End of File
*/

