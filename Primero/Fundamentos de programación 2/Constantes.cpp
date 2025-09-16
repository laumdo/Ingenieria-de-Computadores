#include "Constantes.h"

string dirToString(Direccion d) {
	string dir = "";
	if (d == ARRIBA) {
		dir = "arriba";
	}
	else if (d == ABAJO) {
		dir = "abajo";
	}
	else if (d == IZQUIERDA) {
		dir = "izquierda";
	}
	else if (d == DERECHA) {
		dir = "derecha";
	}
	return dir;
}

