#include <s3c44b0x.h>
#include <s3cev40.h>
#include <common_types.h>
#include <system.h>
#include <leds.h>
#include <segs.h>
#include <uart.h>
#include <rtc.h>
#include <timers.h>
#include <keypad.h>
#include <lcd.h>
#include <ts.h>

#include "kernelcoop.h"

/////////////////////////////////////////////////////////////////////////////
// CONFIGURACION
/////////////////////////////////////////////////////////////////////////////

#define MAXPLACES              (8)
/////////////////////////////////////////////////////////////////////////////
// Declaracion de tipos
/////////////////////////////////////////////////////////////////////////////

typedef struct {

	boolean occupied;
	rtc_time_t end_time;
} plaza_t;

typedef struct {
	plaza_t plaza[MAXPLACES];
	uint16 recaudacion;
	boolean espanol;
	uint8 plazasLibres;
} parking_t;

/////////////////////////////////////////////////////////////////////////////
// Declaracion de funciones
/////////////////////////////////////////////////////////////////////////////

void setup(void);
void plotWelcomeScreen(void);
const char* obtenerDiaSemana(uint8 weekday);
void convertirFechaHora(rtc_time_t tiempoActual, char* buffer, char* diaSemana); // Función para convertir la fecha y hora en una cadena manualmente
void plotSelectionScreen(void); //Pantalla de seleccion de Plaza
uint8 plazaSeleccionada(void); //Nos devuelve la plaza elegida en funcion de la zona del ts presionada
void plotPaymentScreen(uint16 credit); //Pantalla de Pago
void plotOccupiedScreen(uint16 credit); //Pantalla de Plaza Ocupada,permite liberar
void printTicket(void); //Imprime por la uart el ticket de pago
void tiempoOcupado(uint16 credit); //Calcula el tiempo que se desea reservar la plaza en funcion del credito
void idiomaSeleccionado(void); //Devuelve que idioma hemos elegido en funcion de la zona del ts presionada
void idiomaScreen(void); //Pantalla de eleccion de idioma
void liberar(void); //Si la plaza esta Ocupada, esta la opcion de liberar, que si no te has pasado del tiempo la pone a libre, si te has pasado imprime una multa
void multar(uint8 plaza); //Imprime el ticket de multa
/////////////////////////////////////////////////////////////////////////////
// Declaracion de tareas
/////////////////////////////////////////////////////////////////////////////

void tsScanTask(void);
void coinsAcceptorTask(void);
void clockTask(void);
void coinsMoverTask(void);
void mainTask(void);
void ticketPrinterTask(void);

/////////////////////////////////////////////////////////////////////////////
// Declaracion de recursos
/////////////////////////////////////////////////////////////////////////////

parking_t parking;
uint8 plazaSelecc;

struct mbox1 { /* mailbox donde la coinsAcceptorTask indica a mainTask la moneda introducida */
	boolean flag;
	uint8 cents;
} coinAcceptorMsg;

struct mbox2 { /* mailbox donde la tsScanTask indica a mainTask la posicion de la pantalla presionada */
	boolean flag;
	uint16 x, y;
} tsPressedMsg;

struct mbox3 { /* mailbox donde la mainTask indica a coinsMoverTask si debe aceptar o devolver las monedas introducidas */
	boolean flag;
	boolean accept;
} coinsMoverMsg;

boolean printTicketFlag; /* mailbox donde la mainTask indica a ticketPrintTask que imprima un ticket */

/*******************************************************************/

void main(void) {

	sys_init();
	leds_init();
	segs_init();
	uart0_init();
	rtc_init();
	timers_init();
	keypad_init();
	lcd_init();
	ts_init();

	lcd_on();
	ts_on();

	setup();

	scheduler_init(); /* Inicializa el kernel */

	create_task(tsScanTask, 5); /* Crea las tareas de la aplicacion... */
	create_task(coinsAcceptorTask, 5); /* ... el kernel asigna la prioridad segun orden de creacion */
	create_task(mainTask, 10); /* ... las tareas mas frecuentes tienen mayor prioridad (criterio Rate-Monotonic-Scheduling) */
	create_task(coinsMoverTask, 10);
	create_task(clockTask, 10);
	create_task(ticketPrinterTask, 10);

	timer0_open_tick(scheduler, TICKS_PER_SEC); /* Instala scheduler como RTI del timer0  */

	while (1) {
		sleep(); /* Entra en estado IDLE, sale por interrupcion */
		dispacher(); /* Las tareas preparadas se ejecutan en esta hebra (background) en orden de prioridad */
	}

}

