/*
 * southbound_generic.c
 *
 *  Created on: 17 ago. 2017
 *      Author: FLG
 */

#include <southbound_mifare.h>
#include <southbound_generic.h>
#include "ssd1306.h"
#include "fonts.h"
#include "string.h"

void LCD_Init(void)
{
	LCD_Set_Parameters();
	HAL_Delay(500);
	LCD_Display_Update();
}

void LCD_Display_Update(void)
{
	//Fill and Update screen
	ssd1306_Fill(White);
	ssd1306_UpdateScreen();
}

void LCD_SetCursor(uint8_t x, uint8_t y)
{
	ssd1306_SetCursor(x,y);
}

void LCD_Write_mifare_info(Device_Status status)
{
	char info_string[20];

	switch(status)
	{
		case Normal:
			strcpy(info_string, "System OK");
			//info_string = "System OK";
			LCD_Display_Update();
			LCD_SetCursor(10,23);
			LCD_Write_String(info_string);
			break;

		case Reading:
			//info_string = "Reading Card";
			strcpy(info_string,"Reading Card");
			LCD_Display_Update();
			LCD_SetCursor(10,23);
			LCD_Write_String(info_string);
			break;

		case Registered:
			//info_string = "Registered";
			strcpy(info_string,"Registered");
			LCD_Display_Update();
			LCD_SetCursor(10,23);
			LCD_Write_String(info_string);
			LCD_SetCursor(10,33);
			strcpy(info_string,"in Server");
			//info_string = "in Server";
			LCD_Write_String(info_string);
			break;

		case Not_Registered:
			//info_string = "Not Registered";
			strcpy(info_string,"Not Registered");
			LCD_Display_Update();
			LCD_SetCursor(10,23);
			LCD_Write_String(info_string);
			LCD_SetCursor(10,33);
			strcpy(info_string,"in Server(Error)");
			//info_string = "in Server(Error)";
			LCD_Write_String(info_string);
			break;

		case Init_OK:
			strcpy(info_string, "IP Domain:");
			LCD_Display_Update();
			LCD_SetCursor(10,23);
			LCD_Write_String(info_string);
			LCD_SetCursor(10,33);
			strcpy(info_string, Context.IP_server);
			//info_string = "IP server";
			LCD_Write_String(info_string);
			break;
	}

}


void Blink_LED_Status(Device_Status status)
{
	uint8_t counter = 0;

	switch (status)
	{
		case Reading:

			for(counter = 0; counter < 4 ; counter++){
				MIC_Set_Digital_Output_status(0,0);
				HAL_Delay(150);
				MIC_Set_Digital_Output_status(0,1);
				HAL_Delay(150);
			}
			break;

		case Registered:

			MIC_Set_Digital_Output_status(0,0);
			HAL_Delay(500);
			MIC_Set_Digital_Output_status(0,1);

	}
}

void PWM_signal_simulator(void)
{
	uint8_t counter = 0;

	for(counter = 0; counter < 50 ; counter++){
		MIC_Set_Digital_Output_status(1,0);
		HAL_Delay(0.025);
		MIC_Set_Digital_Output_status(1,1);
		HAL_Delay(0.05);
	}
}
void Buzzer_Control(Buzzer_tone tone)
{
	uint8_t i=0;

	switch(tone)
	{
		case short_beep_1:

			PWM_signal_simulator();
			break;

		case short_beep_2:

			while(tone !=0){
				PWM_signal_simulator();
				tone--;
				HAL_Delay(100);
			}
			break;

		case long_beep:
			for(i=0;i<5;i++){
				PWM_signal_simulator();
			}
	}
}


uint8_t sendingATCommands(UART_HandleTypeDef *phuart1, uint32_t timeoutTx,
		uint32_t timeoutRx, uint32_t numberOfReceivedBytes,
		unsigned char *messageTX, unsigned char *messageRX) {
	int lengthOfmessage = 0;


	HAL_StatusTypeDef UARTStatus;

	while (messageTX[lengthOfmessage++] != '\r')
		;

	UARTStatus = MIC_UART_Get_Data(phuart1, &data, 1);
	//UARTStatus = HAL_UART_Receive_IT(phuart1, &dato, 1); // activo interrupcion
	UARTStatus = MIC_UART_Send_Data(phuart1, messageTX, lengthOfmessage,timeoutTx);

	while ((BufferReceptionCounter < numberOfReceivedBytes)
			& (timeout == 0))
		; /// Espera activa hasta que al menos me hayan llegado numberOfReceivedBytes bytes
	HAL_Delay(timeoutRx); // adem�s 100ms

	messageRX = bufferReception;

	if (UARTStatus == HAL_OK)
		return 1;
	else
		return 0;

}






void CleanBufferReception(void) {

	//HAL_NVIC_DisableIRQ (USART3_IRQn);
	uint16_t counter = 0;
	for (counter = 0; counter < 100; counter++)
		bufferReception[counter] = 0x00;
	BufferReceptionCounter = 0;
	//HAL_NVIC_EnableIRQ(USART3_IRQn);

}
