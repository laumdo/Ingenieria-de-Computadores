#include "Tablero.h"

Tablero::Tablero() {

}

Tablero::Tablero(int fil, int col) {
	filas = fil;
	columnas = col;
	for (int i = 0; i < filas; i++) {
		for (int j = 0; j < columnas; j++) {
			celdas[i][j] = NULA;
		}
	}
}

int Tablero::num_filas() const {
	return filas;
}

int Tablero::num_columnas() const {
	return columnas;
}

bool Tablero::correcta(int f, int c) const {
	return (f > 0 && f < filas && c>0 && c < columnas);
}

Celda Tablero::leer(int f, int c) const {
	return celdas[f][c];
}

void Tablero::escribir(int f, int c, Celda valor) {
	celdas[f][c] = valor;
}