/*******************************************************************/

/*
 ** Tarea principal, se activa cada 100 ms muestreando los mensajes enviados de otras tareas y actuando en consecuencia
 */
void mainTask(void) {
	static boolean init = TRUE;
	static enum {
		demo_waiting, demo_acceptCoins, selection, payment, idioma
	} state;
	static uint16 credit;
	static uint16 ticks;
	char bufferFechaLiberacion[40];
	char diaSemana[4];

	if (init) {
		init = FALSE;
		plotWelcomeScreen();
		credit = 0;
		state = demo_waiting;

	} else
		switch (state) {
		case demo_waiting: /* Estado en donde rechaza monedas y espera la pulsacion de la pantalla */

			if (tsPressedMsg.flag) /* Chequea si se ha pulsado la pantalla (mensaje recibido de la tarea tsPressedTask) */
			{
				tsPressedMsg.flag = FALSE; /* Marca el mensaje como leido */
				lcd_clear(); /* Borra pantalla */
				idiomaScreen();
				state = idioma;
				ticks = 500; /* ... en el que debera permanecer un maximo de 500 ticks sin no hay actividad */
			}
			if (coinAcceptorMsg.flag) /* Chequea si se ha introducido una moneda (mensaje recibido de la tarea coinAcceptorTask) */
			{
				coinAcceptorMsg.flag = FALSE; /* Marca el mensaje como leido */
				coinsMoverMsg.accept = FALSE; /* Envia un mensaje para que la moneda se devuelva */
				coinsMoverMsg.flag = TRUE;
			}
			break;
		case idioma:

			if (!(--ticks)) //Decrementa ticks y chequea si ha permanecido en este estado el tiempo maximo
			{
				plotWelcomeScreen(); // Visualiza pantalla inicial
				state = demo_waiting; // Salta al estado demo_waiting
			}
			if (tsPressedMsg.flag) {
				tsPressedMsg.flag = FALSE; /* Marca el mensaje como leído */
				lcd_clear();
				idiomaSeleccionado();
				plotSelectionScreen();
				state = selection;
				ticks = 500; /* ... en el que debera permanecer un maximo de 500 ticks sin no hay actividad */
			}
			break;

		case selection:

			if (!(--ticks)) //Decrementa ticks y chequea si ha permanecido en este estado el tiempo maximo
			{
				plotWelcomeScreen(); // Visualiza pantalla inicial
				state = demo_waiting; // Salta al estado demo_waiting
			}
			if (tsPressedMsg.flag) {
				tsPressedMsg.flag = FALSE; /* Marca el mensaje como leído */
				plazaSelecc = plazaSeleccionada();
				lcd_clear();
				state = payment; /* Cambia al siguiente estado */
				ticks = 500; /* Inicializa el temporizador */
			}
			break;
		case payment:

			if (!(--ticks)) //Decrementa ticks y chequea si ha permanecido en este estado el tiempo maximo
			{
				plotWelcomeScreen(); // Visualiza pantalla inicial
				state = demo_waiting; // Salta al estado demo_waiting
			}
			if (parking.plaza[plazaSelecc - 1].occupied) {
				plotOccupiedScreen(credit);
				if (tsPressedMsg.flag) /* Chequea si se ha pulsado la pantalla (mensaje recibido de la tarea tsPressedTask) */
				{
					tsPressedMsg.flag = FALSE;
					liberar();
					sw_delay_s(5);
					lcd_clear();
					plotSelectionScreen();
					state = selection;
					ticks = 500;
				}
			} else {
				plotPaymentScreen(credit);
				state = demo_acceptCoins;
				ticks = 500;
			}
			break;
		case demo_acceptCoins: /* Estado en deonde acepta monedas y espera la pulsacion de la pantalla durante un tiempo maximo */
			if (!(--ticks)) /* Decrementa ticks y chequea si ha permanecido en este estado el tiempo maximo */
			{
				plotWelcomeScreen(); /* Visualiza pantalla inicial */
				state = demo_waiting; /* Salta al estado demo_waiting */
			}
			if (tsPressedMsg.flag) /* Chequea si se ha pulsado la pantalla (mensaje recibido de la tarea tsPressedTask) */
			{
				tsPressedMsg.flag = FALSE; /* Marca el mensaje como leido*/

				rtc_time_t tiempoActual;
				rtc_gettime(&tiempoActual);

				if (credit < 20) {
					coinsMoverMsg.accept = FALSE; /* Envia un mensaje para que las monedas se acepten */
					coinsMoverMsg.flag = TRUE;
					parking.plaza[plazaSelecc - 1].end_time = tiempoActual;
					lcd_clear();
					if (parking.espanol) {
						lcd_puts_x2(20, 100, BLACK, "Saldo Insuficiente");
					} else {
						lcd_puts_x2(20, 100, BLACK, "Insuficient Balance");
					}
					sw_delay_s(2);

				} else if (credit > 240) {
					coinsMoverMsg.accept = FALSE; /* Envia un mensaje para que las monedas se acepten */
					coinsMoverMsg.flag = TRUE;
					parking.plaza[plazaSelecc - 1].end_time = tiempoActual;
					lcd_clear();
					if (parking.espanol) {
						lcd_puts_x2(50, 100, BLACK, "Saldo Maximo");
						lcd_puts_x2(50, 150, BLACK, "  Superado ");
					} else {
						lcd_puts_x2(50, 100, BLACK, "Maximum balance");
						lcd_puts_x2(50, 150, BLACK, "Exceeded");
					}
					sw_delay_s(5);
				} else {
					coinsMoverMsg.accept = TRUE; /* Envia un mensaje para que las monedas se acepten */
					coinsMoverMsg.flag = TRUE;
					parking.recaudacion += credit;
					parking.plaza[plazaSelecc - 1].occupied = TRUE;
					printTicketFlag = TRUE;
					parking.plazasLibres--;
				}
				credit = 0;
				plotWelcomeScreen(); /* Visualiza pantalla inicial */
				state = demo_waiting; /* Salta al estado demo_waiting */

			}
			if (coinAcceptorMsg.flag) /* Chequea si se ha introducido una moneda (mensaje recibido de la tarea coinAcceptorTask) */
			{
				coinAcceptorMsg.flag = FALSE; /* Marca el mensaje como leido */
				credit += coinAcceptorMsg.cents; /* Incrementa el saldo */

				tiempoOcupado(credit);
				rtc_time_t end_time = parking.plaza[plazaSelecc - 1].end_time;
				convertirFechaHora(end_time, bufferFechaLiberacion, diaSemana);

				lcd_putint(200, 140, BLACK, credit);
				lcd_puts(100, 155, BLACK, bufferFechaLiberacion);
				ticks = 500; /* Restaura el tiempo maximo sin actividad que permanece en este estado */
			}
			break;
		}
}

