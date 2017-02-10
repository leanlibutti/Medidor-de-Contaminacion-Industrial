/*
 * main.c
 *
 *  Created on: 24 ene. 2017
 *      Author: castillo, libutti, sanjuan, risso
 */
/*==================[inclusions]=============================================*/
#include "main.h"       /* <= own header */


/*==================[internal data declaration]==============================*/
uint16_t medida_mq135;
uint16_t medida_mq2;
uint16_t medida_mq3;
uint16_t medida_temp;
uint16_t umbral_mq135= 100;
uint16_t umbral_mq2= 5000;
uint16_t umbral_mq3= 5000;
uint16_t umbral_temp = 25;
uint8_t hora;
uint8_t minutos;
uint8_t segundos;
unsigned char flag_identificador=0; //utilizado para saber que dato modificar 
unsigned char flag_ubicacion=0; //utilizado para saber que digito del dato voy a leer

uint8_t UART0_IRQHandler(){
	dato= uartReadByte();
	if (flag_identificador != 0){
		switch(flag_identificador){
			case 1 :{ //modificar la hora
				if(flag_ubicacion == 0){
					hora = dato;
					flag_ubicacion ++;
				}
				else{
					dato= (uint16_t) dato;
					hora= (dato & 0xff00) + (hora & 0x00ff);
					flag_ubicacion--;
					flag_identificador=0; 
				} 
				break;
			}
			case 2 :{ //modificar los minutos 		
				if(flag_ubicacion == 0){
					minutos = dato;
					flag_ubicacion ++;
				}
				else{
					dato= (uint16_t) dato;
					minutos= (dato & 0xff00) + (minutos & 0x00ff);
					flag_ubicacion--;
					flag_identificador=0; 
				} 
				break;
			}
			case 3 :{ //modificar los segundos
				if(flag_ubicacion == 0){
					segundos = dato;
					flag_ubicacion ++;
				}
				else{
					dato= (uint16_t) dato;
					segundos= (segundos & 0xff00) + (segundos & 0x00ff);
					flag_ubicacion--;
					flag_identificador=0; 
				} 
				break;
			}
			case 4 :{ //modificar el umbral_mq2
				if(flag_ubicacion == 0){
					umbral_mq2 = dato;
					flag_ubicacion ++;
				}
				else{
					dato= (uint16_t) dato;
					umbral_mq2= (dato & 0xff00) + (umbral_mq2 & 0x00ff);
					flag_ubicacion--;
					flag_identificador=0; 
				} 
				break;
			}
			case 5 :{ //modificar l umbral_mq3
				if(flag_ubicacion == 0){
					umbral_mq3 = dato;
					flag_ubicacion ++;
				}
				else{
					dato= (uint16_t) dato;
					umbral_mq3= (dato & 0xff00) + (umbral_mq3 & 0x00ff);
					flag_ubicacion--;
					flag_identificador=0; 
				} 
				break;
			}
			case 6 :{ //modificar el umbral_mq135
				if(flag_ubicacion == 0){
					umbral_mq135 = dato;
					flag_ubicacion ++;
				}
				else{
					dato= (uint16_t) dato;
					umbral_mq135= (dato & 0xff00) + (umbral_mq135 & 0x00ff);
					flag_ubicacion--;
					flag_identificador=0; 
				} 
				break;
			}
		}
	}
	else{ //modifico el identificador 
		switch(dato){
			case 'h':{
				flag_identificador= 1;
				break;
			}
			case 'm':{
				flag_identificador= 2;
				break;
			}
			case 's':{
				flag_identificador= 3;
				break;
			}
			case '2':{
				flag_identificador= 4;
				break;
			}
			case '3':{
				flag_identificador= 5;
				break;
			}
			case '5':{
				flag_identificador= 6;
				break;
			}
		}

	}

}


void SysTick_Handler(void)
{
	segundos ++;
	if (segundos == 60){
		minutos ++;
		segundos=0;
		if (minutos == 60){
			hora ++;
			minutos = 0;
			if (hora == 24)
				hora=0;
		}
	}
	disk_timerproc();   /* Disk timer process */
}

