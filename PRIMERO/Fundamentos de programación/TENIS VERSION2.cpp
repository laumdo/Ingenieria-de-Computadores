//LAURA
#include <iostream>
#include <ctime> //Se usa con frecuencia para conocer el tiempo exacto durante un programa.
#include <string> //Generalización de las cadenas alfanuméricas para albergar cadenas de objetos.
#include <cstdlib>
#include <iomanip>
#include <cmath>

using namespace std;

//Declaración de constantes:
const int LIM_INF_HAB = 1;
const int LIM_SUP_HAB = 2;
const int LIM_INF_VEL = 1;
const int LIM_SUP_VEL = 3;
const int ANCHO_PISTA = 7;
const bool JUEGO_ALEATORIO = false;
const bool MODO_DEBUG = false;
const int DIM_ARRAY_GOLPES = ANCHO_PISTA + 2;
const int JUEGOS_SET = 3;
//Tipo enumerado
typedef enum { NADIE, TENISTA1, TENISTA2 } tTenista;
typedef enum { NADA, QUINCE, TREINTA, CUARENTA, VENTAJA } tPuntosJuego;
//Array
typedef double tConteoGolpes[DIM_ARRAY_GOLPES];

//Prototipos de funciones:
void introducirTenista(string& iniciales, int& habilidad, int& velocidad);
tTenista saqueAleatorio(string iniciales_t1, string iniciales_t2);
string puntosAstring(tPuntosJuego puntuacion);
void pintarMarcador(string nombre1, string nombre2, tPuntosJuego puntos1, tPuntosJuego puntos2, int juegos1, int juegos2, tTenista servicio_para);
int golpeoBola(int posicion_tenista, int habilidad);
int correTenista(int posicion_tenista, int velocidad, int posicion_bola);
int peloteo(string t_1, string t_2, int jugador_actual, int habilidad_t1, int habilidad_t2, int velocidad_t1, int velocidad_t2);
void horizontalesCampo();
void verticalesCampo();
void pintarTenista(string nombre, int pos_tenista);
void pintarBola(int posicion_bola);
void pintarPeloteo(string nombre1, string nombre2, int pos_t1, int pos_t2, tTenista bola_jugador, int pos_bola);
tTenista lance(tTenista tenista_golpea, string nombre, int habilidad, tConteoGolpes golpes, int& golpes_ganados, int velocidad, int& pos_recibe, int& pos_bola);
tTenista jugarPunto(tTenista servicio, string nombre1, int habilidad1, int velocidad1, tConteoGolpes golpes1, int& golpes_ganados1, string nombre2, int habilidad2, int velocidad2, tConteoGolpes golpes2, int& golpes_ganados2);
tTenista hayGanadorSet(int juegos1, int juegos2);
tTenista jugarJuego(tTenista servicio, string nombre1, int habilidad1, int velocidad1, int& juegos1, tConteoGolpes golpes1, int& golpes_ganados1, string nombre2, int habilidad2, int velocidad2, int& juegos2, tConteoGolpes golpes2, int& golpes_ganados2);
tTenista actualizarMarcador(tTenista ganador_punto, tPuntosJuego& puntos1, tPuntosJuego& puntos2, int& juegos1, int& juegos2);
void mostrarEstadisticas(string nombre, tConteoGolpes golpes, int golpes_ganadores);

