#include "main.h"
#include "led.h"
#include "serial.h"
#include "board.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define QUEUE_LEN       10
#define QUEUE_ITEM_SIZE sizeof(uint32_t)

QueueHandle_t cola_r;
QueueHandle_t cola_g;
QueueHandle_t cola_b;

static void initHardware(void)
{
    SystemCoreClockUpdate();
    Board_Init();
    Board_LED_Set(0, false);
    Board_LED_Set(1, false);
    Board_LED_Set(2, false);
}

int main(void)
{
	initHardware();

	ledSetup();

	serialSetup();

	/* Creamos cada cola de QUEUE_LEN elementos,
	 * cada elemento de tamaño QUEUE_ITEM_SIZE.
	 */
	cola_r = xQueueCreate(QUEUE_LEN, QUEUE_ITEM_SIZE);
	cola_g = xQueueCreate(QUEUE_LEN, QUEUE_ITEM_SIZE);
	cola_b = xQueueCreate(QUEUE_LEN, QUEUE_ITEM_SIZE);

	vTaskStartScheduler();

	while (1) { }
}