/*
 ** Emula el comportamiento de la impresora de tickets:
 **   Cada segundo muestrea si ha recibido un mensaje de la tarea principal enviar a traves de la UART el texto del ticket del aparcamiento elegido
 */
void ticketPrinterTask(void) {
	if (printTicketFlag) {
		printTicket();
		printTicketFlag = FALSE;
	}
}
void printTicket(void) {
	char ticketHeader[] = "----- TICKET DE APARCAMIENTO -----\n";
	char ticketFooter[] = "----------------------------------\n";
	char buffer[40];
	char diaSemana[4];

	uart0_puts(ticketHeader);              // Encabezado del ticket
	uart0_puts("Plaza: ");
	uart0_putint(plazaSelecc);                    // Número de plaza
	uart0_puts("\n");

	uart0_puts("Fin del estacionamiento: ");
	convertirFechaHora(parking.plaza[plazaSelecc - 1].end_time, buffer,
			diaSemana);
	uart0_puts("\n");
	uart0_puts(buffer);
	uart0_puts("\n");

	uart0_puts("Gracias por su visita\n"); // Mensaje adicional
	uart0_puts(ticketFooter);              // Pie del ticket
}

/*
 ** Cada segundo visualiza la fecha/hora en la pantalla y libera aquellas plazas cuya hora de finalizacion haya pasado
 */
