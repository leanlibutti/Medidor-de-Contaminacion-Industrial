/*
 * teclado.h
 *
 *
 */
#include "lpc_types.h"

//Uso el P6 ya que los GPIO asignados al teclado matricial son para uno de 4x3 y se dispone de uno de 4x4.

#ifndef TECLADO_H_
#define TECLADO_H_


typedef struct __TECLADO_DATA_Type {
	Bool FIL0;
	Bool FIL1;
	Bool FIL2;
	Bool FIL3;
	Bool COL0;
	Bool COL1;
	Bool COL2;
} TECLADO_DATA_Type;

#define T_PORTFIL01    3
#define T_PORTFIL2     7
#define T_PORTFIL3     4
#define T_PORTCOL0     4
#define T_PORTCOL1     4
#define T_PORTCOL2     1
#define T_FIL0	   1
#define T_FIL1     2
#define T_FIL2     4
#define T_FIL3     0
#define T_COL0     3
#define T_COL1     2
#define	T_COL2	   5


void TECLADO_init_port(void);
void TECLADO_init(void);
unsigned char TECLADO_ver_columna(void);
unsigned char TECLADO_ver_tecla(void);
void enviar_TECLADO_FILA(TECLADO_DATA_Type);

#endif /* TECLADO_H_ */
