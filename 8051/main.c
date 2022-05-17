#include <reg51.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "port.h"
#include "lcd.h"
#include "dht.h"
#include "delay.h"
#include "ADC0804.h"
#include "interrupt.h"
#include "uart.h"

int I_RH, D_RH, I_Temp, D_Temp, CheckSum;
unsigned char dat[20];
unsigned int adc_value_GPMY;
unsigned int voltage_GPMY;
unsigned int dustDensity;

void get_DHT11();
void get_MQ05();
void get_GPMY();

void reverse(char *s){
  char * j;
  int c;

  j = s + strlen(s) - 1;
  while (s < j) {
    c = * s;
    * s++ = * j;
    * j-- = c;
  }
}

void itoa(int n, char s[]) {
  int i, sign;

  if ((sign = n) < 0) 
    n = -n; 
  i = 0;
  do {
    s[i++] = n % 10 + '0';
  } while ((n /= 10) > 0); 
  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';
  reverse(s);
}

void main() {
	Uart_Init();
  LCD_Init();
  INT_Init();
  while (1) {

		get_DHT11();
    get_GPMY();
    get_MQ05();
  }
}

void get_DHT11() {
  Request();
  Response();

  I_RH = Receive_data();
  D_RH = Receive_data();
  I_Temp = Receive_data();
  D_Temp = Receive_data();
  CheckSum = Receive_data();
	
  if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum) {
		LCD_String_xy(0,0,"Error");
	}
  else{
		itoa(I_RH, dat);
		LCD_String_xy(0,0,"Do am:");
		LCD_String(dat);
		itoa(I_Temp, dat);
		LCD_String(" Nhiet do:");
		LCD_String(dat);
  }
  delay_ms(1000);
}

void get_GPMY() {
  LED_GPMY = 1;
  timer_delay280us();
  voltage_GPMY = ADC0804_Read();
  timer_delay40us();
  LED_GPMY = 0;
  timer_delay9680us();
	
	itoa( voltage_GPMY, dat);
	LCD_String_xy(1,0,dat);
	
}

void get_MQ05() {
  if (MQ05) LCD_String(" 1");
  else LCD_String(" 0");
}

void EX1_ISR(void) interrupt 2 {
  timer_delay20ms();
  LED_LCD = !LED_LCD;
}	