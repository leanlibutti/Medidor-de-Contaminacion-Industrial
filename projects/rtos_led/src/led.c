#include "led.h"
#include "board.h"
#include "main.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define LED_ROJO     0
#define LED_VERDE    1
#define LED_AZUL     2
#define APAGADO      0
#define ENCENDIDO    1

void ledTask(void * a)
{
	uint32_t dato;
	uint32_t num;

	while (1) {
		/* esperamos recibir un dato a través de esta cola
		 */

		xQueueReceive(cola_r, &dato, 0);

		if ((dato >= '0') && (dato<='9')){
			num=dato;

			//imprimimos el cartel del estado actual la primera vez que entra luego del cambio
			switch (dato) {
			case '0':
				printf("LED rojo apagado.\r\n");
				break;

			case '9':
				printf("LED rojo apagado.\r\n");
				break;

			default:
				printf("LED rojo destellando cada %u ms.\r\n", dato );
				break;

			}
		}

		//seteamos el valor que tiene el led cada vez que entra a la tarea
		//en caso que sea distinto de 0 o 9, hacemos el toggle del valor del led
		switch(num){
		case '0':
			Board_LED_Set(LED_ROJO, APAGADO);
			break;
		case '9':
			Board_LED_Set(LED_ROJO, ENCENDIDO);
			break;
		default:
			Board_LED_Toggle(LED_ROJO);
			//(num -30) es para castear el ascii al valor real del numero
			vTaskDelay((num - 30) * 100 /portTICK_RATE_MS);
			break;
		}

	}
}

void ledTask1(void * a)
{
	uint32_t dato;
	uint32_t num;

	while (1) {
		/* esperamos recibir un dato a través de esta cola,
		 */
		xQueueReceive(cola_g, &dato, 0);

		if ((dato >= '0') && (dato<='9')){
			num=dato;

			//imprimimos el cartel del estado actual la primera vez que entra luego del cambio
			switch (dato) {
			case '0':
				printf("LED verde apagado.\r\n");
				break;

			case '9':
				printf("LED verde apagado.\r\n");
				break;

			default:
				printf("LED verde destellando cada %u ms.\r\n", dato );
				break;

			}
		}

		//seteamos el valor que tiene el led cada vez que entra a la tarea
		//en caso que sea distinto de 0 o 9, hacemos el toggle del valor del led
		switch(num){
		case '0':
			Board_LED_Set(LED_VERDE, APAGADO);
			break;
		case '9':
			Board_LED_Set(LED_VERDE, ENCENDIDO);
			break;
		default:
			Board_LED_Toggle(LED_VERDE);
			//(num -30) es para castear el ascii al valor real del numero
			vTaskDelay(num * 100 /portTICK_RATE_MS);
			break;
		}

	}
}


void ledTask2(void * a)
{
	uint32_t dato;

	//declarada static para que conserve el valor en cada llamado a la tarea
	static uint32_t num;

	while (1) {

		// esperamos recibir un dato a través de esta cola
		xQueueReceive(cola_b, &dato, 0);

		if ((dato >= '0') && (dato<='9')){
			num=dato;
			//imprimimos el cartel del estado actual la primera vez que entra luego del cambio
			switch (dato) {
			case '0':
				printf("LED azul apagado.\r\n");
				break;

			case '9':
				printf("LED azul apagado.\r\n");
				break;

			default:
				printf("LED azul destellando cada %u ms.\r\n", dato );
				break;

			}
		}

		//seteamos el valor que tiene el led cada vez que entra a la tarea
		//en caso que sea distinto de 0 o 9, hacemos el toggle del valor del led
		switch(num){
		case '0':
			Board_LED_Set(LED_AZUL, APAGADO);
			break;
		case '9':
			Board_LED_Set(LED_AZUL, ENCENDIDO);
			break;
		default:
			Board_LED_Toggle(LED_AZUL);
			//(num -30) es para castear el ascii al valor real del numero
			vTaskDelay((num-30) * 100 /portTICK_RATE_MS);
			break;
		}

	}
}


void ledSetup(void)
{
	Board_LED_Set(LED_ROJO, APAGADO);
	Board_LED_Set(LED_AZUL, APAGADO);
	Board_LED_Set(LED_VERDE, APAGADO);

	//tarea del led rojo
	xTaskCreate(ledTask,
			(const char *)"task",
			configMINIMAL_STACK_SIZE*2,
			0,
			tskIDLE_PRIORITY+1,
			0);

	//tarea del led verde
	xTaskCreate(ledTask1,
			(const char *)"task",
			configMINIMAL_STACK_SIZE*2,
			0,
			tskIDLE_PRIORITY+1,
			0);

	//tarea del led azul
	xTaskCreate(ledTask2,
			(const char *)"task",
			configMINIMAL_STACK_SIZE*2,
			0,
			tskIDLE_PRIORITY+1,
			0);
}
