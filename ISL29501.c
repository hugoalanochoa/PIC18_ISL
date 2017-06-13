/**
  ******************************************************************************
  * @file    ISL29501.c
  * @author  ochoaha
  * @date    May 22, 2017
  * @brief   This file contains the API function for the intersil sensor ISL29501
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 MSD Performance </center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of MSD Performance nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************  
  */

/*
**====================================================================================
** Imported definitions
**====================================================================================
*/
#include "ISL29501.h"
#include <math.h>
#include <stdlib.h>
#include "i2c.h"
#include "util.h"
/*
**====================================================================================
** Public constant definitions
**====================================================================================
*/

#define REF_DIST 0.500f                 /**<Calibration distance in meters*/
#define M_PI_F 3.14159265358979323846f
#define DEFAULT_CAL_DIST_VALUES  1      /**< If set , Distance calibration values are loaded with default values */

/*
**====================================================================================
** Private type definitions
**====================================================================================
*/

typedef struct
{
  uint8_t regadd;       /**< Register address */
  uint8_t data;

}SnrCmd_T;




/*
**====================================================================================
** Private constant definitions
**====================================================================================
*/

const SnrCmd_T SnrInitSeq [] =
    {
        {0x10, 0x06},    /**< Optional - Set integration time increase over default is normal */
        {0x11, 0x34},    /**< Optional - Set measurement period, default normally OK */
        {0x13, 0x7D},
        {0x60, 0x01},
        {0x18, 0x12},
        {0x19, 0x22},
        {0x90, 0x06},
        {0x91, 0xFA},
        {0x24, 0x45},   /**< Crosstalk registers Begin*/
        {0x25, 0x66},
        {0x26, 0xCF},
        {0x27, 0x48},
        {0x28, 0x42},
        {0x29, 0x64},
        {0x2A, 0xFF},
        {0x2B, 0x00},
        {0x2C, 0x07},
        {0x2D, 0xC9},
        {0x2E, 0xB2},   /**< Crosstalk registers End */







        {0x31, 0x5B},  /**< Ambient and Temperature Begin */
        {0x32, 0x00},
        {0x33, 0x06},
        {0x34, 0x26},
        {0x35, 0x00},
        {0x36, 0x45},
        {0x37, 0x00},
        {0x38, 0x00},
        {0x39, 0x30},
        {0x3A, 0x00},
        {0x3B, 0xD3},   /**< Ambient and Temperature End  */

        {0x2F,0x0B},
        {0x30,0xA7},

        {0x60, 0x01}     /**< INT enable */


    };

#if SENSOR_READ_ALL == 1
uint8_t sensor_regs[100];
#endif



/*
**====================================================================================
** Private function prototypes
**====================================================================================
*/

double Log2( double n );





/**
** Reads a single sensor register
**
** \details
**
** \return    Data read into the given register
**************************************************************************************/
uint8_t
read_reg (
          uint8_t reg);   /**< Sensor Register to read */

/**
** Writes a sigle sensor register
**
** \details
**
** \return    Nothing
**************************************************************************************/
void
write_reg (
           uint8_t reg,         /**< register to write */
           uint8_t data);       /**< data to write */



#if SENSOR_READ_ALL == 1
/**
** Reads registers from 0x00 to 0x64
**
** \details
**
** \return    Nothing
**************************************************************************************/


void
SensorReadAll (
               void)
{
  uint8_t idx;

  for (idx = 0; idx < COUNT_OF(sensor_regs); ++idx)
    {
      sensor_regs[idx] = read_reg (idx);

    }

}
#endif


/*
**====================================================================================
** Private variable declarations
**====================================================================================
*/




/**
 ** Reads I2C Register from sensor
 **
 ** \details
 **
 ** \return    Nothing
 **************************************************************************************/
