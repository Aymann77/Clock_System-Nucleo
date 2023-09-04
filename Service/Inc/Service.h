/*******************************************************************************
* Filename              :   Service.h
* Author                :   Mohamemd Waleed Gad
* Origin Date           :   Aug 29, 2023
* Version               :   1.0.0
* Compiler              :   GCC ARM Embedded Toolchain
* Target                :   
* Notes                 :   None 
**
*******************************************************************************/
/************************ SOURCE REVISION LOG *********************************
*
*    Date    Version   Author             Description 
*  14/10/20   1.0.0   Mohamemd Waleed   Initial Release.
*
*******************************************************************************/
#ifndef INC_SERVICE_H_
#define INC_SERVICE_H_

/* ========================================================================= *
 *                      FUNCTIONS PROTOTYPES SECTION                         *
 * ========================================================================= */

/*==============================================================================================================================================
 *@fn      :  void ClockInit()
 *@brief  :  This Function Is Responsible For Initializing The Clocks For The Used Peripherals
 *@retval void :
 *==============================================================================================================================================*/
void ClockInit();

/*==============================================================================================================================================
 *@fn      :  void PinInit()
 *@brief  :   This Function Is Responsible For Initializing The Pins For The Used Peripherals
 *@retval void :
 *==============================================================================================================================================*/
void PinInit();

/*==============================================================================================================================================
 *@fn      : void CalcAlarm(uint8_t AlarmNumber)
 *@brief  :  This Function Is Responsible For Calculating The Alarm Time And Storing It In The Global Array
 *@paramter[in]  : uint8_t AlarmNumber : The Number Of The Alarm To Be Set
 *@retval void :
 *==============================================================================================================================================*/
void CalcAlarm(uint8_t AlarmNumber);

/*==============================================================================================================================================
 *@fn      :  void CompTime()
 *@brief  :   This Function Is Responsible For Comparing The Current Time With The Alarm Time And Send The Alarm Number To The Blue Pill If They Are Equal
 *@retval void :
 *==============================================================================================================================================*/
void CompTime();

/*==============================================================================================================================================
 *@fn      : void PeripheralInit()
 *@brief  :  This Function Is Responsible For Initializing The Peripherals
 *@retval void :
 *==============================================================================================================================================*/
void PeripheralInit();

/*==============================================================================================================================================
 *@fn      : void SetAlarm()
 *@brief  :  This Function Is Responsible For Setting The Alarm Time
 *@retval void :
 *==============================================================================================================================================*/
void SetAlarm();

/*==============================================================================================================================================
 *@fn      : void SPI1_ISR()
 *@brief  :  This Function Is The ISR For The SPI1 Interrupt
 *@retval void :
 *==============================================================================================================================================*/
void SPI1_ISR();

void InterruptsInit (void);

void SysTickPeriodicISR();
/* ========================================================================= *
 *                         PRIVATE MACROS SECTION                            *
 * ========================================================================= */

typedef enum
{
Equal,
NotEqual
}Equality_t;

#define Filling       {{0xFF ,0xFF,0xFF },{0xFF ,0xFF,0xFF },{0xFF ,0xFF,0xFF },{0xFF ,0xFF,0xFF },{0xFF ,0xFF,0xFF }}


#endif /* INC_SERVICE_H_ */