int main() {
	string iniciales_t1, iniciales_t2, ganador;
	int habilidad_t1, habilidad_t2, velocidad_t1, velocidad_t2;
	int juegos1 = 0, juegos2 = 0;
	int golpes_ganados1 = 0, golpes_ganados2 = 0;
	tTenista saque = NADIE, ganador_set = NADIE;
	tConteoGolpes golpes1, golpes2;
	tPuntosJuego puntuacion = NADA;

	cout << "Bienvenido al simulador de tenis." << endl;
	cout << endl;
	//Pedimos a los jugadrores que introduzcan sus datos.
	cout << "Introduzca los datos del jugador 1: " << endl;
	introducirTenista(iniciales_t1, habilidad_t1, velocidad_t1);
	cout << endl;
	cout << "Introduzca los datos del jugador 2: " << endl;
	introducirTenista(iniciales_t2, habilidad_t2, velocidad_t2);
	cout << endl;

	//Determinar aleatoriamente el saque.
	saque = saqueAleatorio(iniciales_t1, iniciales_t2);
	cout << endl;

	//Jugar 
	while (ganador_set == NADIE) {
		//Reinicio de arrays en cada juego:
		for (int i = 0; i <= ANCHO_PISTA + 1; i++) {
			golpes1[i] = 0;
		}
		for (int j = 0; j <= ANCHO_PISTA + 1; j++) {
			golpes2[j] = 0;
		}

		jugarJuego(saque, iniciales_t1, habilidad_t1, velocidad_t1, juegos1, golpes1, golpes_ganados1, iniciales_t2, habilidad_t2, velocidad_t2, juegos2, golpes2, golpes_ganados2);
		mostrarEstadisticas(iniciales_t1, golpes1, golpes_ganados1);
		cout << endl;
		mostrarEstadisticas(iniciales_t2, golpes2, golpes_ganados2);
		cout << endl;
		ganador_set = hayGanadorSet(juegos1, juegos2);

		if (saque == TENISTA1) {
			saque = TENISTA2;
		}
		else if (saque == TENISTA2) {
			saque = TENISTA1;
		}
		cout << endl;
		system("pause");
		cout << endl;
	}
	pintarMarcador(iniciales_t1, iniciales_t2, puntuacion, puntuacion, juegos1, juegos2, saque);
	if (ganador_set == TENISTA1) {
		ganador = iniciales_t1;
	}
	else if (ganador_set == TENISTA2) {
		ganador = iniciales_t2;
	}
	cout << endl;
	cout << "El ganador es: " << ganador << endl;
	cout << endl;
	cout << "El juego ha terminado,gracias por participar!" << endl;

	return 0;
}

tTenista saqueAleatorio(string iniciales_t1, string iniciales_t2) {
	int saque;
	tTenista jugador_actual = NADIE;

	srand(time(NULL)); // se utiliza la biblioteca <ctime>
	saque = rand() % 2; // generará un numero entre 0 y 1 (si queremos entre 0,1 y 2:rand()%2+1
	if (saque == 0) {
		jugador_actual = TENISTA1;
		cout << "Servicio para: " << iniciales_t1 << endl;
	}
	if (saque == 1) {
		jugador_actual = TENISTA2;
		cout << "Servicio para: " << iniciales_t2 << endl;
	}
	return jugador_actual;
}

void introducirTenista(string& iniciales, int& habilidad, int& velocidad) {
	cout << "- Introduzca sus iniciales (3 letras exactamente): ";
	cin >> iniciales;
	while (iniciales.length() != 3) {
		cout << "El numero de caracteres debe ser 3, vuelva a intentarlo: ";
		cin >> iniciales;
	}

	cout << "- Introduzca su habilidad (valor entre 1 y 2): ";
	cin >> habilidad;
	while (habilidad<LIM_INF_HAB || habilidad>LIM_SUP_HAB) {
		cout << "El valor no es valido, vuelva a intentarlo: ";
		cin >> habilidad;
	}

	cout << "- Introduzca su velocidad (valor entre 1 y 3): ";
	cin >> velocidad;
	while (velocidad<LIM_INF_VEL || velocidad>LIM_SUP_VEL) {
		cout << "El valor no es valido, vuelva a intentarlo: ";
		cin >> velocidad;
	}
}