int
SensorRead (
            uint8_t  regadd,             /**< Register Address  */
            uint8_t  *data,              /**< Data buffer  */
            uint8_t     n_bytes)            /**< Number of bytes to read */
{

  I2C_MESSAGE_STATUS status;
  uint8_t RegAddr;
  RegAddr = regadd;
  do
    {
    //  status = HAL_I2C_Master_Transmit (&XNUCLEO53L0A1_hi2c, ISL29501_I2C_ADDRESS, &RegAddr, 1, 100);
      I2C_MasterWrite(&RegAddr,1,ISL29501_I2C_ADDRESS,&status);
//      if (status)
//        break;
    //  status = HAL_I2C_Master_Receive (&XNUCLEO53L0A1_hi2c, ISL29501_I2C_ADDRESS, data, n_bytes, n_bytes * 100);
      I2C_MasterRead(data,1u,ISL29501_I2C_ADDRESS,&status); 
  
  }
  while (0);

  return ((int)status);
}

/**
** Writes Sensor Registers
**
** \details
**
** \return    Nothing
**************************************************************************************/

int
SensorWrite (
             uint8_t regadd, /**< Register Address  */
             uint8_t *data, /**< Data buffer  */
             uint8_t n_bytes) /**< Number of bytes to write */ 
{
    uint8_t idx;
    I2C_MESSAGE_STATUS status;
    uint8_t data_buffer[10];
    data_buffer[0] = regadd; /*register address is the first byte*/
    for (idx = 1; idx < n_bytes; idx++) 
    {
        data_buffer[idx] = data[idx - 1u];
    }
    //  status = HAL_I2C_Master_Transmit (&XNUCLEO53L0A1_hi2c, ISL29501_I2C_ADDRESS, data, n_bytes + 1, 100);
    I2C_MasterWrite( &data_buffer[0], n_bytes + 1,ISL29501_I2C_ADDRESS,&status);
    return status;
}



/**
 ** Initializes sensor
 **
 ** \details
 **
 ** \return    Nothing
 **************************************************************************************/
int
SensorInit (
            void)
{

  uint8_t idx;
  I2C_MESSAGE_STATUS status;
  uint8_t data[2] =  { 0u, 0u };

/*  signal start stop */
  ISL_SS(1u);
  HAL_Delay(10);

  for (idx = 0; idx < (sizeof(SnrInitSeq) / sizeof(SnrCmd_T)); ++idx)
    {
      data[0] = SnrInitSeq[idx].regadd;
      data[1] = SnrInitSeq[idx].data;
   //   status = HAL_I2C_Master_Transmit (&XNUCLEO53L0A1_hi2c, ISL29501_I2C_ADDRESS, data, 2u, 100);
      I2C_MasterWrite(data,2u,ISL29501_I2C_ADDRESS,&status);
    }




return (int)status;
}

/**
** Reads register 0x00 , Device ID, if return value is 0x0A sensor is detected
**
** \details
**
** \return    true if sensor is detected
**************************************************************************************/
uint8_t
SensorDetected (
                void)
{
  uint8_t sensor_detected = 0u;
  if (read_reg (0x00) == 0x0Au)
    {
      sensor_detected = 1u;
    }
  return (sensor_detected);
}
/*
** Takes a distance measurement from sensor
**
** \details
**
** \return    Nothing
**************************************************************************************/
uint32_t
SensorMeasure (
               void)
{
  uint16_t timeout = 0xFFFFu;
  uint8_t data[2] =
    { 0u, 0u };
  uint32_t distance = 0u;
  float distance_read = 0.0;

  SensorRead (0x69, &data[0], 1u);
  /*signal start*/
  ISL_SS(0u);

  while (timeout > 0u)
    {

      if (GET_IRQ() == 0u)
        {
          break;
        }
    }

  SensorRead (0xD1, &data[0], 1);
  SensorRead (0xD2, &data[1], 1);


  distance = (uint32_t) (((uint32_t) (data[0]) << 8) | (uint32_t) (data[1]));

  distance_read = ((distance * 33.33) / 65536) * 1000;

  if (distance_read < 0.0)
    {
      distance_read = 0.0;
    }

//  distance = ((uint32_t) (((data[0] << 8) + (data[1])))) * (uint32_t) 3333;
//
//  distance *= (uint32_t) 10;
//  distance /= (uint32_t) 65536;

  ISL_SS(1u);

  return (uint32_t) distance_read;

}


