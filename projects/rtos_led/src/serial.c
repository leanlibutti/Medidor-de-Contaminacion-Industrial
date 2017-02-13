#include "serial.h"
#include "board.h"
#include "main.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <string.h>

/* longitud de los buffers de transmisión y recepción */
#define UART_BUF_LEN 512

/* período de envío del mensaje de prueba */
#define TIEMPO_UART 1000

/* mensaje de prueba */
#define TEST_STRING "Hola mundo!\r\n"

/* definición de UART RS232 a utilizar en función de la placa */
/* EDU-CIAA (LPC4337) */
#ifdef lpc4337_m4
#define UART_PORT LPC_USART2
#define UART_IRQn USART2_IRQn
#endif
/* picoCIAA (LPC54102) */
#ifdef lpc54102_m4
#define UART_PORT LPC_USART0
#define UART_IRQn UART0_IRQn
#endif

/* Buffers circulares (ring buffers) provistos por LPCOpen */
RINGBUFF_T rrb, trb;
uint8_t rbuf[UART_BUF_LEN];
uint8_t tbuf[UART_BUF_LEN];

/* Definición del handler de interrupción según el microcontrolador */
#ifdef lpc54102_m4
void UART0_IRQHandler(void)
#endif
#ifdef lpc4337_m4
void UART2_IRQHandler(void)
#endif
{
	/* llamo al handler de LPCOpen para ring buffers */
	Chip_UART_IRQRBHandler(UART_PORT, &rrb, &trb);
}

/* funcion para enviar datos */
int32_t serialSend(void * data, uint32_t datalen)
{
	return Chip_UART_SendRB(UART_PORT, &trb, data, datalen);
}

/* función para recibir datos */
int32_t serialRead(void * data, uint32_t datalen)
{
	return Chip_UART_ReadRB(UART_PORT, &rrb, data, datalen);
}

/* tarea que implementa el eco de caracteres enviados por el usuario */
void serialEchoTask(void * a)
{
	uint32_t bytes_leidos;
	char buf[10];
	uint32_t dato1;
	uint32_t dato2;

	printf("Esperando LED y tiempo... \r\n");

	while (1) {
		do{
			bytes_leidos = Chip_UART_ReadRB(UART_PORT, &rrb, buf, 10);
		}while(bytes_leidos == 0);

		dato1= buf[0];

		if ((dato1 >= '0') && (dato1 <= '9')) {
			dato2= buf[1];
			switch (dato1){
				case 'r':{
					xQueueSend(cola_r, dato2, 0);
					break;
				}
				case 'R':{
					xQueueSend(cola_r, dato2, 0);
					break;
				}
				case 'g':{
					xQueueSend(cola_g, dato2, 0);
					break;
				}
				case 'G':{
					xQueueSend(cola_g, dato2, 0);
					break;
				}
				case 'b':{
					xQueueSend(cola_b, dato2, 0);
					break;
				}
				case 'B':{
					xQueueSend(cola_b, dato2, 0);
					break;
				}

			}
		}
		else
			Chip_UART_SendRB(UART_PORT, &trb, buf, "dato incorrecto");

		}
		vTaskDelay(1);
}

void serialSetup(void)
{
	/* configuro la UART en función de la placa */
	Chip_UART_Init(UART_PORT);
	Chip_UART_SetBaud(UART_PORT, 115200);

	/* Configuración típica para EDU-CIAA */
	Chip_SCU_PinMux(7, 1, MD_PDN, FUNC6);              /* P7_1: UART2_TXD */
	Chip_SCU_PinMux(7, 2, MD_PLN|MD_EZI|MD_ZI, FUNC6); /* P7_2: UART2_RXD */

	RingBuffer_Init(&rrb, rbuf, 1, UART_BUF_LEN);
	RingBuffer_Init(&trb, tbuf, 1, UART_BUF_LEN);

	Chip_UART_IntEnable(LPC_USART2, UART_IER_RBRINT);

	/* Habilito interrupción de la UART */
	Chip_UART_TXEnable(UART_PORT);
	NVIC_EnableIRQ(UART_IRQn);

	/* Creo tareas */
	xTaskCreate(serialEchoTask,
			(const char *)"serialEchoTask",
			1024, /* 1kB de pila */
			0,
			tskIDLE_PRIORITY+1,
			0);
}