void clockTask(void) {
	static rtc_time_t tiempoActual; // Estructura para guardar el tiempo actual
	static char bufferFechaHora[40]; // Buffer para almacenar la fecha y hora
	static char diaSemana[4]; // Buffer para el día de la semana (3 caracteres + terminador)

	rtc_gettime(&tiempoActual);

// Convertir la fecha y hora en un formato de cadena
	convertirFechaHora(tiempoActual, bufferFechaHora, diaSemana);
	lcd_puts(40, 10, BLACK, bufferFechaHora);

}

/*
 ** Cada 50 ms muestrea la touchscreen y envia un mensaje a la tarea principal con la posicion del lugar pulsado
 */
void tsScanTask(void) {
	static boolean init = TRUE;
	static enum {
		wait_keydown, scan, wait_keyup
	} state;

	if (init) {
		init = FALSE;
		state = wait_keydown;
	} else
		switch (state) {
		case wait_keydown: /* Estado esperando la presion de la pantalla */
			if (ts_pressed()) /* Chequea si la pantalla esta presionada */
				state = scan; /* Salta al estado scan */
			break;

		case scan: /* Estado en que escanea la pantalla */
			ts_getpos(&tsPressedMsg.x, &tsPressedMsg.y); /* Lee la pantalla */
			tsPressedMsg.flag = TRUE; /* Envia un mensaje con el valor de la posición presionada */
			state = wait_keyup; /* Salta al estado wait_keyup */
			break;

		case wait_keyup: // Estado esperando la depresion de la pantalla
			if (!ts_pressed()) // Chequea si la pantalla ya no esta presionada
				state = wait_keydown; // Salta al estado wait_keydown
			break;
		}
}

/*
 ** Emula el comportamiento de un reconocedor de monedas:
 **   Cada 50 ms muestrea el keypad y envia un mensaje a la tarea principal con el valor de la moneda asociada a la tecla
 */
void coinsAcceptorTask(void) {
	static boolean init = TRUE;
	static enum {
		wait_keydown, scan, wait_keyup
	} state;

	if (init) {
		init = FALSE;
		state = wait_keydown;
	} else
		switch (state) {
		case wait_keydown: /* Estado esperando la presion teclas */
			if (keypad_pressed()) /* Chequea si hay una tecla presionada */
				state = scan; /* Salta al estado scan */
			break;
		case scan: /* Estado en que escanea el teclado */
			switch (keypad_scan()) /* Lee el teclado */
			{
			case KEYPAD_KEY3: /* La tecla F esta asociada a la moneda de 0,10 euros */
				coinAcceptorMsg.cents = 10; /* Envia un mensaje con el valor de la moneda */
				coinAcceptorMsg.flag = TRUE;
				break;
			case KEYPAD_KEY2:
				coinAcceptorMsg.cents = 20; /* Envia un mensaje con el valor de la moneda */
				coinAcceptorMsg.flag = TRUE;
				break;
			case KEYPAD_KEY1:
				coinAcceptorMsg.cents = 50; /* Envia un mensaje con el valor de la moneda */
				coinAcceptorMsg.flag = TRUE;
				break;
			case KEYPAD_KEY0:
				coinAcceptorMsg.cents = 100; /* Envia un mensaje con el valor de la moneda */
				coinAcceptorMsg.flag = TRUE;
				break;
			}
			state = wait_keyup; /* Salta al estado wait_keyup */
			break;
		case wait_keyup: /* Estado esperando la depresion de teclas  */
			if (!keypad_pressed()) /* Chequea si la tecla ya no esta presionada */
				state = wait_keydown; /* Salta al estado wait_keydown */
			break;
		}
}