string puntosAstring(tPuntosJuego puntuacion) {
	string puntos;
	//puntuacion = puntuacion + 1;
	if (puntuacion == NADA) {
		puntos = "00";
	}
	if (puntuacion == QUINCE) {
		puntos = "15";
	}
	if (puntuacion == TREINTA) {
		puntos = "30";
	}
	if (puntuacion == CUARENTA) {
		puntos = "40";
	}
	if (puntuacion == VENTAJA) {
		puntos = "Ad";
	}
	return puntos;
}

void pintarMarcador(string nombre1, string nombre2, tPuntosJuego puntos1, tPuntosJuego puntos2, int juegos1, int juegos2, tTenista servicio_para) {
	string marcador_t1, marcador_t2;
	marcador_t1 = puntosAstring(puntos1);
	marcador_t2 = puntosAstring(puntos2);

	if (servicio_para == TENISTA1) {
		cout << nombre1 << "  " << juegos1 << " : " << marcador_t1 << " *" << endl;
		cout << nombre2 << "  " << juegos2 << " : " << marcador_t2 << endl;
	}
	if (servicio_para == TENISTA2) {
		cout << nombre1 << "  " << juegos1 << " : " << marcador_t1 << endl;
		cout << nombre2 << "  " << juegos2 << " : " << marcador_t2 << " *" << endl;
	}
}

int golpeoBola(int posicion_tenista, int habilidad) {
	int calle_destino, distancia, num_aleatorio, desviacion;
	double prob_acierto;

	if (JUEGO_ALEATORIO == true) { //se genera aleatoriamente el destino de la bola
		calle_destino = rand() % 8;
	}
	else if (JUEGO_ALEATORIO == false) {
		cin >> calle_destino; //el jugador introduce la calle a la que desea enviar la bola
		if (MODO_DEBUG == true) {
			cout << "\n El jugador dispara hacia la calle " << calle_destino << endl;
		}
	}
	if ((calle_destino < 1) || (calle_destino > ANCHO_PISTA)) {
		if (MODO_DEBUG == true) {
			cout << "\n La bola ha salido del campo" << endl;
		}
		calle_destino = calle_destino;
	}
	else {
		distancia = abs(calle_destino - posicion_tenista);
		if (distancia <= habilidad) {
			calle_destino = calle_destino;
			if (MODO_DEBUG == true) {
				cout << "\n Ese ha sido un tiro sencillo" << endl;
				cout << "\n La bola llega a la casilla " << calle_destino << endl;
			}
		}
		else if (distancia > habilidad) {
			prob_acierto = 100 - ((calle_destino - habilidad) / ((ANCHO_PISTA - 1) - LIM_INF_HAB)) * 100;
			num_aleatorio = rand() % 99 + 1;
			if (num_aleatorio < prob_acierto) {
				calle_destino = calle_destino;
				if (MODO_DEBUG == true) {
					cout << "\n Tiro complicado que... (probab_exito= " << prob_acierto << " y resultado = " << num_aleatorio << ") Llega a su destino!" << endl;
					cout << "\n La bola llega a la casilla " << calle_destino << endl;
				}
			}
			else { //se puede eliminar el if anterior y poer este con condicion (num_aleatorio>prob)
				desviacion = rand() % 2;
				if (desviacion == 0) {
					calle_destino = calle_destino + 1; //la bola se desvía a la derecha
				}
				if (desviacion == 1) {
					calle_destino = calle_destino - 1; //la bola se desvía a la derecha
				}
				if (MODO_DEBUG == true) {
					cout << "\n Tiro complicado que... (probab_exito= " << prob_acierto << " y resultado = " << num_aleatorio << ") No ha sido preciso!" << endl;
					cout << "\n La bola acaba en la casilla " << calle_destino << endl;
				}
			}
		}
	}

	return calle_destino;
}

