/**
 ******************************************************************************
 * @file           : main.c
 * @author         : MOHAMMEDs & HEMA
 * @brief          : Main program body
 ******************************************************************************
 */

/************************ SOURCE REVISION LOG *********************************
 *
 *    Date      Author             Description
 *  25/08/23   Mohamemd Ayman    Initial Release ( INCLUDES ).
 *  27/08/23   Mohammed Ayman    Program Basic Functionality.
 *  27/08/23   Mohammed Wael     Implement Function For Reading From USART.
 *  27/08/23   Mohammed Waled    Initial Release For Functions ( PeripheralInit - SetAlarm - CalcAlarm)
 *	28/08/23   Mohammed Ayman    Art Changes to Make Project More Prettier.
 *  28/08/23   Mohammed Wael     Implement Function to Calculate the Calender Given From User via USART.
 *  28/08/23   Ibrahim Mohammed  Implement Function to Reading data from RTC.
 *  28/08/23   Ibrahim Mohammed  Implement Function to Transmit data via SPI.
 *  28/08/23   Mohammed Ayman    Adding A Service Layer & Well Documenting the Application Layer.
 *  28/08/23   Mohammed Waled    Initial Release For CompTime Function
 *  29/08/23   Mohammed Wael     Implement Function to Check the Calender Given From User via USART and Send it to RTC.
 *	29/08/23   Mohammed Ayman    Merging is Done of 2 branches on Develop Branch.
 *	29/08/23   Mohammed Waled    Adding Some Documentation And Fixing Some Bugs
 *  30/08/23   Mohammed Wael     Solving The Conflicts and Pushing to Remote Repe.
 *	30/08/23   Mohammed Ayman    Trying The Application After Merging.
 *
 *
 *******************************************************************************/

/* ========================================================================= *
 *                            INCLUDES SECTION                               *
 * ========================================================================= */

#include <stdint.h>

#include "../Library/ErrTypes.h"
#include "../Library/STM32F446xx.h"

#include "../Drivers/Inc/I2C_Interface.h"
#include "../Drivers/Inc/UART_Interface.h"
#include "../Drivers/Inc/SYSTICK_Interface.h"

#include "../HAL/Inc/DS1307_Interface.h"

#include "../Service/Inc/Service.h"

/* ========================================================================= *
 *                        GLOBAL VARIABLES SECTION                           *
 * ========================================================================= */

/* Variable to Put Date & Time Configuration in It to Send it to RTC
 *  to Set the Date & Time
 */
extern DS1307_Config_t Date_Time_RTC;

/* Variable to Store the Configuration of I2C1 that will be Used in the Application ( In Setting Date & Time ) */
extern I2C_Configs_t *I2C_CONFIG;

/* ========================================================================= *
 *                        MAIN APPLICATION SECTION                           *
 * ========================================================================= */

int main(void)
{
	/* Error State Variable to Check if the Receiving Process of Date & Time From User
	 * is Done Successfully or Not
	 */
	Error_State_t Receiving_State = OK;

	/* Choosen Option By the User From the Provided Options */
	OPTIONS_t ChoosenOption = NO_OPTION;

	/* Variable to Store the ID Sent From User */
	uint8_t *ID_Ptr = NULL;

	/* Variable to Store Password Sent From User */
	uint8_t *Pass_Ptr = NULL;

	/* Enable Clock on Used Peripherals Only */
	Clock_Init();

	/* Set Pins Configurations */
	Pins_Init();

	/* NVIC Interrupts Configuration */
	Interrupts_Init();

	/* Initialize USART2 */
	USART2_Init();

	/* Initialize SPI1 */
	SPI1_Init();

	/* Initialize I2C1 */
	I2C1_Init();

	/* Clear Terminal Window With Every Reset */
	Clear_Terminal();

	/* Receive ID From User */
	ID_Ptr = ID_Reception();

	/* Receive Password From User */
	Pass_Ptr = Pass_Reception();

	/* Check on Login Info Passing ID , Password & Number of Allowed Tries
	 *   Program Exits This Function Only If Login Info is Correct , IF Not Function will Force
	 *   An SPI Data Transfer & Stuck Inside SPI Call Back in the Background
	 */
	Check_LoginInfo(ID_Ptr, Pass_Ptr, NUM_OF_TRIES);



	while (1)
	{
		/* System Login is Initiated Successfully */

		/* Display the Menu of Options if Login Info is Correct
		 * Else Stuck in the Call Back Function
		 */
		ChoosenOption = Display_Menu();

		/* Switch on the Chossen Option */
		switch (ChoosenOption)
		{
		/* ========================================================================= *
		 * 								OPTIONS SECTION 						     *
		 * ========================================================================= */
		case DISPLAY_OPTION:

			/* Read Date & Time */
			Reading_Time();

			/* Transmit Date & Time  */
			Transmit_Time();

			break;

		case SET_ALARM_OPTION:

			/* Set Alarm */
			SetAlarm();

			/* Configuring SYSTICK To Call CompTime Function Every One Second
			to Compare Between Alarm Set By User & Real Time From RTC */
			SYSTICK_voidSetINT(1000, 1, &SysTickPeriodicISR);

			break;

		case SET_DATE_TIME_OPTION:

			while (1)
			{
				/*Read the settled time and date from PC terminal*/
				Receiving_State = ReadDateTime_FromPC();

				/*Check The Received Date & Time*/
				if (OK == Receiving_State)
				{

					/*Receiving Calender from user is done Successfully*/
					/*Write the Received Calender in the RTC Module*/
					DS1307_WriteDateTime(I2C_CONFIG, &Date_Time_RTC);

					/*Display message to user that the time settled successfully*/
					USART_SendStringPolling(UART_2, "\nThe Given Time Settled successfully\n");

					/*Return to Main Menu*/
					break;
				}
				else
				{
					/*Wrong Calender Received from User*/
					USART_SendStringPolling(UART_2, "\nWrong Date or Time is Given , Please Try Again\n");
				}
			}
			break;

		default:

			/* If User Passed a Wrong Option */
			WRONG_OptionChoosen();
			continue;
		}

		/* After the User Functionality is Done Check if User wants To Continue or Not */
		Check_IF_ContinueisNeeded();
	}
}

/* ========================================================================= *
 *                        INTERRUPTS SECTION                                 *
 * ========================================================================= */

/* SPI Call Back Function to Be Called When SPI Data Transfer is Done when
  User Finish Number of Tries in Login to Force Him to Reset the Board */
void SPI_CallBackFunc(void)
{
	/* ShutDown Sequence Excecution */
	ShutDown_Sequence();
}

/* SYSTICK ISR to Compare Between Alarm Set By User & Real Time From RTC */
void SysTickPeriodicISR()
{
	/* Systick ISR to Compare Time Comes Every 1 second */
	CompTime();
}