/*
 ** Emula el comportamiento del dispositivo que envia las monedas a la alcancia o al cajetin de devolucion:
 **   Cada 100 ms muestrea si ha recibido un mensaje de la tarea principal para mover monedas
 **   Si van a la alcancia activa durante 1 s los leds y muestra una A en los segs
 **   Si van al cajetin de devolucion activa durante 1 segundo los leds y muestra una D en los segs
 */
void coinsMoverTask(void) {
	static boolean init = TRUE;
	static enum {
		off, on
	} state;
	static uint32 ticks;

	if (init) {
		init = FALSE;
		state = off;
	} else
		switch (state) {
		case off: /* Estado con leds y 7-segs apagados */
			if (coinsMoverMsg.flag) /* Chequea si ha recibido mensaje */
			{
				coinsMoverMsg.flag = FALSE; /* Marca el mensaje como leido */
				if (coinsMoverMsg.accept) /* Muestra el mensaje que corresponda en segs */
					segs_putchar(0xA);
				else
					segs_putchar(0xD);
				led_on(LEFT_LED); /* Enciende leds */
				led_on(RIGHT_LED);
				state = on; /* Salta al estado on ... */
				ticks = 10; /* ... en el que debera permanecer 10 ticks */
			}
			break;
		case on: /* Estado con leds y 7-segs encendidos */
			if (!(--ticks)) /* Decrementa ticks y chequea si ha permanecido en este estado el tiempo requerido */
			{
				segs_off(); /* Apaga segs */
				led_off(LEFT_LED); /* Apaga leds */
				led_off(RIGHT_LED);
				state = off; /* Salta al estado off */
			}
			break;
		}
}

/*******************************************************************/

/*
 ** Inicializa flags, mailboxes y variables globales
 */
void setup(void) {
	coinAcceptorMsg.flag = FALSE;
	coinAcceptorMsg.cents = 0;

	tsPressedMsg.flag = FALSE;
	tsPressedMsg.x = 0;
	tsPressedMsg.y = 0;

	coinsMoverMsg.flag = FALSE;
	coinsMoverMsg.accept = FALSE;
	parking.plazasLibres=MAXPLACES;
	printTicketFlag = FALSE;
	uint8 i;
	for (i = 0; i < MAXPLACES; i++) {

		parking.plaza[i].occupied = FALSE; // Inicialmente todas las plazas están libres
	}
	parking.recaudacion = 0;
}

void plotWelcomeScreen(void) {
	const uint16 X_HORARIO = 10; // Posición inicial del horario de funcionamiento
	const uint16 Y_HORARIO = 120;

//Limpiar la pantalla con el color de fondo:
	lcd_clear();
	lcd_puts_x2(24, 40, BLACK, "Pulse la pantalla");
	lcd_puts_x2(24, 68, BLACK, "  para comenzar  ");

	lcd_puts(24, 100, BLACK, "Plazas libres: ");
	lcd_putint(150, 100, BLACK, parking.plazasLibres);
	lcd_puts(170, 100, BLACK, "Recaudacion: ");
	lcd_putint(280, 100, BLACK, parking.recaudacion);

// Dibujar el horario de funcionamiento
	lcd_draw_box(0, 125, LCD_WIDTH - 1, 220, BLACK, 1); // Limpiar área donde se dibuja la fecha y hora
	lcd_puts(X_HORARIO + 30, Y_HORARIO, BLACK, "HORARIO DE FUNCIONAMIENTO:");

	lcd_puts(X_HORARIO, Y_HORARIO + 20, BLACK, "dom: gratis");
	lcd_puts(X_HORARIO, Y_HORARIO + 40, BLACK, "lun: 09:00-21:00");
	lcd_puts(X_HORARIO, Y_HORARIO + 60, BLACK, "mar: 09:00-21:00");
	lcd_puts(X_HORARIO, Y_HORARIO + 80, BLACK, "mie: 09:00-21:00");
	lcd_puts(X_HORARIO + 140, Y_HORARIO + 20, BLACK, "jue: 09:00-21:00");
	lcd_puts(X_HORARIO + 140, Y_HORARIO + 40, BLACK, "vie: 09:00-21:00");
	lcd_puts(X_HORARIO + 140, Y_HORARIO + 60, BLACK, "sab: 09:00-15:00");

}

