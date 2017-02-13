/*
 * serial.h
 *
 *  Created on: Oct 4, 2016
 *      Author: pablo
 */

#ifndef PROJECTS_BAREMETAL_INC_SERIAL_H_
#define PROJECTS_BAREMETAL_INC_SERIAL_H_

#include "board.h"

void serialSetup(void);
int32_t serialSend(void * data, uint32_t datalen);
int32_t serialRead(void * data, uint32_t datalen);

#endif /* PROJECTS_BAREMETAL_INC_SERIAL_H_ */