int correTenista(int posicion_tenista, int velocidad, int posicion_bola) {
	int distancia_jugador_bola;

	distancia_jugador_bola = abs(posicion_tenista - posicion_bola);
	if (velocidad >= distancia_jugador_bola) {
		posicion_tenista = posicion_bola;
	}
	else { //si la distancia es > velocidad, el jugador no llega a la bola, pero corre en su dirección
		if (posicion_bola < posicion_tenista) {
			posicion_tenista = posicion_tenista - velocidad;
		}
		if (posicion_bola > posicion_tenista) {
			posicion_tenista = posicion_tenista + velocidad;
		}
		if (MODO_DEBUG == true) {
			cout << "\n Su rival no llega a la bola." << endl;
		}
	}

	return posicion_tenista; //devuelve la posición a la que llega el tenista que va a por la pelota
}

void horizontalesCampo() {
	cout << " ";
	for (int i = 0; i < ANCHO_PISTA; i++) {
		cout << " -";
	}
	cout << endl;
}

void verticalesCampo() {
	cout << " ";
	for (int i = 0; i < ANCHO_PISTA + 1; i++) {
		cout << "| ";
	}
	cout << endl;
}

void pintarTenista(string nombre, int pos_tenista) {
	cout << " ";

	for (int i = 1; i < pos_tenista; i++) {
		cout << "  ";
	}
	cout << nombre << endl;
}

void pintarBola(int posicion_bola) {
	cout << " ";
	for (int i = 0; i < posicion_bola - 1; i++) {
		cout << "| ";
	}
	cout << "|o| ";
	for (int i = posicion_bola; i < ANCHO_PISTA; i++) {
		cout << "| ";
	}
	cout << endl;
}

void pintarPeloteo(string nombre1, string nombre2, int pos_t1, int pos_t2, tTenista bola_jugador, int pos_bola) {
	pintarTenista(nombre1, pos_t1);
	horizontalesCampo();
	if (bola_jugador == TENISTA1) {
		pintarBola(pos_bola);
	}
	else {
		verticalesCampo();
	}
	verticalesCampo();
	verticalesCampo();

	cout << "-";
	for (int i = 1; i <= ANCHO_PISTA; i++) { //Dibuja la red con los números de cada calle
		cout << "-" << i;
	}
	cout << "--" << endl;

	verticalesCampo();
	verticalesCampo();
	if (bola_jugador == TENISTA2) {
		pintarBola(pos_bola);
	}
	else {
		verticalesCampo();
	}
	horizontalesCampo();
	pintarTenista(nombre2, pos_t2);
}

tTenista lance(tTenista tenista_golpea, string nombre, int habilidad, tConteoGolpes golpes, int& golpes_ganados, int velocidad, int& pos_recibe, int& pos_bola) {
	//realiza el golpe de la bola usando golpeBola(), actualizando la posición de esta.
	tTenista tenista_receptor, ganador_lance = NADIE;
	//int errores_no_forzados1=0, errores_no_forzados2=0;

	cout << "Golpea " << nombre << endl;
	if (tenista_golpea == TENISTA1) {
		pos_bola = golpeoBola(pos_recibe, habilidad); //realiza el saque
		golpes[pos_bola] = golpes[pos_bola] + 1;

		if (pos_bola > 0 && pos_bola <= ANCHO_PISTA) { //mientras la bola se encuentre en el campo
			//tenista_receptor = TENISTA2;
			pos_recibe = correTenista(pos_recibe, velocidad, pos_bola); //el rival corre en dirección a la bola
			if (pos_recibe != pos_bola) {
				ganador_lance = TENISTA1;
				golpes_ganados++;
			}
			else if (pos_recibe == pos_bola) {
				ganador_lance = NADIE;
			}
		}
		else if (pos_bola <= 0 || pos_bola > ANCHO_PISTA) {
			//errores_no_forzados1++;
			ganador_lance = TENISTA2;
		}
	}
	if (tenista_golpea == TENISTA2) {
		pos_bola = golpeoBola(pos_recibe, habilidad); //realiza el saque
		golpes[pos_bola] = golpes[pos_bola] + 1;

		if (pos_bola > 0 && pos_bola <= ANCHO_PISTA) { //mientras la bola se encuentre en el campo
			//tenista_receptor = TENISTA1;
			pos_recibe = correTenista(pos_recibe, velocidad, pos_bola); //el rival corre en dirección a la bola
			if (pos_recibe != pos_bola) {
				ganador_lance = TENISTA2;
				golpes_ganados++;
			}
			else if (pos_recibe == pos_bola) {
				ganador_lance = NADIE;
			}
		}
		else if (pos_bola <= 0 || pos_bola > ANCHO_PISTA) {
			//errores_no_forzados2++;
			ganador_lance = TENISTA1;
		}
	}

	return ganador_lance;
}