// Función auxiliar para obtener el día de la semana en texto
const char* obtenerDiaSemana(uint8 weekday) {
	switch (weekday) {
	case 0:
		return "dom";
	case 1:
		return "lun";
	case 2:
		return "mar";
	case 3:
		return "mie";
	case 4:
		return "jue";
	case 5:
		return "vie";
	case 6:
		return "sab";
	default:
		return "???";
	}
}

void convertirFechaHora(rtc_time_t tiempoActual, char* buffer, char* diaSemana) {
// Obtener el nombre del día de la semana y almacenarlo en diaSemana
	const char* nombreDia = obtenerDiaSemana(tiempoActual.wday);
	int i = 0;

// Copiar el nombre del día al buffer diaSemana
	while (*nombreDia) {
		diaSemana[i++] = *nombreDia++;
	}
	diaSemana[i] = '\0';  // Terminar la cadena

// Construir la cadena de fecha y hora
// Formato: "Día, dd/mm/aa hh:mm:ss"
	int pos = 0;

// Día de la semana
	i = 0;
	while (diaSemana[i]) {
		buffer[pos++] = diaSemana[i++];
	}
	buffer[pos++] = ',';
	buffer[pos++] = ' ';

// Día
	buffer[pos++] = '0' + (tiempoActual.mday / 10); // Primer dígito del día
	buffer[pos++] = '0' + (tiempoActual.mday % 10); // Segundo dígito del día
	buffer[pos++] = '/';

// Mes
	buffer[pos++] = '0' + (tiempoActual.mon / 10);  // Primer dígito del mes
	buffer[pos++] = '0' + (tiempoActual.mon % 10); // Segundo dígito del mes
	buffer[pos++] = '/';

// Año (últimos dos dígitos)
	buffer[pos++] = '0' + (tiempoActual.year % 100) / 10; // Primer dígito del año
	buffer[pos++] = '0' + (tiempoActual.year % 100) % 10; // Segundo dígito del año
	buffer[pos++] = ' ';

// Hora
	buffer[pos++] = '0' + (tiempoActual.hour / 10); // Primer dígito de la hora
	buffer[pos++] = '0' + (tiempoActual.hour % 10); // Segundo dígito de la hora
	buffer[pos++] = ':';

// Minutos
	buffer[pos++] = '0' + (tiempoActual.min / 10); // Primer dígito de los minutos
	buffer[pos++] = '0' + (tiempoActual.min % 10); // Segundo dígito de los minutos
	buffer[pos++] = ':';

// Segundos
	buffer[pos++] = '0' + (tiempoActual.sec / 10); // Primer dígito de los segundos
	buffer[pos++] = '0' + (tiempoActual.sec % 10); // Segundo dígito de los segundos

// Terminar la cadena
	buffer[pos] = '\0';
}

void plotSelectionScreen(void) {

	if (parking.espanol) {
		lcd_puts_x2(20, 35, BLACK, "Selecciona plaza");
	} else {
		lcd_puts_x2(20, 35, BLACK, "Select place");
	}
	lcd_draw_box(0, 80, 79, 139, BLACK, 1); /* Pinta cuadricula */
	lcd_draw_box(79, 80, 159, 139, BLACK, 1);
	lcd_draw_box(159, 80, 239, 139, BLACK, 1);
	lcd_draw_box(239, 80, 319, 139, BLACK, 1);
	lcd_draw_box(0, 139, 79, 199, BLACK, 1);
	lcd_draw_box(79, 139, 159, 199, BLACK, 1);
	lcd_draw_box(159, 139, 239, 199, BLACK, 1);
	lcd_draw_box(239, 139, 319, 199, BLACK, 1);

	uint16 COL = 86;
	uint8 i;
	uint8 plaza = 1;
	for (i = 0; i < 2; i++) {
		uint8 j;
		uint16 FILA = 32;
		for (j = 0; j < 4; j++) {
			if (!parking.plaza[plaza - 1].occupied) {
				lcd_putint_x2(FILA, COL, BLACK, plaza);
			} else {
				lcd_putchar_x2(FILA, COL, BLACK, 'X');
			}
			FILA += 80;
			plaza++;
		}
		COL += 60;
	}
}

