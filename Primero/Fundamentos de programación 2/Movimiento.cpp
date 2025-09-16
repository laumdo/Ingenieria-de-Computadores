#include"Movimiento.h"

Movimiento::Movimiento(int f, int c) {
	fil = f;
	col = c;
}

int Movimiento::fila() const {
	return fil;
}

int Movimiento::columna() const {
	return col;
}

Direccion Movimiento::dir_activa() const {
	return activa;
}

void Movimiento::fijar_dir_activa(Direccion d) {
	activa = d;
}

void Movimiento::insertar_dir(Direccion d) { ////????????????
    if (cont >= NUMDIR) {
        cout << "No se puede insertar m�s direcciones. L�mite alcanzado." << endl;
        return;
    }


    // Insertar la direcci�n en la lista de direcciones posibles
    direcciones[cont] = d;
    cont++;

	//seg�n el estado del juego?
}

int Movimiento::num_dirs() const {
	return cont;
}

Direccion Movimiento::direccion(int i) const {
	return direcciones[i];
}