tTenista jugarPunto(tTenista servicio, string nombre1, int habilidad1, int velocidad1, tConteoGolpes golpes1, int& golpes_ganados1, string nombre2, int habilidad2, int velocidad2, tConteoGolpes golpes2, int& golpes_ganados2) {
	//conduce el juego de un punto
	tTenista sacar = NADIE;
	tTenista ganador_punto;
	int pos_t1 = ANCHO_PISTA / 2 + 1, pos_t2 = ANCHO_PISTA / 2 + 1, pos_bola = ANCHO_PISTA / 2 + 1;

	golpes_ganados1 = 0;
	golpes_ganados2 = 0;

	while (sacar == NADIE) {
		if (servicio == TENISTA1) {
			pintarPeloteo(nombre1, nombre2, pos_t1, pos_t2, TENISTA1, pos_bola);
			cout << endl;
			system("pause");
			sacar = lance(servicio, nombre1, habilidad1, golpes1, golpes_ganados1, velocidad1, pos_t2, pos_bola);

			if (sacar == NADIE) {
				servicio = TENISTA2;
			}
		}
		if (servicio == TENISTA2) {
			pintarPeloteo(nombre1, nombre2, pos_t1, pos_t2, TENISTA2, pos_bola);
			cout << endl;
			system("pause");
			sacar = lance(servicio, nombre2, habilidad2, golpes2, golpes_ganados2, velocidad2, pos_t1, pos_bola);

			if (sacar == NADIE) {
				servicio = TENISTA1;
			}
		}
	}

	if (sacar == TENISTA1) {
		ganador_punto = TENISTA1;
		pintarPeloteo(nombre1, nombre2, pos_t1, pos_t2, TENISTA2, pos_bola);

	}
	else {
		ganador_punto = TENISTA2;
		pintarPeloteo(nombre1, nombre2, pos_t1, pos_t2, TENISTA1, pos_bola);
	}

	return ganador_punto;
}

tTenista jugarJuego(tTenista servicio, string nombre1, int habilidad1, int velocidad1, int& juegos1, tConteoGolpes golpes1, int& golpes_ganados1, string nombre2, int habilidad2, int velocidad2, int& juegos2, tConteoGolpes golpes2, int& golpes_ganados2) {
	tTenista ganador_punto;
	tTenista ganador_juego = NADIE;
	tPuntosJuego puntos1 = NADA, puntos2 = NADA;

	do {
		cout << endl;
		pintarMarcador(nombre1, nombre2, puntos1, puntos2, juegos1, juegos2, servicio);
		cout << endl;
		ganador_punto = jugarPunto(servicio, nombre1, habilidad1, velocidad1, golpes1, golpes_ganados1, nombre2, habilidad2, velocidad2, golpes2, golpes_ganados2);
		if (ganador_punto == TENISTA1) {
			cout << "\n El jugador " << nombre1 << " ha ganado el punto." << endl;
		}
		else if (ganador_punto == TENISTA2) {
			cout << "\n El jugador " << nombre2 << " ha ganado el punto." << endl;
		}
		cout << endl;
		ganador_juego = actualizarMarcador(ganador_punto, puntos1, puntos2, juegos1, juegos2);
	} while (ganador_juego == NADIE);

	if (ganador_juego == TENISTA1) {
		cout << "\n El ganador del juego es " << nombre1 << endl;
		cout << endl;
	}
	else if (ganador_juego == TENISTA2) {
		cout << "\n El ganador del juego es " << nombre2 << endl;
		cout << endl;
	}

	return ganador_juego;
}

