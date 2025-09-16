#include "Juego.h"
#include<fstream>
#include<iomanip>

void Juego::ejecuta_movimiento(const Movimiento& mov) {

}

void Juego::nuevo_estado() {

}

bool Juego::hay_ganador() const {
	return estado_int == GANADOR;
}

bool Juego::hay_movimientos() const {
	return false;
}

Juego::Juego() {
	estado_int = BLOQUEO;
	for (int i = 0; i < tablero.num_filas(); i++) {
		for (int j = 0; j < tablero.num_columnas(); j++) {
			tablero.escribir(i, j, VACIA);
		}
	}
}

istream& operator>>(istream& in, Tablero tablero) {
	

	return in;
}

bool Juego::cargar(istream& entrada) { //????
	bool cargado = false;
	string nombreArchivo;
	cout << "Indica el nombre del archivo: ";
	cin >> nombreArchivo;

	
	int f, c;
	entrada >> f >> c;
	Tablero tab(f, c);
	tablero = tab;

	for (int i = 0; i < tablero.num_filas(); i++) {
		for (int j = 0; j < tablero.num_columnas(); j++) {
			int c;
			entrada >> c;
			tablero.escribir(i, j, intToCelda(c));
		}
	}
	entrada >> f_meta >> c_meta;
    estado_int = JUGANDO;
	recuentoCeldas(); //cuenta el numero de fichas que hay en el tablero cargado


	return cargado;
}


bool Juego::posicion_valida(int f, int c) const {
	return tablero.correcta(f, c) && tablero.leer(f,c)==FICHA;
}

void Juego::posibles_movimientos(Movimiento& mov) const {
    int cont = 0;

    for (int i = 0; i < NUMDIR; ++i) {
        // Calcular la posición de destino en base a la dirección
        int fila_salta = mov.fila() + DIRS[i][0];
        int columna_salta = mov.columna() + DIRS[i][1];
        int fila_destino = mov.fila() + DIRS[i][0] * 2;
        int columna_destino = mov.columna() + DIRS[i][1] * 2;
        if (tablero.correcta(fila_salta, columna_salta) && tablero.leer(fila_salta, columna_salta) == FICHA && tablero.leer(fila_destino,columna_destino)==VACIA) {
            Direccion d = convertirDir(i);
            mov.insertar_dir(d);
            cont++;
        }
    }

    //Si no fuera posible ningún movimiento(la ficha está bloqueada)

    // Verificar si la dirección es válida (horizontal o vertical)
    if (d != ARRIBA && d != ABAJO && d != IZQUIERDA && d != DERECHA) {
        cout << "Dirección inválida." << endl;
        return;
    }
}


Estado Juego::estado() const {
	return estado_int;
}

void Juego::jugar(const Movimiento& mov) {

}

void Juego::mostrar() const {

}

//Funciones privadas:
Celda Juego::intToCelda(int n) {
	Celda sol = NULA;

	if (n == 1)
		sol = VACIA;
	else if (n == 2)
		sol = FICHA;

	return sol;
}

Direccion Juego::convertirDir(int i) {
    Direccion d = INDETERMINADA;
    if (i == 0) {
        d = ARRIBA;
    }
    else if (i == 1) {
        d = ABAJO;
    }
    else if (i == 2) {
        d = IZQUIERDA;
    }
    else if (i == 3) {
        d = DERECHA;
    }
    return d;
}

const int DEFAULT_COLOR = -1;
void Juego::color_fondo(int color) {
    if (color == DEFAULT_COLOR)
        cout << RESET;
    else if (color == NULA)
        cout << BG_BLACK;
    else if (color == FICHA)
        cout << BG_LBLUE;
    else // color == VACIA
        cout << BG_ORANGE;
}

void Juego:: pinta_cabecera() {
    cout << setw(2) << "    "; // margen inicial
    cout << setw(5) << 1;
    for (int i = 2; i <= ; i++) {
        cout << setw(7) << i;
    }
    cout << endl;
}

void Juego::pinta_linea(char esquinaIzda, char cruce, char esquinaDer) {
    cout << "    "; // margen inicial
    cout << esquinaIzda;
    for (int i = 0; i < COLS - 1; i++) {
        cout << string(6, char(196)) << cruce;
    }
    cout << string(6, char(196)) << esquinaDer << endl;
}


void Juego::pinta_borde_celda(int fila) {
    cout << "    "; // margen inicial
    for (int k = 0; k < COLS; k++) { // cada columna
        cout << char(179);
        color_fondo(tablero[fila][k]);
        cout << "      ";
        color_fondo(DEFAULT_COLOR);
    }
    cout << char(179) << endl; // lateral derecho
}

void Juego::pinta_centro_celda(int fila) {
    cout << "  " << setw(2) << fila + 1; // margen inicial
    for (int k = 0; k < COLS; k++) { // cada columna
        cout << char(179);
        // el color de fondo depende del contenido
        color_fondo(tablero[fila][k]);

        if (fila == f_meta && k == c_meta) { // meta
            cout << YELLOW;
            cout << "  " << char(219) << char(219) << "  ";
        }
        else {
            cout << "      ";
        }
        color_fondo(DEFAULT_COLOR);
    }
    cout << char(179) << endl; // lateral derecho
}

void Juego::recuentoCeldas() {
	for (int i = 0; i < tablero.num_filas(); i++) {
		for (int j = 0; j < tablero.num_columnas(); j++) {
			Celda c = tablero.leer(i, j);
			if (c == FICHA) {
				n_fichas++;
			}
		}
	}
}