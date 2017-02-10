/*
 * teclado.c
 *     
 */
 
#include "teclado.h"
#include "puertos.h"
TECLADO_DATA_Type data;



const unsigned char teclas[4][3] = {
			{'1','2','3'},
			{'4','5','6'},
			{'7','8','9'},
			{'*','0','#'}
	};


void TECLADO_init_port(void)
{
     port_pin(T_PORTFIL01,T_FIL0,MD_PLN|MD_ZI|MD_ZI,init_out);
     port_pin(T_PORTFIL01,T_FIL1,MD_PLN|MD_ZI|MD_ZI,init_out);
     port_pin(T_PORTFIL2,T_FIL2,MD_PLN|MD_ZI|MD_ZI,init_out);
     port_pin(T_PORTFIL3,T_FIL3,MD_PLN|MD_ZI|MD_ZI,init_out);
     port_pin(T_PORTCOL0,T_COL0,MD_PUP|MD_EZI|MD_ZI,init_in); //preguntar si las comlumnas no serian init_in ya que se configuran como entradas.
     port_pin(T_PORTCOL1,T_COL1,MD_PUP|MD_EZI|MD_ZI,init_in);
     port_pin(T_PORTCOL2 ,T_COL2,MD_PUP|MD_EZI|MD_ZI,init_in);
   
}


/*
void TECLADO_init(void){
	//Se configuran las columnas como entradas y las filas como salidas
	//Se habilita el pull-up enable para las entradas
	 port_pin(4,T_COL0,enable,init_in);
     port_pin(4,T_COL1,enable,init_in);
     port_pin(1,T_COL2,enable,init_in);
     port_pin(3,T_FIL0,disable,init_out);
     port_pin(3,T_FIL1,disable,init_out);
     port_pin(7,T_FIL2,disable,init_out);
     port_pin(4,T_FIL3,disable,init_out);
	
}
*/

unsigned char TECLADO_ver_columna(void){
	if(!data.COL0){	//preguntar si se esta chequeando correctamente las columnas hoy hay q hacer algo para que se actualice el valor de T_COL0
		return 0;
	}else if(!data.COL1) {
		return 1;
	}else if(!data.COL2){
		return 2;
	} else{
		return ' ';
	}
	//devuelve el numero de la columna que se esta presionando y si no se esta presionando nada devuelve espacio
	//como control para que no se detecten teclas oprimidas cuando no hay nada presionado.
}


unsigned char TECLADO_ver_tecla(void){
	static unsigned char tecla, anterior=' ';  //chequeo para el rebote del pulsador
	volatile unsigned char columna = ' ',solto=0,T=' ';  //solto: Chequeo para que no tome otra letra hasta que no se deje de presionar la actual
														 //columna: Devuelve la columna oprimida
	
	data.FIL0 = 0;
	data.FIL1 = 1;
	data.FIL2 = 1;
	data.FIL3 = 1;
	enviar_TECLADO_FILA(data);	//preguntar si es necesario y si al actualizar el valor de las filas se esta actualizando el valor de las columnas.
	columna = TECLADO_ver_columna();
	if(columna!=' ') T=teclas[0][columna];
	

	data.FIL0 = 1;
	data.FIL1 = 0;
	data.FIL2 = 1;
	data.FIL3 = 1;
	enviar_TECLADO_FILA(data);	//preguntar si es necesario y si al actualizar el valor de las filas se esta actualizando el valor de las columnas.
	columna = TECLADO_ver_columna();
	if(columna!=' ') T=teclas[1][columna];
	

	data.FIL0 = 1;
	data.FIL1 = 1;
	data.FIL2 = 0;
	data.FIL3 = 1;
	enviar_TECLADO_FILA(data);	//preguntar si es necesario y si al actualizar el valor de las filas se esta actualizando el valor de las columnas.
	columna = TECLADO_ver_columna();
	if(columna!=' ') T=teclas[2][columna];
	

	data.FIL0 = 1;
	data.FIL1 = 1;
	data.FIL2 = 1;
	data.FIL3 = 0;
	enviar_TECLADO_FILA(data);	//preguntar si es necesario y si al actualizar el valor de las filas se esta actualizando el valor de las columnas.
	columna = TECLADO_ver_columna();
	if(columna!=' ') T=teclas[3][columna];
	
	
	if((T==' ') && (anterior!=' '))
	  solto=1;
	else
	    tecla=T;	  
	
	anterior= T;
	  
	if(solto==1)
	  return tecla;
	else
	  return ' ';

}

void enviar_TECLADO_FILA (TECLADO_DATA_Type d){
   if (d.FIL0==0)
   		port_pin(T_PORTFIL01,T_FIL0,disable,high);

	else		
		port_pin(T_PORTFIL01,T_FIL0,disable,low);
    
    if (d.FIL1==0)
		port_pin(T_PORTFIL01,T_FIL1,disable,high);
    else
    	port_pin(T_PORTFIL01,T_FIL1,disable,low);
   if (d.FIL2==0)
		port_pin(T_PORTFIL2,T_FIL2,disable,high);
   	else		
   		port_pin(T_PORTFIL2,T_FIL2,disable,low);
   if (d.FIL3==0)
   		port_pin(T_PORTFIL3,T_FIL3,disable,high);
	else		
		port_pin(T_PORTFIL3,T_FIL3,disable,low);
   
}

//Preguntar si hay que hacer algo estilo enviar_LCD