uint32_t SensorMeasureF (void)
{
  float distance;
  uint16_t data_msb, data_lsb, prec;
  uint16_t offset;
  uint8_t inter, data_invalid;
  float phase;

    //setup for reading sensor
    write_reg(0xB0, 0x49);


    //read data ready flag
    inter = read_reg(0x69);

    //read sensor offset
    offset = read_reg(0x30);
    offset |= (read_reg(0x2f) << 8);

    //read data validity
    data_invalid = read_reg(0xd0);

    //read corrected data
    data_lsb = read_reg(0xd2);
    data_msb = read_reg(0xd1);




    //convert distance data to meters
    distance = ((data_msb << 8) + data_lsb) / 2000.0f;
    //correct distance from errors due to phase-distance inconsistency
    phase = (distance / 33.3f) * M_PI_F * 2.0f;
    distance += (1.1637f * phase * phase * phase - 3.8654f * phase * phase + 1.3796f * phase - 0.0436f);
    if(distance > 3)
      {
        distance = 0;
      }

    distance*=1000;
    return (uint32_t)(distance);
}



/**
** Magnitude Calibration Procedure
**
** \details
**
** \return    Nothing
**************************************************************************************/
void
SensorMagitudeCal (
                   void)
{
  uint8_t data;
  uint16_t timeout = 0xFFFFu;
  data = 0x61;
//  SensorWrite (0x13, &data, 1u); //Setup single shot mode
//  data = 0x01;
//  SensorWrite (0x60, &data, 1u); //INT Enable
  SensorWrite (0x69, &data, 1u); //Clear flag
  /*signal start*/
  ISL_SS(0u);

  while (timeout > 0u)
    {
      timeout--;
      if (GET_IRQ() == 0u)
        {
          break;
        }
    }
  ISL_SS(1u);

  SensorRead (0xF6,  &data, 1);
  SensorWrite (0x2c, &data, 1);
  SensorRead (0xF7,  &data, 1);
  SensorWrite (0x2d, &data, 1);
  SensorRead (0xF8,  &data, 1);
  SensorWrite (0x2e, &data, 1);
//  data = 0x7c;
//  SensorWrite (0x13, &data, 1u); /*Return 0x13 to default*/
//  data = 0x00;
//  SensorWrite (0x60, &data, 1u); /*Return 0x60 to default*/
  /*signal stop*/

}

void
SensorCrossTalkCal (
                    void)
{
  uint8_t data;
  uint8_t data2;
  uint8_t data3;
  uint8_t bytes[3] =
    { 0u, 0u, 0u };
  uint8_t n;
  uint16_t timeout = 0xFFFFu;
  double Isum = 0.0;
  double Qsum = 0.0;
  double Gsum = 0.0;
//  data = 0x7Du;
//  SensorWrite (0x13, &data, 1);
//  data = 0x01u;
//  SensorWrite (0x60, &data, 1);
  SensorWrite (0x69, &data, 1u); //Clear flag

  for (n = 0; n < 100; ++n)
    {
      /*signal start*/
      ISL_SS(0u);

      while (timeout > 0u)
        {
          timeout--;
         if (GET_IRQ() == 0u)
            {
              break;
            }
        }
      SensorRead (0xDA, &data, 1); /*Raw exponent */
      SensorRead (0xDB, &data2, 1); /*MSB*/
      SensorRead (0xDC, &data3, 1); /*LSB*/
      Isum += Bytes2Double3S (data, data2, data3); /*convert to double*/
      SensorRead (0xDD, &data, 1); /*Raw exponent */
      SensorRead (0xDE, &data2, 1); /*MSB*/
      SensorRead (0xDF, &data3, 1); /*LSB*/
      Qsum += Bytes2Double3S (data, data2, data3); /*convert to double*/
      SensorRead (0xE6, &data, 1); /*MSB*/
      SensorRead (0xE7, &data2, 1); /*LSB*/
      Gsum += Bytes2Double2U (data, data2); /*convert to double*/

      /*signal stop*/
      ISL_SS(1u);
    }

  Isum = Isum / 100;
  Qsum = Qsum / 100;
  Gsum = Gsum / 100;

  /*Convert to 3-byte format, EXP/MSB/LSB*/

  Double2Bytes3S (Isum, bytes);

  SensorWrite (0x24, &bytes[0], 1); /*Write I EXP*/
  SensorWrite (0x25, &bytes[1], 1); /*Write I MSB*/
  SensorWrite (0x26, &bytes[2], 1); /*Write I LSB*/

  /*Convert to 3-byte format, EXP/MSB/LSB*/

  Double2Bytes3S (Qsum, bytes);

  SensorWrite (0x27, &bytes[0], 1); /*Write I EXP*/
  SensorWrite (0x28, &bytes[1], 1); /*Write I MSB*/
  SensorWrite (0x29, &bytes[2], 1); /*Write I LSB*/

  /*Convert Double to bytes*/

//  Double2Bytes2U (Gsum, bytes);
//  SensorWrite (0x2A, &bytes[0], 1); /*Write I MSB*/
//  SensorWrite (0x2B, &bytes[1], 1); /*Write I LSB*/

}

