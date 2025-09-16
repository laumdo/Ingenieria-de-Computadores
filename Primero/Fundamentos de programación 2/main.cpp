
#include "colores.h"
#include <iomanip>
#include <iostream>

using namespace std;

/*
* Esto es un ejemplo de c�mo pintar un tablero constante
* con el mismo formato que aparece en el enunciado de la
* pr�ctica.
* Las funciones que aparecen a continuaci�n hacen uso de
* varias constantes, para el tablero y sus dimensiones,
* o la posici�n de la meta.
* En la pr�ctica, esas funciones seguramente sean m�todos
* de una clase con acceso a esos datos del juego.
*/

const int FILS = 4;
const int COLS = 5;
const  int tablero[FILS][COLS] = {
{0, 0, 1, 2, 2},
{2, 1, 2, 2, 0},
{0, 1, 1, 2, 0},
{0, 0, 0, 0, 0}
};

const int f_meta = 2, c_meta = 1;

enum Celda { NULA, VACIA, FICHA };

const int DEFAULT_COLOR = -1;
void color_fondo(int color) {
    if (color == DEFAULT_COLOR)
        cout << RESET;
    else if (color == NULA)
        cout << BG_BLACK;
    else if (color == FICHA)
        cout << BG_LBLUE;
    else // color == VACIA
        cout << BG_ORANGE;
}

void pinta_cabecera() {
    cout << setw(2) << "    "; // margen inicial
    cout << setw(5) << 1;
    for (int i = 2; i <= COLS; i++) {
        cout << setw(7) << i;
    }
    cout << endl;
}

void pinta_linea(char esquinaIzda, char cruce, char esquinaDer) {
    cout << "    "; // margen inicial
    cout << esquinaIzda;
    for (int i = 0; i < COLS - 1; i++) {
        cout << string(6, char(196)) << cruce;
    }
    cout << string(6, char(196)) << esquinaDer << endl;
}


void pinta_borde_celda(int fila) {
    cout << "    "; // margen inicial
    for (int k = 0; k < COLS; k++) { // cada columna
        cout << char(179);
        color_fondo(tablero[fila][k]);
        cout << "      ";
        color_fondo(DEFAULT_COLOR);
    }
    cout << char(179) << endl; // lateral derecho
}

void pinta_centro_celda(int fila) {
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

int main() {
    system("cls"); // borrar consola
    cout << RESET;

    // borde superior
    pinta_cabecera();
    pinta_linea(char(218), char(194), char(191));
    // para cada fila
    for (int fil = 0; fil < FILS; fil++) {
        // primera l�nea
        pinta_borde_celda(fil);
        // segunda l�nea, con la meta posiblemente
        pinta_centro_celda(fil);
        // tercera l�nea
        pinta_borde_celda(fil);
        // separaci�n entre filas
        if (fil < FILS - 1) {
            pinta_linea(char(195), char(197), char(180));
        }
        else {
            pinta_linea(char(192), char(193), char(217));
        }
    }

}