void config_sensor(){
	//configuracion del micro
    Board_Init();
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock); // configurado para que interrumpa cada 1 segundo

	//configuracion del adc
	analogConfig(ENABLE_ANALOG_INPUTS);

	//configuracion del Rs485
	uint8_t direccion;
	direccion= 1;
	rsConfig(direccion);

	//configuracion de la sd
	config_sd();

}

int main(){
  unsigned char hacer= 1;
  config_sensor();
  while(1){
	//leo las medidas de los 4 sensores (medida en 16 bits de tensiones)
	medida_mq135= analogRead(AI3); //conectado al CH3 
	medida_mq2= analogRead(AI0); //conectado al CH0
	medida_mq3= analogRead(AI2); //conectado al CH2
	medida_temp= analogRead(AI1); //conectado al CH1

	//realizar ajuste y conversion de las medidas realizadas con la medida de la temperatura
	medida_mq2= 12.1 * medida_mq135 - 1856.8 ; // 200 a 10000 ppm
	medida_mq135= 0.23 * medida_mq135 - 29.88; // 10 a 200 ppm
	medida_mq3= 12.2 * medida_mq3 - 1977.78; // 100 a 10000 ppm
	medida_temp= (medida_temp - 0.79) / 0.1; // 0 a 150 °C 

	//enviar medidas reales por rs485 al poncho interfaz
	uint8_t array[2];

	array[0]=medida_mq135 & 0xff;
	array[1]=(medida_mq135 >> 8);
	uartWriteByte(array[0]); //envia parte baja de los 16 bits*
	uartWriteByte(array[1]); //envia parte alta de los 16 bits

	array[0]=medida_mq2 & 0xff;
	array[1]=(medida_mq2 >> 8);
	uartWriteByte(array[0]); //envia parte baja de los 16 bits
	uartWriteByte(array[1]); //envia parte alta de los 16 bits

	array[0]=medida_mq3 & 0xff;
	array[1]=(medida_mq3 >> 8);
	uartWriteByte(array[0]); //envia parte baja de los 16 bits
	uartWriteByte(array[1]); //envia parte alta de los 16 bits

	//guardar medidas reales en la memoria sd
	unsigned char cadena1[]= {'M','q','1','3','5',((medida_mq135/10000) + 48), (((medida_mq135 % 10000) / 1000) + 48), (((medida_mq135/100) % 100) + 48), (((medida_mq135/10) % 1000) + 48), ((medida_mq135 % 10000) + 48), '\0','\n' };
	sdWrite(cadena1);
	if (medida_mq135 > umbral_mq135){
		sdWrite("Alarma!! MQ135 paso el umbral!!");
	}
	unsigned char cadena2[]= {'M','q','2',((medida_mq2/10000) + 48), (((medida_mq2 % 10000) / 1000) + 48), (((medida_mq2/100) % 100) + 48), (((medida_mq2/10) % 1000) + 48), ((medida_mq2 % 10000) + 48), '\0','\n' };
	sdWrite(cadena2);
	if (medida_mq2 > umbral_mq2){
		sdWrite("Alarma!! MQ2 paso el umbral!!");
	}
	unsigned char cadena3[]= {'M','q','3',((medida_mq3/10000) + 48), (((medida_mq3 % 10000) / 1000) + 48), (((medida_mq3/100) % 100) + 48), (((medida_mq3/10) % 1000) + 48), ((medida_mq3 % 10000) + 48), '\0','\n' };
	sdWrite(cadena3);
	if (medida_mq3 > umbral_mq3){
		sdWrite("Alarma!! MQ3 paso el umbral!!");
	}
	if (hacer == 2){ // mandar la medida de temperatura cada 1 segundo 
		array[0]=medida_temp & 0xff;
		array[1]=(medida_temp >> 8);
		uartWriteByte(array[0]); //envia parte baja de los 16 bits
		uartWriteByte(array[1]); //envia parte alta de los 16 bits
		hacer = 0;
	}

	//esperar 1 segundo
	delay(500);
	hacer ++;
  }

}


