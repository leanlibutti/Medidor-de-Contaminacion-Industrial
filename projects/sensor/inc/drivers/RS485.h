#include "chip.h"
#include "stdint.h"
#include "string.h"

void rsConfig(uint8_t address);

void uartWriteByte(uint8_t byte);

void uartWrite(uint8_t str[], uint8_t address);

uint8_t uartReadByte(void);

void uartRead2Bytes( uint16_t *);

void uartWrite2Bytes( uint16_t );