void
SensorDistanceCal (
                   void)
{
  uint8_t data;
  uint8_t bytes[2];
  double phase_sum = 0.0;
  uint8_t n;
  uint16_t timeout = 0xFFFFu;
  data = 0x7Cu;
  SensorWrite (0x13, &data, 1);
//  data = 0x01u;
//  SensorWrite (0x60, &data, 1);
  SensorWrite (0x69, &data, 1u); //Clear flag

  for (n = 0; n < 100; ++n)
    {

      /*signal start*/
      ISL_SS(0u);
      while (timeout > 0u)
        {
          timeout--;
          if (GET_IRQ() == 0u)
            {
              break;
            }
        }
      SensorRead (0xD8, &bytes[0], 1); /*MSB */
      SensorRead (0xD9, &bytes[1], 1); /*LSB */

      phase_sum += Bytes2Double2U (bytes[0], bytes[1]); /*convert to double*/
      ISL_SS(1u);
    }


  phase_sum = phase_sum / 100;
  phase_sum = phase_sum - (REF_DIST /33.33 * 65536);      // subtract actual distance to get phase offset
  bytes[0]= (uint8_t)(((int)phase_sum & 0xFF00) >> 8);    // Write phase offset MSB
  bytes[1]= (uint8_t)((int)phase_sum & 0x00FF);           // Write phase offset LSB


#if DEFAULT_CAL_DIST_VALUES == 1
  bytes[0]=0x0B;
  bytes[1]=0xFD;
#endif


  write_reg(0x2F, bytes[0]);
  write_reg(0x30, bytes[1]);




}



void
SensorDistCal2 (
                void)
{
  uint8_t data;
  uint8_t bytes[2];
  double phase_sum = 0.0;
  uint8_t n;
  uint16_t timeout = 0xFFFFu;
  float offset;
  uint8_t off_lsb, off_msb;
  data = 0u;
  SensorWrite (0x2f, &data, 1);           //DIST_CAL_OFF_MSB
  SensorWrite (0x30, &data, 1);           //DIST_CAL_OFF_MSB

  HAL_Delay (100);

  for (n = 0; n < 100; ++n)
    {

      /*signal start*/
      ISL_SS(0u);
      while (timeout > 0u)
        {
          timeout--;
         if (GET_IRQ() == 0u)
            {
              break;
            }
        }
      SensorRead (0xD1, &bytes[0], 1); /*MSB */
      SensorRead (0xD2, &bytes[1], 1); /*LSB */

      phase_sum += Bytes2Double2U (bytes[0], bytes[1]); /*convert to double*/
      ISL_SS(1u);
    }
  phase_sum = phase_sum / 100;
  phase_sum /= 2000;        //convert to meters

  offset = (phase_sum - REF_DIST) * 2000;

  off_lsb = (0xFF & (uint16_t) offset);
  off_msb = (0xFF00 & (uint16_t) offset) >> 8;

  SensorWrite (0x2f, &off_msb, 1u); //Clear flag
  SensorWrite (0x30, &off_lsb, 1u); //Clear flag

}






