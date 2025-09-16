#pragma once

#pragma once
#include<iostream>
#include"Movimiento.h"
#include "colores.h"

class Juego {
private:
	Tablero tablero;
	int f_meta, c_meta;
	Estado estado_int;
	int n_fichas;
	void ejecuta_movimiento(const Movimiento& mov);
	void nuevo_estado();
	bool hay_ganador() const;
	bool hay_movimientos() const;

	Celda intToCelda(int n);
	Direccion convertirDir(int i);
	void recuentoCeldas();
	//Para pintar tablero:
	void color_fondo(int color);
	void pinta_cabecera();
	void pinta_linea(char esquinaIzda, char cruce, char esquinaDer);
	void pinta_borde_celda(int fila);
	void pinta_centro_celda(int fila);

public:
	Juego();
	bool cargar(istream& entrada);
	bool posicion_valida(int f, int c) const;
	void posibles_movimientos(Movimiento& mov) const;
	Estado estado() const;
	void jugar(const Movimiento& mov);
	void mostrar() const;
};