tTenista hayGanadorSet(int juegos1, int juegos2) {
	tTenista ganador = NADIE;
	int diferencia;

	if (juegos1 >= JUEGOS_SET || juegos2 >= JUEGOS_SET) {
		diferencia = abs(juegos1 - juegos2); //gana el que consiga ganar JUEGOS_SET con al menos 2 juegos de ventaja.
		if (diferencia >= 2) {
			if (juegos1 > juegos2) {
				ganador = TENISTA1;
			}
			else if (juegos2 > juegos1) {
				ganador = TENISTA2;
			}
		}
	}
	return ganador;
}

tTenista actualizarMarcador(tTenista ganador_punto, tPuntosJuego& puntos1, tPuntosJuego& puntos2, int& juegos1, int& juegos2) {
	tTenista ganador = NADIE;
	int puntuacion1, puntuacion2, diferencia;

	puntuacion1 = (int)puntos1; //Se asocian los valores numericos con el tipo tPuntosJuego
	puntuacion2 = (int)puntos2;

	if (ganador_punto == TENISTA1) { //Incrementar puntuación del ganador
		puntuacion1++;
	}
	else if (ganador_punto == TENISTA2) {
		puntuacion2++;
	}
	if (puntuacion1 == 4 && puntuacion2 == 4) { //Si ambos empatan en la ventaja, restarles un punto
		puntuacion1 = 3;
		puntuacion2 = 3;
	}
	//Comprobar el ganador del punto:
	if (puntos1 > 3 || puntos2 > 3) {
		diferencia = abs(puntos1 - puntos2);
		if (diferencia >= 2) {
			if (puntos1 > puntos2) {
				ganador = TENISTA1;
				juegos1++;
			}
			else {
				ganador = TENISTA2;
				juegos2++;
			}
		}
	}
	else {
		ganador = NADIE;
	}
	puntos1 = (tPuntosJuego)puntuacion1; //Volver a convertir la variable al valor utilizado en el parámetro
	puntos2 = (tPuntosJuego)puntuacion2;

	return ganador;
}

void mostrarEstadisticas(string nombre, tConteoGolpes golpes, int golpes_ganados) {
	int total_golpes = 0;
	double porcentaje;
	int errores_no_forzados = golpes[0] + golpes[ANCHO_PISTA + 1];

	for (int i = 0; i <= ANCHO_PISTA + 1; i++) { //Calcula los golpes totales
		total_golpes += golpes[i];
	}

	cout << "Estadisticas de " << nombre << endl;
	cout << "\t Golpes totales: " << total_golpes << endl;
	cout << "\t Golpes ganadores: " << golpes_ganados << endl;
	cout << "\t Errores no forzados: " << errores_no_forzados << endl;
	cout << "\t Distribucion de los golpes en pista" << endl;
	cout << "\t \t Calle"; // \t 0 \t 1 \t 2 \t 3 \t 4 \t 5 \t 6 \t 7 \t 8 << endl;
	for (int k = 0; k <= ANCHO_PISTA + 1; k++) {
		cout << "\t " << k;
	}
	cout << endl;
	cout << "\t \t %";

	for (int j = 0; j <= ANCHO_PISTA + 1; j++) { //Calcula la distribución de las calles
		if (golpes[j] != 0) {
			porcentaje = golpes[j] * 100 / total_golpes;
		}
		if (golpes[j] == 0) {
			porcentaje = 0;
		}
		cout << "\t " << porcentaje;
	}
	//cout<<endl;
}