double
Bytes2Double3S (
                uint8_t exp,
                uint8_t msb,
                uint8_t lsb)
// method converts 3 signed bytes in format {Exp,MSB,LSB} to a double
{
  double result = 0;
  int iMantissa = 0;
  uint8_t negative = 0u;  // flag for negative numbers

  if (msb > 127)
    negative = 1u;    // negative number

  iMantissa = msb << 8;
  iMantissa |= lsb;
  if (negative)
    {
      iMantissa = ((iMantissa - 1) ^ 0xFFFF);   // convert from 2's complement
      result = -iMantissa * pow (2, exp);               // combine mantissa and exponent
    }
  else
    result = iMantissa * pow (2, exp);

  return result;
}


double
Bytes2Double2U (
                uint8_t msb,
                uint8_t lsb)
// method converts bytes in format {MSB,LSB} to a double
{
  double result = 0;
  result = (double) (unsigned short) (((int) (msb) << 8) | (int) (lsb));
  return result;
}


 void
Double2Bytes3S (
                double dNum,
                uint8_t* baResult)
// method converts a double to 3 bytes with signed mantissa {Exp,MSB,LSB}
{
  double dNumLog = 0;
  uint8_t a;
  uint8_t negative = 0u;
  uint8_t exp, new_exp = 0;
  int iMantissa = 0;
  double dMantissa = 0;

  if (dNum < 0)       // handle negative numbers
    {
      negative = 1u;        // set negative flag
      dNum = fabs (dNum);  // convert to positive
    }

  dNumLog = Log2(dNum);            // log base 2 of input
  exp = (uint8_t) dNumLog;                    // exponent of the double
  dMantissa = (dNumLog - (double) exp);    // log of mantissa
  dMantissa = pow (2, dMantissa);     // convert mantissa to double
  new_exp = exp;                          // start new exponent as the original exponent

  // it might seem like 15 shifts is the correct number but it's 14.
  // Doing 15 shifts into the sign bit making it a negative number.
  for (a = 1; a <= exp && a < 15; ++a)    // convert mantissa to whole number
    {
      dMantissa = dMantissa * 2;          // double the mantissa
      --new_exp;                          // decrement the exponent
    }

  if (negative)
    iMantissa = (int) (-dMantissa);     // take 2's complement, convert to short
  else
    iMantissa = (int) (dMantissa);      // convert to short

  baResult[0] = new_exp;
  baResult[1] = (uint8_t) ((iMantissa & 0xFF00) >> 8);
  baResult[2] = (uint8_t) (iMantissa & 0x00FF);
}



void
Double2Bytes2U (
                double dNum,
                uint8_t* baResult)
// method converts a double to 2 bytes {MSB,LSB}
{
  int iNum = (int) dNum;
  baResult[0] = (uint8_t) ((iNum & 0x0000FF00) >> 8);
  baResult[1] = (uint8_t) (iNum & 0x000000FF);
}


uint8_t
read_reg (
          uint8_t reg)
{
  uint8_t data;
  SensorRead (reg, &data, 1);
  return data;
}

void
write_reg (
           uint8_t reg,
           uint8_t data)
{
  I2C_MESSAGE_STATUS status; 
  uint8_t data_buffer[2];
  data_buffer[0] = reg;
  data_buffer[1] = data;

   
 // (void) HAL_I2C_Master_Transmit (&XNUCLEO53L0A1_hi2c, ISL29501_I2C_ADDRESS, data_buffer, 2u, 100);
  I2C_MasterWrite(data_buffer,2,ISL29501_I2C_ADDRESS,&status);
}

double Log2( double n )  
{  
    // log(n)/log(2) is log2.  
    return log( n ) / log( 2 );  
}