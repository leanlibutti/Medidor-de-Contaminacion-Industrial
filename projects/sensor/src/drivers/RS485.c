#include "RS485.h"       /* <= own header */

#define UART0_DIR_P   6
#define UART0_DIR_P_  2
#define UART0_TXD_P   9
#define UART0_TXD_P_  5
#define UART0_RXD_P   9
#define UART0_RXD_P_  6

#define ON 1
#define OFF 0

/* Funcion que configura la uart
* Parametros:
*	address: direccion MAC
* Salidas:ninguna
*
*Consultar bit de stop en el frame 
*
*
*/
void rsConfig(uint8_t address){
	Chip_UART_Init(LPC_USART0);
	Chip_UART_SetBaud(LPC_USART0, 19200);
	Chip_UART_SetupFIFOS(LPC_USART0, UART_FCR_FIFO_EN |UART_FCR_RX_RS| UART_FCR_TX_RS | UART_FCR_TRG_LEV3); /* habilita la cola fifo y seteo para que la interrupcion en la recepcion se genere cada 2 bytes */
	Chip_UART_TXEnable(LPC_USART0); /* habilito transmisor */
	Chip_UART_IntEnable(LPC_USART0, UART_IER_RBRINT); /* habilito interrupcion en el receptor */
	Chip_SCU_PinMux(UART0_TXD_P, UART0_TXD_P_ , MD_PDN, FUNC7);              /* P9_5,FUNC7: UART0_TXD */
	Chip_SCU_PinMux(UART0_RXD_P, UART0_RXD_P_ , MD_PLN|MD_EZI|MD_ZI, FUNC7); /* P9_6,FUNC7: UART0_RXD */
	Chip_SCU_PinMux(UART0_DIR_P, UART0_DIR_P_, MD_PDN, FUNC2);               /* P6_2,FUNC2: UART0_DIR */
	Chip_UART_SetRS485Flags(LPC_USART0, UART_RS485CTRL_DCTRL_EN | UART_RS485CTRL_OINV_1 );
	Chip_UART_SetRS485Addr(LPC_USART0, address); /* seteo la direccion de comparacion para la recepcion */
	/* dudas sobre la prioridad que se setea (mirar datasheet - capitulo NVIC) */
	NVIC_SetPriority(USART0_IRQn, UART_IER_RBRINT);
	/* Enable Interrupt for UART channel */
	NVIC_EnableIRQ(USART0_IRQn);
}


void uartWriteByte(uint8_t byte){

	while ((Chip_UART_ReadLineStatus(LPC_USART0) & UART_LSR_THRE) == 0); /* Wait for space in FIFO */
	Chip_UART_SendByte(LPC_USART0, byte);
	//ledSet(ON);
}

void uartWrite(uint8_t str[], uint8_t address){

	int i = 0;
   while(str[i]!= '\0'){
	  uartWriteByte(address);
	  uartWriteByte(str[i]);
	  i++;
   }
}

void uartRead2Bytes( uint16_t *dato){
	if (Chip_UART_ReadLineStatus(LPC_USART0) & UART_LSR_RDR)
		Chip_UART_Read(LPC_USART0, &dato, 2);

}

void uartWrite2Bytes(uint16_t dato){
	if (Chip_UART_ReadLineStatus(LPC_USART0) & UART_LSR_RDR)
		Chip_UART_Send(LPC_USART0, &dato, 2);

}

uint8_t uartReadByte (void){

   uint8_t receivedByte;

   if (Chip_UART_ReadLineStatus(LPC_USART0) & UART_LSR_RDR){
      receivedByte = Chip_UART_ReadByte(LPC_USART0);
   }
   return receivedByte;
}
