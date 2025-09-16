#pragma once
#include <iostream>
#include"Constantes.h"

using namespace std;

class Tablero {
private:
	int filas, columnas;
	Celda celdas[MAXDIM][MAXDIM];
public:
	Tablero();
	Tablero(int fil, int col);
	int num_filas() const;
	int num_columnas() const;
	bool correcta(int f, int c) const;
	Celda leer(int f, int c) const;
	void escribir(int f, int c, Celda valor);
};