void idiomaSeleccionado(void) {

	if (tsPressedMsg.x > 0 && tsPressedMsg.y > 80 && tsPressedMsg.x < 150
			&& tsPressedMsg.y < 150) {
		parking.espanol = TRUE;
	} else if (tsPressedMsg.x > 160 && tsPressedMsg.y > 80
			&& tsPressedMsg.x < 319 && tsPressedMsg.y < 150) {
		parking.espanol = FALSE;
	}
}

void idiomaScreen(void) {

	lcd_puts_x2(20, 100, BLACK, "ESPANOL");
	lcd_puts_x2(150, 100, BLACK, "||");
	lcd_puts_x2(200, 100, BLACK, "ENGLISH");
}

uint8 plazaSeleccionada(void) { //Función que devuelve que plaza ha sido pulsada

	uint8 sol;

	if (tsPressedMsg.x > 0 && tsPressedMsg.y > 80 && tsPressedMsg.x < 79
			&& tsPressedMsg.y < 139) {
		sol = 1;
	} else if (tsPressedMsg.x > 79 && tsPressedMsg.y > 80
			&& tsPressedMsg.x < 159 && tsPressedMsg.y < 139) {
		sol = 2;
	} else if (tsPressedMsg.x > 159 && tsPressedMsg.y > 80
			&& tsPressedMsg.x < 239 && tsPressedMsg.y < 139) {
		sol = 3;
	} else if (tsPressedMsg.x > 239 && tsPressedMsg.y > 80
			&& tsPressedMsg.x < 319 && tsPressedMsg.y < 139) {
		sol = 4;
	} else if (tsPressedMsg.x > 0 && tsPressedMsg.y > 139 && tsPressedMsg.x < 79
			&& tsPressedMsg.y < 199) {
		sol = 5;
	} else if (tsPressedMsg.x > 79 && tsPressedMsg.y > 139
			&& tsPressedMsg.x < 159 && tsPressedMsg.y < 199) {
		sol = 6;
	} else if (tsPressedMsg.x > 159 && tsPressedMsg.y > 139
			&& tsPressedMsg.x < 239 && tsPressedMsg.y < 199) {
		sol = 7;
	} else if (tsPressedMsg.x > 239 && tsPressedMsg.y > 139
			&& tsPressedMsg.x < 319 && tsPressedMsg.y < 199) {
		sol = 8;
	}
	return sol;
}

void plotPaymentScreen(uint16 credit) {
	if (parking.espanol) {
// Mostrar tarifas
		lcd_draw_box(0, 35, LCD_WIDTH - 1, 95, BLACK, 1); // Limpiar área
		lcd_puts(110, 30, BLACK, "TARIFA:");
		lcd_puts(45, 45, BLACK, "Precio por minuto: 0,01");
		lcd_puts(35, 60, BLACK, "Estancia Minima: 20 minutos");
		lcd_puts(35, 75, BLACK, "Estancia Maxima: 240 minutos");

		lcd_puts_x2(80, 110, BLACK, "Plaza: ");
		lcd_putint_x2(200, 110, BLACK, plazaSelecc);
		lcd_puts(80, 140, BLACK, "Credito: ");

// Mostrar fecha y hora de liberación
		lcd_puts(50, 155, BLACK, "Fin:");

// Instrucciones
		lcd_puts(80, 190, BLACK, "Inserte monedas");
		lcd_puts(50, 210, BLACK, "Pulse pantalla para aceptar");
	} else {
		lcd_draw_box(0, 35, LCD_WIDTH - 1, 95, BLACK, 1);
		lcd_puts(110, 30, BLACK, "TARIFF:");
		lcd_puts(45, 45, BLACK, "Price per minute: 0,01");
		lcd_puts(35, 60, BLACK, "Minimum Stay: 20 minutes");
		lcd_puts(35, 75, BLACK, "Maximum Stay: 240 minutes");

		lcd_puts_x2(30, 110, BLACK, "Parking space: ");
		lcd_putint_x2(260, 110, BLACK, plazaSelecc);
		lcd_puts(80, 140, BLACK, "Credit: ");

		lcd_puts(50, 155, BLACK, "End:");

		lcd_puts(80, 190, BLACK, "Insert coins");
		lcd_puts(50, 210, BLACK, "Press screen to accept");
	}
}

void liberar(void) {
	static rtc_time_t tiempoActual; // Estructura para guardar el tiempo actual

	rtc_gettime(&tiempoActual);

	if (tsPressedMsg.x > 80 && tsPressedMsg.y > 50 && tsPressedMsg.x < 250
			&& tsPressedMsg.y < 95) {

		if (parking.plaza[plazaSelecc - 1].end_time.hour <= tiempoActual.hour
				&& parking.plaza[plazaSelecc - 1].end_time.min
						<= tiempoActual.min
				&& parking.plaza[plazaSelecc - 1].end_time.sec
						<= tiempoActual.sec) {
			multar(plazaSelecc - 1);

		}
		parking.plaza[plazaSelecc - 1].occupied = FALSE;
		parking.plazasLibres++;
	}
}

void plotOccupiedScreen(uint16 credit) {
	char bufferFechaLiberacion[40];
	char diaSemana[4];

// Convertir la fecha y hora de liberación a cadena
	rtc_time_t end_time = parking.plaza[plazaSelecc - 1].end_time;
	convertirFechaHora(end_time, bufferFechaLiberacion, diaSemana);
	if (parking.espanol) {
		lcd_puts_x2(140, 60, BLACK, "LIBERAR");
		lcd_puts_x2(30, 110, BLACK, "Plaza ");
		lcd_putint_x2(120, 110, BLACK, plazaSelecc);
		lcd_puts_x2(140, 110, BLACK, " ocupada ");
// Mostrar fecha y hora de liberación
		lcd_puts(60, 150, BLACK, "Fin:");
		lcd_puts(110, 150, BLACK, bufferFechaLiberacion);
	} else {
		lcd_puts_x2(140, 60, BLACK, "FREE");
		lcd_puts_x2(30, 110, BLACK, "Place ");
		lcd_putint_x2(120, 110, BLACK, plazaSelecc);
		lcd_puts_x2(140, 110, BLACK, " Occupied ");
// Mostrar fecha y hora de liberación
		lcd_puts(60, 150, BLACK, "End:");
		lcd_puts(110, 150, BLACK, bufferFechaLiberacion);
	}

}

void tiempoOcupado(uint16 credit) {

	rtc_time_t current_time, end_time;
	rtc_gettime(&current_time);

	end_time = current_time;

	if (credit + end_time.min < 60) {
		end_time.min += credit;
	} else {
		end_time.hour = ((credit + end_time.min) / 60);
		end_time.min = ((credit + end_time.min) % 60);
	}

	parking.plaza[plazaSelecc - 1].end_time = end_time;
}

void multar(uint8 plaza) {

	char ticketHeader[] = "----- TICKET DE MULTA -----\n";
	char ticketFooter[] = "----------------------------------\n";

	uart0_puts(ticketHeader);              // Encabezado del ticket
	uart0_puts("Plaza: ");
	uart0_putint(plazaSelecc);                    // Número de plaza
	uart0_puts("\n");

	uart0_puts("Tiene una multa de 50€ ");
	uart0_puts("\n");

	uart0_puts("Acuda a ventana para pagar\n"); // Mensaje adicional
	uart0_puts(ticketFooter);              // Pie del ticket
}
